/*==================================================================*\
  Task.hpp
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
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduler {
		class	WorkerContext;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace Scheduler {

	namespace Scheduler	= ::Eldritch2::Scheduler;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint Task : public Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum class ExecutionState : ::Eldritch2::uint32 {
			UNSCHEDULED,	//!< The @ref Task has not been queued for execution on a @ref WorkerContext, nor have @ref Task::Execute() or @ref Task::Finalize() been invoked upon it.
			SCHEDULED,		//!< The @ref Task has been queued for execution on a @ref WorkerContext, but neither @ref Task::Execute() nor @ref Task::Finalize() have been invoked upon it.
			RUNNING,		//!< The @ref Task has been queued for execution on a @ref WorkerContext, and at least @ref Task::Execute() has been (initially) invoked upon it. 
			DONE			//!< The @ref Task has been queued for execution on a @ref WorkerContext, and both @ref Task::Execute() and @ref Task::Finalize() have both been called and returned.
		};

		struct ContinuationTaskSemantics {};
		struct CodependentTaskSemantics {};

	// - EXECUTION STATE TRACKING ------------------------

		//! Determines the scheduling/execution status of this @ref Task instance.
		/*!	@returns an @ref ExecutionState enum describing the current scheduling state for the @ref Task.
			@remarks Thread-safe.
			@see @ref ExecutionState
			*/
		ExecutionState	GetStatus() const;

		// Attempts to mark this @ref Task object as being scheduled for execution by a Scheduler
		// instance, returning true if the progress update was successful (i.e. the Task
		// transitioned to the 'scheduled' state from the 'uninitialized'/initial state) and
		// false if scheduling was somehow pre-empted or impossible.
		// Thread-safe.
		bool			TrySchedulingOnContext( Scheduler::WorkerContext& activeWorkerContext );

	// - TASK GRAPH INSPECTION ---------------------------

		//!	Gets a human-readable string containing a description of what the @ref Task does in its @ref Execute() and @ref Finalize() methods.
		/*!	@returns A null-terminated C string containing the friendly name of the @ref Task. This should not change over the course of program execution.
			@remarks Thread-safe.
			*/
		virtual const ::Eldritch2::UTF8Char* const	GetHumanReadableName() const abstract;

		//!	Checks the @ref Task to see if it can possibly initiate execution of the passed-in @ref Task instance.
		/*!	@param[in] task @ref Task instance to be checked for dependency.
			@returns @parblock _True_ if the @ref Task instance specified by _task_ cannot start until this @ref Task completes, or _false_ if
				_task_ may start before this @ref Task completes. @endparblock
			@remarks Thread-safe.
			@remarks Since task graphs are directed, this is not commutative.
			*/
		ETInlineHint bool							IsContinuedBy( const Scheduler::Task& task ) const;

		//!	Checks the @ref Task to see if it can possibly finish execution of the passed-in @ref Task instance.
		/*!	@param[in] task @ref Task instance to be checked for codependency.
			@returns @parblock _True_ if the @ref Task instance specified by _task_ cannot complete until this @ref Task completes, or _false_ if
				_task_ may complete before this @ref Task completes. @endparblock
			@remarks Thread-safe.
			@remarks Since task graphs are directed, this is not commutative.
			*/
		ETInlineHint bool							IsCodependentOn( const Scheduler::Task& task ) const;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref Task instance.
		/*!	@param[in] dependentTask @ref Task to (potentially) be started once the new @ref Task finishes execution.
			@param[in] codependentTask @ref Task that cannot finish until the new @ref Task has also completed.
			@remarks Codependency and dependency are mutually-exclusive states for task graphs. The parameters will be checked for this in debug mode.
			@remarks Designed to be called within subclasses only.
			*/
		Task( Scheduler::Task& continuationTask, Scheduler::Task& codependentTask );
		//!	Constructs this @ref Task instance.
		/*!	@param[in] codependentTask @ref Task that cannot finish until the new @ref Task has also completed.
			@remarks Designed to be called within subclasses only.
			*/
		Task( Scheduler::Task& continuationTask, const ContinuationTaskSemantics );
		//!	Constructs this @ref Task instance.
		/*!	@param[in] dependentTask @ref Task to (potentially) be started once the new @ref Task finishes execution.
			@remarks Designed to be called within subclasses only.
			*/
		Task( Scheduler::Task& codependentTask, const CodependentTaskSemantics );
		//!	Constructs this @ref Task instance.
		/*!	@remarks Designed to be called within subclasses only.
			*/
		Task();

	public:
		//!	Destroys this @ref Task instance.
		virtual ~Task() = default;

	// - TEMPLATE METHOD COMPONENTS ---------------------

	protected:
		// Derived classes should override this method in order to perform any
		// computations. This method may also optionally return a Task object pointer
		// that will be immediately scheduled (and executed, if possible) after the
		// method returns. If there is no additional continuation-style work, a null
		// pointer may be returned.
		// Not thread-safe.
		virtual Scheduler::Task*	Execute( Scheduler::WorkerContext& executingContext ) abstract;

		// Derived classes can override this method to perform any finalization or
		// other operations that require the task to have been run successfully. The
		// scheduler will invoke this method once on the Task once the end count reaches
		// zero.
		virtual void				Finalize( Scheduler::WorkerContext& executingContext );

	// - DATA MEMBERS ------------------------------------
		
	private:
		Scheduler::Task* const			_continuationTask;
		Scheduler::Task* const			_codependentTask;
		::std::atomic<size_t>			_endCount;
		::std::atomic<size_t>			_startCount;
		::std::atomic<ExecutionState>	_state;
	};

// ---------------------------------------------------

	extern const Task::ContinuationTaskSemantics	ContinuationTaskSemantics;
	extern const Task::CodependentTaskSemantics		CodependentTaskSemantics;

}	// namespace Scheduler
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scheduler/Task.inl>
//------------------------------------------------------------------//