/*==================================================================*\
  ThreadScheduler.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/WorkerContext.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduler {
		class	Thread;
	}

	namespace Utility {
		class	ReaderWriterUserMutex;
		class	WaitableUserEvent;
		class	UserSemaphore;
		class	UserMutex;
	}

	class	Allocator;
	class	ErrorCode;
}

namespace Eldritch2 {
namespace Scheduler {

	typedef ::Eldritch2::uintptr	BackoffContext;

// ---------------------------------------------------

	class ETPureAbstractHint ThreadScheduler : private Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------

	public:
		//!	Execution priority for a thread. 
		enum class ThreadPriority {
			BelowNormal,
			Normal,
			AboveNormal,
			Priority,

			COUNT
		};

	// ---

		enum class EventInitialState {
			Unsignaled,
			Signaled
		};

	// ---

		//!	Minimal state used for incremental backoff.
		using BackoffContext	= Scheduler::BackoffContext;

	// - WORK SCHEDULING ---------------------------------

		// Attempts to assign exclusive ownership of the denoted @ref Module instance to this @ref ThreadScheduler.
		// If the operation succeeds, the associated code for the @ref Module will be executed continually
		// on a unique operating system thread until it is either dequeued from the @ref ThreadScheduler or
		// manually told to cease execution.
		virtual ::Eldritch2::ErrorCode	Enqueue( Scheduler::Thread& thread ) abstract;

	// ---------------------------------------------------

		virtual void	Bootstrap( size_t totalWorkerCount, const Scheduler::WorkerContext::WorkItem& initialTask );

		virtual void	FlagForShutdown() abstract;

	// ---------------------------------------------------

		virtual void	Backoff( BackoffContext& context );

	// ---------------------------------------------------

		virtual size_t	GetMaximumTaskParallelism() const abstract;

	// ---------------------------------------------------

		virtual void	SetThreadPriorityForCaller( const ThreadPriority priority );

	// - SYNCHRONIZATION OBJECT ALLOCATION ---------------

		//!	Instructs the @ref ThreadScheduler to allocate a read/write lock that allows concurrent read access, but enforces mutually-exclusive writes (also ensuring no readers are currently accessing the protected resource)
		virtual ::Eldritch2::Result<Utility::ReaderWriterUserMutex>	AllocateReaderWriterUserMutex( ::Eldritch2::Allocator& allocator ) abstract;

		//!	Instructs the @ref ThreadScheduler to allocate a semaphore object useful for limiting the degree of concurrent access to a shared resource.
		virtual ::Eldritch2::Result<Utility::UserSemaphore>			AllocateSemaphore( ::Eldritch2::Allocator& allocator, const int initialCount, const int maximumCount ) abstract;

		//!	Instructs the @ref ThreadScheduler to allocate a basic mutually-exclusive lock.
		virtual ::Eldritch2::Result<Utility::UserMutex>				AllocateUserMutex( ::Eldritch2::Allocator& allocator ) abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref ThreadScheduler instance.
		ThreadScheduler() = default;

		//!	Destroys this @ref ThreadScheduler instance.
		~ThreadScheduler() = default;
	};

}	// namespace Scheduler
}	// namespace Eldritch2