/*==================================================================*\
  FiberJobSystem.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/Win32/FiberJobSystem.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <Windows.h>
#include <Objbase.h>
#include <Process.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {
namespace Win32 {

	FiberJobSystem::FiberJobSystem(
	) : _allocator( "Job System Root Allocator" ),
		_workers( nullptr ),
		_shutdownCode( AsPosixInt( Error::Unspecified ) ) {
	}

// ---------------------------------------------------

	FiberJobSystem::~FiberJobSystem() {
		for (JobThread& thread : _workers) {
			thread.AwaitCompletion();
		}
	}

// ---------------------------------------------------

	ErrorCode FiberJobSystem::LaunchOnCaller( Thread& thread ) {
		enum { MS_VC_EXCEPTION = 0x406D1388 };

#pragma pack( push, 8 )
	//	Taken from http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
		struct THREADNAME_INFO {
			DWORD	dwType;		//	Must be 0x1000.
			LPCSTR	szName;		//	Pointer to name (in user address space).
			DWORD	dwThreadID;	//	Thread ID (-1 = caller thread).
			DWORD	dwFlags;	//	Reserved for future use, must be 0.
		};
#pragma pack( pop )

	// ---

		MicroProfileOnThreadCreate( thread.GetName() );

	ET_PUSH_COMPILER_WARNING_STATE()
/*	(6312, 6322) MSVC warns about the SEH code used here to name a thread in the debugger. This is safe and is taken from official
 *	documentation, so disable the warnings here. */
	ET_SET_MSVC_WARNING_STATE( disable : 6312 6322 )
		if (IsDebuggerPresent() != FALSE) {
			THREADNAME_INFO	info = {
				0x1000,
				thread.GetName(),
				static_cast<DWORD>(-1),
				0u,
			};

		//	Raise a magic SEH exception, which Visual Studio intercepts.
			__try {
				RaiseException( MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR*>( &info ) );
			} __except( EXCEPTION_CONTINUE_EXECUTION ) {}
		}
	ET_POP_COMPILER_WARNING_STATE()

		ET_VERIFY( SUCCEEDED( CoInitializeEx( nullptr, 0 ) ), "Error in CoInitializeEx()!" );

		thread.Enter();

		CoUninitialize();

		MicroProfileOnThreadExit();
		return Error::None;
	}

// ---------------------------------------------------

	ErrorCode FiberJobSystem::Launch( Thread& thread ) {
		struct CreateInfo {
			FiberJobSystem*	scheduler;
			Thread*			thread;
		}	createInfo;

		const auto entryPoint( [] ( void* createInfo ) -> unsigned int {
			static_cast<CreateInfo*>(createInfo)->scheduler->LaunchOnCaller( *static_cast<CreateInfo*>(createInfo)->thread );
			return 0;
		} );

		createInfo.scheduler	= this;
		createInfo.thread		= &thread;

		if (!_beginthreadex( nullptr, 0, entryPoint, &createInfo, 0, nullptr )) {
			switch (errno) {
				case EINVAL:	return Error::InvalidParameter;
				case EACCES:	return Error::OutOfMemory;
				default:		return Error::Unspecified;
			}	// switch( errno )
		}

		BackoffContext backoffContext( 0u );

		while (!thread.HasStarted()) {
		//	Wait until the thread signals it's copied the information safely.
			BackOff( backoffContext );
		}

		return Error::None;
	}

// ---------------------------------------------------

	void FiberJobSystem::SetShouldShutDown( int shutdownCode ) {
		_shutdownCode.store( shutdownCode, std::memory_order_relaxed );

		for (JobThread& thread : _workers) {
			thread.SetShouldShutDown();
		}
	}

// ---------------------------------------------------

	void FiberJobSystem::BackOff( BackoffContext& context ) {
		const BackoffContext	backoff( context++ );

		if (backoff < 10u) {
			_mm_pause();
		} else if (backoff < 20u) {
			for (int i( 0u ); i != 25u; ++i) {
				_mm_pause();
				_mm_pause();
			}
		} else if (backoff < 22u) {
			SwitchToThread();
		} else if (backoff < 24u) {
			Sleep( 0u );
		} else if (backoff < 26u) {
			Sleep( 1u );
		} else {
			Sleep( 10u );
		}
	}

// ---------------------------------------------------

	void FiberJobSystem::SetCallerCoreAffinity( ProcessorAffinity affinity ) const {
		SetThreadIdealProcessor( GetCurrentThread(), static_cast<DWORD>(affinity) );
	}

// ---------------------------------------------------

	void FiberJobSystem::SetCallerExecutionPriority( ThreadExecutionPriority priority ) {
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

	size_t FiberJobSystem::GetMaximumParallelism() const {
		return _workers.GetSize();
	}

}	// namespace Win32
}	// namespace Scheduling
}	// namespace Eldritch2