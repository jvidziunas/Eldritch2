/*==================================================================*\
  FiberScheduler.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/Win32/FiberScheduler.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Platform/UserReadWriteMutex.hpp>
#include <Platform/PlatformInterface.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Platform/UserWaitableEvent.hpp>
#include <Platform/UserSemaphore.hpp>
#include <Utility/Algorithms.hpp>
#include <Utility/Result.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <Windows.h>
#include <Objbase.h>
#include <Process.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {
namespace Win32 {
namespace {

	static ETInlineHint ETPureFunctionHint size_t GetRandom( size_t& seed ) {
		enum : size_t {
			Shift	= static_cast<size_t>( sizeof( size_t ) == sizeof( uint64 ) ? 32u : 16u ),
			Mask	= static_cast<size_t>( sizeof( size_t ) == sizeof( uint64 ) ? 0x7FFFFFFF : 0x7FFF )
		};

		seed = ( 214013u * seed + 2531011u );
		return ( seed >> Shift ) & Mask;
	}

}	// anonymous namespace

	FiberScheduler::FiberScheduler( Allocator& allocator ) : _allocator( allocator, "Scheduling Root Allocator" ), _workers( nullptr ) {
		MicroProfileInit();
	}

// ---------------------------------------------------

	FiberScheduler::~FiberScheduler() {
		for( auto& worker : _workers ) {
			worker.AwaitCompletion();
		}

		MicroProfileShutdown();
	}

// ---------------------------------------------------

	ErrorCode FiberScheduler::EnqueueOnCaller( Thread& thread ) {
#pragma pack( push, 8 )
	//	Taken from http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
		struct THREADNAME_INFO {
			DWORD	dwType;		//!<	Must be 0x1000.
			LPCSTR	szName;		//!<	Pointer to name (in user address space).
			DWORD	dwThreadID;	//!<	Thread ID (-1 = caller thread).
			DWORD	dwFlags;	//!<	Reserved for future use, must be 0.
		};
#pragma pack( pop )

		enum {
			MS_VC_EXCEPTION = 0x406D1388
		};

	// ---

		MicroProfileOnThreadCreate( thread.GetHumanReadableName() );

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
	/*	MSVC warns about the SEH code used here to name a thread in the debugger.
		This is safe and is taken from official documentation, so disable the warnings here. */
#	pragma warning( disable : 6312 )
#	pragma warning( disable : 6322 )
#endif
		if( 0 != IsDebuggerPresent() ) {
			THREADNAME_INFO	info = {
				0x1000,
				thread.GetHumanReadableName(),
				static_cast<DWORD>( -1 ),
				0u,
			};

			__try {
				RaiseException( MS_VC_EXCEPTION, 0, sizeof( info ) / sizeof( ULONG_PTR ), reinterpret_cast<ULONG_PTR*>( &info ) );
			} __except( EXCEPTION_CONTINUE_EXECUTION ) {}
		}
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif

		ETRuntimeVerificationWithMessage( SUCCEEDED( CoInitializeEx( nullptr, 0 ) ), "Error in CoInitializeEx()!" );

		thread.SchedulerEntryPoint();

		CoUninitialize();

		MicroProfileOnThreadExit();
		return Error::None;
	}

// ---------------------------------------------------

	ErrorCode FiberScheduler::Enqueue( Thread& thread ) {
		struct CreateInfo {
			FiberScheduler*	scheduler;
			Thread*			thread;
		}	createInfo;

		auto threadEntryPoint( [] ( void* createInfo ) -> unsigned int {
			static_cast<CreateInfo*>(createInfo)->scheduler->EnqueueOnCaller( *static_cast<CreateInfo*>(createInfo)->thread );
			return 0;
		} );

		createInfo.scheduler	= this;
		createInfo.thread		= &thread;

	//	Create a new operating system thread.
		if( const auto threadHandle = _beginthreadex( nullptr, 0, threadEntryPoint, &createInfo, 0, nullptr ) ) {
			for( BackoffContext backoffContext( 0u ); !thread.HasStartedExecution(); ) {
			//	Wait until the thread signals it's copied the information safely.
				Backoff( backoffContext );
			}

			return Error::None;
		}

	//	We received an invalid handle, let's figure out why.
		switch( errno ) {
			case EINVAL:	{ return Error::InvalidParameter;	break; }	// case EINVAL
			case EACCES:	{ return Error::OutOfMemory;		break; }		// case EACCES
		}	// switch( errno )

		return Error::Unspecified;
	}

// ---------------------------------------------------

	void FiberScheduler::ShutDown() {
		for( WorkerThread& worker : _workers ) {
			worker.RequestGracefulShutdown();
		}
	}

// ---------------------------------------------------

	void FiberScheduler::Backoff( BackoffContext& context ) {
		const BackoffContext	backoff( context );

		if( backoff < 10u ) {
			_mm_pause();
		} else if( backoff < 20u ) {
			for( int i( 0u ); i != 25u; ++i ) {
				_mm_pause();
				_mm_pause();
			}
		} else if( backoff < 22u ) {
			SwitchToThread();
		} else if( backoff < 24u ) {
			Sleep( 0u );
		} else if( backoff < 26u ) {
			Sleep( 1u );
		} else {
			Sleep( 10u );
		}

		 ++context;
	}

// ---------------------------------------------------

	void FiberScheduler::SetCallerCoreAffinity( ProcessorAffinity affinity ) const {
		SetThreadIdealProcessor( GetCurrentThread(), static_cast<DWORD>(affinity) );
	}

// ---------------------------------------------------

	void FiberScheduler::SetCallerExecutionPriority( ThreadExecutionPriority priority ) {
		static const int priorities[] = {
			THREAD_PRIORITY_BELOW_NORMAL,
			THREAD_PRIORITY_NORMAL,
			THREAD_PRIORITY_ABOVE_NORMAL,
			THREAD_PRIORITY_TIME_CRITICAL
		};

		static_assert(_countof(priorities) == static_cast<size_t>(ThreadExecutionPriority::COUNT), "Thread priority table needs update!");

	// ---

		SetThreadPriority( GetCurrentThread(), priorities[static_cast<size_t>(priority)] );
	}

// ---------------------------------------------------

	FiberScheduler::WorkerThread& FiberScheduler::GetRandomWorker( const WorkerThread& executingWorker, size_t& randomSeed ) {
		size_t	index( GetRandom( randomSeed ) % _workers.GetSize() );

	//	Ensure we don't pick the calling worker for theft.
		if( &_workers[index] == &executingWorker ) {
			index = (index + 1) % _workers.GetSize();
		}

		return _workers[index];
	}

// ---------------------------------------------------

	size_t FiberScheduler::GetMaximumParallelism() const {
	//	TODO: It might be prudent to make this a constant initialized at construction time.
		return static_cast<size_t>(_workers.GetSize());
	}

}	// namespace Win32
}	// namespace Scheduling
}	// namespace Eldritch2

