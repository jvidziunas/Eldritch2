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
#include <Utility/Concurrency/WaitableUserEvent.hpp>
#include <Utility/Concurrency/UserSemaphore.hpp>
#include <Scheduler/Win32/FiberScheduler.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <System/SystemInterface.hpp>
#include <Utility/Result.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <Windows.h>
#include <Objbase.h>
#include <Process.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace {

	static unsigned int ETStdCall ThreadEntryPoint( void* thread ) {
#pragma pack( push, 8 )
		// Taken from http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
		struct THREADNAME_INFO {
			::DWORD		dwType;		//!<	Must be 0x1000.
			::LPCSTR	szName;		//!<	Pointer to name (in user address space).
			::DWORD		dwThreadID;	//!<	Thread ID (-1 = caller thread).
			::DWORD		dwFlags;	//!<	Reserved for future use, must be 0.
		};
#pragma pack( pop )

		enum {
			MS_VC_EXCEPTION = 0x406D1388
		};

	// ---

		::MicroProfileOnThreadCreate( static_cast<Thread*>(thread)->GetHumanReadableName() );

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
		/*	MSVC warns about the SEH code used here to name a thread in the debugger.
			This is safe and is taken from official documentation, so disable the warnings here.
		*/
#	pragma warning( disable : 6312 )
#	pragma warning( disable : 6322 )
#endif
		if( 0 != ::IsDebuggerPresent() ) {
			THREADNAME_INFO	info = {
				0x1000,
				static_cast<Thread*>(thread)->GetHumanReadableName(),
				static_cast<::DWORD>(-1),
				0u,
			};

			__try {
				::RaiseException( MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(::ULONG_PTR), reinterpret_cast<::ULONG_PTR*>(&info) );
			} __except( EXCEPTION_CONTINUE_EXECUTION ) {}
		}
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif

		ETRuntimeVerificationWithMsg( SUCCEEDED( ::CoInitializeEx( nullptr, 0 ) ), "Error in ::CoInitializeEx()!" );

		static_cast<Thread*>(thread)->SchedulerEntryPoint();

		::CoUninitialize();

		::MicroProfileOnThreadExit();
		return 0u;
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Scheduler {
namespace Win32 {

	FiberScheduler::FiberScheduler( const System::SystemInterface& systemInterface, ::Eldritch2::Allocator& allocator ) : _allocator( allocator, UTF8L("Scheduler Root Allocator") ),
																														  _workerThreads( { _allocator, UTF8L("Scheduler Worker Thread Collection Allocator") } ),
																														  _syncObjectAlignmentInBytes( systemInterface.GetL0CacheLineSizeInBytes() ) {
		::MicroProfileInit();
	}

// ---------------------------------------------------

	FiberScheduler::~FiberScheduler() {
		for( auto& worker : _workerThreads ) {
			worker.EnsureTerminated();
		}

		_workerThreads.Clear();

		::MicroProfileShutdown();
	}

// ---------------------------------------------------

	ErrorCode FiberScheduler::Enqueue( Scheduler::Thread& thread ) {
		// Create a new operating system thread.
		if( const auto threadHandle = ::_beginthreadex( nullptr, 0, &ThreadEntryPoint, &thread, 0, nullptr ) ) {
			for( BackoffContext backoffContext( 0u ); !thread.HasStartedExecution(); ) {
				// Wait until the thread signals it's copied the information safely.
				Backoff( backoffContext );
			}

			return Error::None;
		}

		// We received an invalid handle, let's try to figure out why
		switch( errno ) {
			case EINVAL:	{ return Error::InvalidParameter; break; }	// case EINVAL
			case EACCES:	{ return Error::OutOfMemory; break; }		// case EACCES
		}	// switch( errno )

		return Error::Unspecified;
	}

// ---------------------------------------------------

	void FiberScheduler::Bootstrap( size_t totalWorkerCount, const WorkerContext::WorkItem& initialTask ) {
		totalWorkerCount = Max<size_t>( totalWorkerCount, 1u );

		// Make sure we aren't trying to re-bootstrap after the initial launch.
		if( _workerThreads ) {
			return;
		}

		_workerThreads.Reserve( totalWorkerCount );

		WorkerContext::FinishCounter	unused( 0 );

		// Construct the main thread, and queue the first task.
		_workerThreads.EmplaceBack( *this, _allocator );
		_workerThreads.Back().Enqueue( unused, initialTask );

		// Attach all worker threads that *aren't* run on the main thread...
		for( size_t worker( 0u ); worker < (totalWorkerCount - 1); ++worker ) {
			_workerThreads.EmplaceBack( *this, _allocator );

			// ... and get them started right away.
			if( !Enqueue( _workerThreads.Back() ) ) {
				return;
			}
		}

		// ... and finally kick off the initial task!
		ThreadEntryPoint( &(_workerThreads.Front()) );
	}

// ---------------------------------------------------

	void FiberScheduler::FlagForShutdown() {
		for( WorkerThread& workerThread : _workerThreads ) {
			workerThread.RequestGracefulShutdown();
		}
	}

// ---------------------------------------------------

	void FiberScheduler::Backoff( BackoffContext& context ) {
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

	void FiberScheduler::SetIdealProcessorAffinityForCaller( const ProcessorAffinity affinity ) const {
		::SetThreadIdealProcessor( ::GetCurrentThread(), static_cast<::DWORD>(affinity) );
	}

// ---------------------------------------------------

	void FiberScheduler::SetThreadPriorityForCaller( const ThreadPriority priority ) {
		static const int threadPriorityTable[] = {
			THREAD_PRIORITY_BELOW_NORMAL,
			THREAD_PRIORITY_NORMAL,
			THREAD_PRIORITY_ABOVE_NORMAL,
			THREAD_PRIORITY_TIME_CRITICAL
		};

		static_assert(_countof(threadPriorityTable) == static_cast<size_t>(ThreadPriority::COUNT), "Thread priority table needs");

	// ---

		::SetThreadPriority( ::GetCurrentThread(), threadPriorityTable[static_cast<size_t>(priority)] );
	}

// ---------------------------------------------------

	Result<Utility::ReaderWriterUserMutex> FiberScheduler::AllocateReaderWriterUserMutex( Allocator& allocator ) {
		class UserMutex : public ReaderWriterUserMutex {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref UserMutex instance.
			ETForceInlineHint UserMutex() {
				::InitializeSRWLock( &_lock );
			}

			~UserMutex() = default;

		// ---------------------------------------------------

			void Enter() override sealed {
				if( const auto worker = WorkerThread::GetActiveWorkerThread() ) {
					while( !TryEnter() ) {
						// Yield current thread.
					}
				} else {
					::AcquireSRWLockExclusive( &_lock );
				}
			}

			void Leave() override sealed {
				::ReleaseSRWLockExclusive( &_lock );
			}

			bool TryEnter() override sealed {
				return !!::TryAcquireSRWLockExclusive( &_lock );
			}

		// ---------------------------------------------------

			void EnterAsReader() override sealed {
				if( const auto worker = WorkerThread::GetActiveWorkerThread() ) {
					while( !TryEnterAsReader() ) {
						// Yield current thread.
					}
				} else {
					::AcquireSRWLockShared( &_lock );
				}
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

		if( auto mutex = new(allocator, _syncObjectAlignmentInBytes, Allocator::AllocationDuration::Normal) UserMutex ) {
			return { ::std::move( mutex ) };
		}

		return { Error::OutOfMemory };
	}

// ---------------------------------------------------

	Result<UserSemaphore> FiberScheduler::AllocateSemaphore( Allocator& allocator, const int initialCount, const int maximumCount ) {
		class Semaphore : public UserSemaphore {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			ETInlineHint Semaphore( const ::HANDLE semaphoreHandle ) : _semaphoreHandle( semaphoreHandle ) {}

			~Semaphore() {
				::CloseHandle( _semaphoreHandle );
			}

		// ---------------------------------------------------

			int IncreaseCount( const int count ) override sealed {
				::LONG	previousCount( 0 );

				::ReleaseSemaphore( _semaphoreHandle, count, &previousCount );

				return static_cast<int>( previousCount );
			}

		// ---------------------------------------------------

			void Acquire() override sealed {
				if( const auto worker = WorkerThread::GetActiveWorkerThread() ) {
					while( !TryAcquire() ) {

					}
				} else {
					::WaitForSingleObject( _semaphoreHandle, INFINITE );
				}
			}

			bool TryAcquire() override sealed {
				// Timeout interval of 0 will cause the function to return immediately.
				return WAIT_OBJECT_0 == ::WaitForSingleObject( _semaphoreHandle, 0 );
			}

		// - DATA MEMBERS ------------------------------------

		private:
			const ::HANDLE	_semaphoreHandle;
		};

	// ---

		if( const auto semaphore = ::CreateSemaphoreW( nullptr, initialCount, maximumCount, nullptr ) ) {
			if( auto result = new(allocator, _syncObjectAlignmentInBytes, Allocator::AllocationDuration::Normal) Semaphore( semaphore ) ) {
				return { ::std::move( result ) };
			}

			::CloseHandle( semaphore );

			return { Error::OutOfMemory };
		}
		
		return { Error::InvalidParameter };
	}

// ---------------------------------------------------

	Result<Utility::UserMutex> FiberScheduler::AllocateUserMutex( Allocator& allocator ) {
		return AllocateReaderWriterUserMutex( allocator );
	}

// ---------------------------------------------------

	FiberScheduler::WorkerThread& FiberScheduler::GetRandomWorkerThread( const WorkerThread& executingWorker, size_t& randomSeed ) {
		auto	FastRand( [] ( size_t& seed ) -> size_t {
			enum : size_t {
				Shift	= static_cast<size_t>(sizeof(size_t) == sizeof(uint64) ? 32u : 16u),
				Mask	= static_cast<size_t>(sizeof(size_t) == sizeof(uint64) ? 0x7FFFFFFF : 0x7FFF)
			};

			seed = (214013u * seed + 2531011u);
			return (seed >> Shift) & Mask;
		} );

		size_t	workerIndex( FastRand( randomSeed ) % _workerThreads.Size() );

		// Ensure we don't pick the calling worker for theft.
		if( &_workerThreads[workerIndex] == &executingWorker ) {
			workerIndex = (workerIndex + 1) % _workerThreads.Size();
		}

		return _workerThreads[workerIndex];
	}

// ---------------------------------------------------

	size_t FiberScheduler::GetMaximumTaskParallelism() const {
		// TODO: It might be prudent to make this a constant initialized at construction time.
		return static_cast<size_t>(_workerThreads.Size());
	}

}	// namespace Win32
}	// namespace Scheduler
}	// namespace Eldritch2

