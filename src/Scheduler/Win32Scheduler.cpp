/*==================================================================*\
  Win32Scheduler.cpp
  ------------------------------------------------------------------
  Purpose:
  Provides a scheduler implementation policy that wraps Windows
  specific implementations of job queuing functionalities.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Concurrency/WaitableUserEvent.hpp>
#include <Utility/Concurrency/UserSemaphore.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <System/Win32SystemInterface.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Scheduler/Win32Scheduler.hpp>
#include <Utility/Memory/MemStdLib.hpp>
#include <Utility/ResultPair.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <Objbase.h>
#include <Process.h>
//------------------------------------------------------------------//
#include <algorithm>
#include <cerrno>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace {

	enum {
		MS_VC_EXCEPTION = 0x406D1388
	};

// ---------------------------------------------------

	static unsigned int ETStdCall ThreadEntryPoint( void* thread ) {
	// Taken from http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
#pragma pack( push, 8 )
	struct THREADNAME_INFO {
		::DWORD		dwType;		// Must be 0x1000.
		::LPCSTR	szName;		// Pointer to name (in user address space).
		::DWORD		dwThreadID;	// Thread ID (-1 = caller thread).
		::DWORD		dwFlags;	// Reserved for future use, must be zero.
	};
#pragma pack( pop )

	// ---

		THREADNAME_INFO	info = {
			0x1000,
			static_cast<Thread*>(thread)->GetHumanReadableName(),
			static_cast<::DWORD>(-1),
			0u,
		};

		::MicroProfileOnThreadCreate( static_cast<Thread*>(thread)->GetHumanReadableName() );

		if( 0 != ::IsDebuggerPresent() ) {
			__try {
				::RaiseException( MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(::ULONG_PTR), reinterpret_cast<::ULONG_PTR*>(&info) );
			} __except( EXCEPTION_CONTINUE_EXECUTION ) {}
		}

		{
			const auto	initializeResult( ::CoInitializeEx( nullptr, 0 ) );
			ETRuntimeVerificationWithMsg( SUCCEEDED( initializeResult ), "Error in CoInitializeEx()!" );
		}

		static_cast<Thread*>(thread)->SchedulerEntryPoint();
		::MicroProfileOnThreadExit();

		::CoUninitialize();

		return 0u;
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Scheduler {

	Win32Scheduler::Win32Scheduler( const Win32SystemInterface& systemInterface, Allocator& allocator ) : _allocator( allocator, UTF8L("Win32 Scheduler Allocator") ),
																										  _workerThreads( nullptr, nullptr ),
																										  _systemCacheLineSizeInBytes( systemInterface.GetL0CacheLineSizeInBytes() ) {
		::MicroProfileInit();
	}

// ---------------------------------------------------

	Win32Scheduler::~Win32Scheduler() {
		for( WorkerThread& worker : _workerThreads ) {
			worker.~WorkerThread();
		}

		_allocator.Deallocate( _workerThreads.first, AlignedDeallocationSemantics );
		::MicroProfileShutdown();
	}

// ---------------------------------------------------

	ErrorCode Win32Scheduler::Bootstrap( Task& initialTask, size_t totalWorkerCount ) {
		ErrorCode	result( Errors::NONE );

		// Make sure we aren't trying to re-bootstrap after the initial launch.
		if( _workerThreads ) {
			return Errors::INVALID_OBJECT_STATE;
		}

		totalWorkerCount = Max<size_t>( totalWorkerCount, 1u );

		// Allocate a big array of worker thread objects.
		if( auto* const workerThreadMemory = _allocator.Allocate( totalWorkerCount * sizeof(WorkerThread), _systemCacheLineSizeInBytes, Allocator::AllocationOption::PERMANENT_ALLOCATION ) ) {
			_workerThreads = Range<WorkerThread*>( static_cast<WorkerThread*>(workerThreadMemory), totalWorkerCount );

			for( WorkerThread& workerThread : _workerThreads ) {
				new(&workerThread) Win32Scheduler::WorkerThread( *this );
			}

			// Queue the bootstrap task to the primary thread.
			initialTask.TrySchedulingOnContext( *_workerThreads.first );

			// Attach all worker threads that *aren't* run on the main thread...
			for( WorkerThread& thread : Range<WorkerThread*>( _workerThreads.first + 1, _workerThreads.onePastLast ) ) {
				result = Enqueue( thread );

				if( !result ) {
					break;
				}
			}

			if( result ) {
				// ... and finally kick off the initial task!
				result	= ConvertCallerToThread( *_workerThreads.first );
			}
		} else {
			result = Errors::OUT_OF_MEMORY;
		}

		return result;
	}

// ---------------------------------------------------

	void Win32Scheduler::FlagForShutdown() {
		for( WorkerThread& workerThread : _workerThreads ) {
			workerThread.RequestGracefulShutdown();
		}
	}

// ---------------------------------------------------

	void Win32Scheduler::Backoff( TaskScheduler::BackoffContext& context ) {
		const BackoffContext	backoff( context );

		if( backoff < 10u ) {
			::_mm_pause();
		} else if( backoff < 20u ) {
			for( int i( 0u ); i != 25u; ++i ) {
				::_mm_pause(); ::_mm_pause();
			}
		} else if( backoff < 22u ) {
			::SwitchToThread();
		} else if( backoff < 24u ) {
			::Sleep( 0u );
		} else if( backoff < 26u ) {
			::Sleep( 1u );
		} else {
			::Sleep( 10u );
		}

		 ++context;
	}

// ---------------------------------------------------

	void Win32Scheduler::Sleep( const uint64 timeInMicroseconds ) {
		::Sleep( static_cast<::DWORD>(timeInMicroseconds / 1000u) );
	}

// ---------------------------------------------------

	void Win32Scheduler::SetThreadPriorityForCaller( const ThreadPriority priority ) {
		static const int threadPriorityTable[ThreadPriority::PRIORITY_COUNT] = {
			THREAD_PRIORITY_BELOW_NORMAL,
			THREAD_PRIORITY_NORMAL,
			THREAD_PRIORITY_ABOVE_NORMAL,
			THREAD_PRIORITY_TIME_CRITICAL
		};

		::SetThreadPriority( ::GetCurrentThread(), threadPriorityTable[static_cast<size_t>(priority)] );
	}

// ---------------------------------------------------

	void Win32Scheduler::SetIdealProcessorAffinityForCaller( const ProcessorAffinity affinity ) const {
		::SetThreadIdealProcessor( ::GetCurrentThread(), static_cast<::DWORD>(affinity) );
	}

// ---------------------------------------------------

	ResultPair<UserMutex> Win32Scheduler::AllocateUserMutex( Allocator& allocator ) {
		const auto	tempResult( AllocateReaderWriterUserMutex( allocator ) );

		return { static_cast<UserMutex*>(tempResult.object), tempResult.resultCode };
	}

// ---------------------------------------------------

	ResultPair<ReaderWriterUserMutex> Win32Scheduler::AllocateReaderWriterUserMutex( Allocator& allocator ) {
		class UserMutex : public ReaderWriterUserMutex {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this UserMutex instance.
			ETForceInlineHint UserMutex() {
				::InitializeSRWLock( &_lock );
			}

			// Destroys this UserMutex instance.
			ETForceInlineHint ~UserMutex() {}

		// ---------------------------------------------------

			void Enter() override sealed {
				::AcquireSRWLockExclusive( &_lock );
			}

			void Leave() override sealed {
				::ReleaseSRWLockExclusive( &_lock );
			}

			bool TryEnter() override sealed {
				return !!::TryAcquireSRWLockExclusive( &_lock );
			}

		// ---------------------------------------------------

			void EnterAsReader() override sealed {
				::AcquireSRWLockShared( &_lock );
			}

			void LeaveAsReader() override sealed {
				::ReleaseSRWLockShared( &_lock );
			}

			bool TryEnterAsReader() override sealed {
				return !!::TryAcquireSRWLockShared( &_lock );
			}

		// - DATA MEMBERS ------------------------------------

		private:
			::SRWLOCK	_lock;
		};

	// ---

		auto* const	mutex( new(allocator, _systemCacheLineSizeInBytes, Allocator::AllocationOption::PERMANENT_ALLOCATION) UserMutex() );

		return { mutex, mutex ? Errors::NONE : Errors::OUT_OF_MEMORY };
	}

// ---------------------------------------------------

	ResultPair<WaitableUserEvent> Win32Scheduler::AllocateWaitableEvent( Allocator& allocator, const EventInitialState initialState ) {
		class Event : public WaitableUserEvent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this Event instance.
			ETForceInlineHint Event( const ::HANDLE event ) : _event( event ) {}

			// Destroys this Event instance.
			~Event() {
				::CloseHandle( _event );
			}

		// ---------------------------------------------------

			void BlockUntilSignaled() override sealed {
				::MsgWaitForMultipleObjects( 1u, &_event, FALSE, INFINITE, QS_ALLEVENTS );
			}

			void Signal() override sealed {
				::SetEvent( _event );
			}

			void Reset() override sealed {
				::ResetEvent( _event );
			}

		// - DATA MEMBERS ------------------------------------

		private:
			const ::HANDLE	_event;
		};

	// ---

		if( const auto eventHandle = ::CreateEventW( nullptr, FALSE, (EventInitialState::UNSIGNALED == initialState ? FALSE : TRUE), nullptr ) ) {
			auto* const	event( new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) Event( eventHandle ) );
			if( !event ) {
				::CloseHandle( eventHandle );
			}

			return { event, event ? Errors::NONE : Errors::OUT_OF_MEMORY };
		}

		return { nullptr, Errors::UNSPECIFIED };
	}

// ---------------------------------------------------

	ResultPair<UserSemaphore> Win32Scheduler::AllocateSemaphore( ::Eldritch2::Allocator& allocator, const size_t initialCount, const size_t maximumCount ) {
		class Semaphore : public UserSemaphore {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this Semaphore instance.
			ETForceInlineHint Semaphore( const ::HANDLE semaphore ) : _semaphore( semaphore ) {}

			// Destroys this Semaphore instance.
			~Semaphore() {
				::CloseHandle( _semaphore );
			}

		// ---------------------------------------------------

			size_t IncreaseCount( const size_t count ) override sealed {
				::LONG	previousCount( 0u );

				::ReleaseSemaphore( _semaphore, static_cast<::LONG>(count), &previousCount );
				return static_cast<size_t>(previousCount);
			}

			void Acquire() override sealed {
				::MsgWaitForMultipleObjects( 1u, &_semaphore, FALSE, INFINITE, QS_ALLEVENTS );
			}

		// - DATA MEMBERS ------------------------------------

		private:
			const ::HANDLE	_semaphore;
		};

	// ---

		if( const auto semaphoreHandle = ::CreateSemaphore( nullptr, static_cast<::LONG>(initialCount), static_cast<::LONG>(maximumCount), nullptr ) ) {
			auto* const	semaphore( new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) Semaphore( semaphoreHandle ) );

			if( semaphore ) {
				return { semaphore, Errors::NONE };
			} else {
				::CloseHandle( semaphoreHandle );
			}
		}

		return { nullptr, Errors::UNSPECIFIED };
	}

// ---------------------------------------------------

	ErrorCode Win32Scheduler::Enqueue( Thread& thread ) {
		// Create a new operating system thread.
		if( const auto threadHandle = ::_beginthreadex( nullptr, 0, &ThreadEntryPoint, &thread, 0, nullptr ) ) {
			for( BackoffContext backoffContext( 0u ); !thread.HasStartedExecution(); ) {
				// Wait until the thread signals it's copied the information safely
				this->Backoff( backoffContext );
			}

			return Errors::NONE;
		}

		// We received an invalid handle, let's try to figure out why
		switch( errno ) {
			case EINVAL:	{ return Errors::INVALID_PARAMETER; break; }	// case EINVAL
			case EACCES:	{ return Errors::OUT_OF_MEMORY; break; }	// case EACCES
			default:		{ return Errors::UNSPECIFIED; break; }	// default case
		}	// switch( errno )
	}

// ---------------------------------------------------

	Win32Scheduler::WorkerThread& Win32Scheduler::GetRandomWorkerThread( const WorkerThread& executingWorker, size_t& randomSeed ) {
		struct FastRandHelper {
			static ETNoAliasHint ETForceInlineHint size_t FastRand( size_t& seed ) {
				enum : size_t {
					SHIFT = static_cast<size_t>(sizeof( size_t ) == sizeof( uint64 ) ? 32 : 16),
					MASK = static_cast<size_t>(sizeof( size_t ) == sizeof( uint64 ) ? 0x7FFFFFFF : 0x7FFF)
				};

				seed = (214013 * seed + 2531011);
				return (seed >> SHIFT) & MASK;
			}
		};

		size_t	workerIndex( FastRandHelper::FastRand( randomSeed ) % _workerThreads.Size() );

		if( _workerThreads.first + workerIndex == &executingWorker ) {
			workerIndex = (workerIndex + 1) % _workerThreads.Size();
		}

		return _workerThreads[workerIndex];
	}

// ---------------------------------------------------

	size_t Win32Scheduler::GetMaximumTaskParallelism() const {
		return static_cast<size_t>(_workerThreads.Size());
	}

// ---------------------------------------------------

	ErrorCode Win32Scheduler::ConvertCallerToThread( Thread& thread ) {
		ThreadEntryPoint( &thread );

		return Errors::NONE;
	}

}	// namespace Scheduler
}	// namespace Eldritch2