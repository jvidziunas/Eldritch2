/*==================================================================*\
  TaskScheduler.hpp
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduler {
		class	Thread;
		class	Task;
	}

	namespace Utility {
		class	ReaderWriterUserMutex;
		class	WaitableUserEvent;
		template <typename ResultObjectType>
		struct	ResultPair;
		class	UserSemaphore;
		class	UserMutex;
	}

	class	Allocator;
	class	ErrorCode;
}

namespace Eldritch2 {
namespace Scheduler {

	namespace Scheduler	= ::Eldritch2::Scheduler;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	typedef ::Eldritch2::uintptr	BackoffContext;

// ---------------------------------------------------

	class ETPureAbstractHint TaskScheduler : private Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------
	public:

		// Execution priority for a thread. 
		enum class ThreadPriority {
			BELOW_NORMAL,
			NORMAL,
			ABOVE_NORMAL,
			CRITICAL,

			PRIORITY_COUNT
		};

	// ---

		enum class EventInitialState {
			UNSIGNALED,
			SIGNALED
		};

	// ---

		// Minimal state used for incremental backoff.
		typedef Scheduler::BackoffContext	BackoffContext;

	// - WORK SCHEDULING ---------------------------------

		// Attempts to assign exclusive ownership of the denoted Module instance to this Scheduler.
		// If the operation succeeds, the associated code for the Module will be executed continually
		// on a unique operating system thread until it is either dequeued from the Scheduler or
		// manually told to cease execution.
		virtual ::Eldritch2::ErrorCode	Enqueue( Scheduler::Thread& thread ) abstract;

	// ---------------------------------------------------

		virtual ::Eldritch2::ErrorCode	Bootstrap( Scheduler::Task& initialTask, size_t totalWorkerCount ) abstract;

		virtual void					FlagForShutdown() abstract;

	// ---------------------------------------------------

		virtual void	Sleep( const ::Eldritch2::uint64 timeInMicroseconds ) abstract;

		virtual void	Backoff( BackoffContext& context );

	// ---------------------------------------------------

		virtual size_t	GetMaximumTaskParallelism() const abstract;

	// ---------------------------------------------------

		virtual void	SetThreadPriorityForCaller( const ThreadPriority priority );

	// - SYNCHRONIZATION OBJECT ALLOCATION/DEALLOCATION --

		//! Instructs the Scheduler to allocate a read/write lock that allows concurrent read access, but enforces mutually-exclusive writes (also ensuring no readers are currently accessing the protected resource)
		virtual Utility::ResultPair<Utility::ReaderWriterUserMutex>	AllocateReaderWriterUserMutex( ::Eldritch2::Allocator& allocator ) abstract;

		virtual Utility::ResultPair<Utility::WaitableUserEvent>		AllocateWaitableEvent( ::Eldritch2::Allocator& allocator, const EventInitialState initialState ) abstract;

		//! Instructs the Scheduler to allocate a semaphore object useful for limiting the degree of concurrent access to a shared resource.
		virtual Utility::ResultPair<Utility::UserSemaphore>			AllocateSemaphore( ::Eldritch2::Allocator& allocator, const size_t initialCount, const size_t maximumCount ) abstract;

		//! Instructs the Scheduler to allocate a basic mutually-exclusive lock.
		virtual Utility::ResultPair<Utility::UserMutex>				AllocateUserMutex( ::Eldritch2::Allocator& allocator ) abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref TaskScheduler instance.
		TaskScheduler() = default;

		//! Destroys this @ref TaskScheduler instance.
		~TaskScheduler() = default;
	};

}	// namespace Scheduler
}	// namespace Eldritch2