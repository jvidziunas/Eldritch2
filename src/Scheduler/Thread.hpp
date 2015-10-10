/*==================================================================*\
  Thread.hpp
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
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduler {
		class	TaskScheduler;
	}
}

namespace Eldritch2 {
namespace Scheduler {

	namespace Scheduler	= ::Eldritch2::Scheduler;

// ---------------------------------------------------

	class ETPureAbstractHint Thread : private Utility::Noncopyable {
	public:
		//! Retrieves a pointer to a null-terminated character array containing the UTF8-encoded name of this thread.
		/*! @remark This pointer should not change over the lifetime of the Thread instance, and implementing classes are highly encouraged to use a string literal for this purpose.
			*/
		virtual const ::Eldritch2::UTF8Char* const	GetHumanReadableName() const abstract;

	// ---------------------------------------------------

		//! Checks to see if this Thread instance has been scheduled on a concrete operating system thread.
		/*!	@see _executionState()
			*/
		bool	HasStartedExecution() const;

		//! Checks to see if this Thread instance is still scheduled on a concrete operating system thread.
		/*!	@see _executionState()
			*/
		bool	HasCompleted() const;

		//! Notifies this Thread instance that it should terminate, and blocks the calling thread until it completes doing so.
		/*!	@remark The turnaround time for a full termination is very much unbounded. Where possible, try to call Thread::RequestGracefulShutdown() ahead of time and then call this function at the last possible moment.
			@see RequestGracefulShutdown()
			@see HasCompleted()
			*/
		void	EnsureTerminated();

	// ---------------------------------------------------

		//! Notifies this Thread instance that the bound operating system thread should terminate.
		/*! @remark Implementations should be fully re-entrant, as this may be called from multiple threads, multiple times on the same thread, or both.
			*/
		virtual void	RequestGracefulShutdown() abstract;

		void			SchedulerEntryPoint();

	// ---------------------------------------------------

	protected:
		virtual ::Eldritch2::ErrorCode Run() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this Thread instance.
		Thread();

		// Destroys this Thread instance.
		~Thread();

	// ---------------------------------------------------

	private:
		enum ExecutionState : ::Eldritch2::uint32 {
			UNINITIALIZED,
			RUNNING,
			DONE
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ErrorCode			_resultCode;
		/** Marker used by the underlying operating system thread to indicate whether or not it has forked/joined. */
		::std::atomic<ExecutionState>	_executionState;
	};

}	// namespace Scheduler
}	// namespace Eldritch2