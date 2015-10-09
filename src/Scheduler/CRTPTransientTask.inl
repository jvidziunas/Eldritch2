/*==================================================================*\
  CRTPTransientTask.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduler {

	namespace Scheduler	= ::Eldritch2::Scheduler;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <typename ImplementingTask>
	ETForceInlineHint CRTPTransientTask<ImplementingTask>::CRTPTransientTask() : Scheduler::Task() {}

// ---------------------------------------------------

	template <typename ImplementingTask>
	ETForceInlineHint CRTPTransientTask<ImplementingTask>::CRTPTransientTask( Scheduler::Task& dependentTask, const ContinuationTaskSemantics semantics ) : Scheduler::Task( dependentTask, semantics ) {}

// ---------------------------------------------------

	template <typename ImplementingTask>
	ETForceInlineHint CRTPTransientTask<ImplementingTask>::CRTPTransientTask( Scheduler::Task& codependentTask, const CodependentTaskSemantics semantics ) : Scheduler::Task( codependentTask, semantics ) {}

// ---------------------------------------------------

	template <typename ImplementingTask>
	ETForceInlineHint CRTPTransientTask<ImplementingTask>::CRTPTransientTask( Scheduler::Task& dependentTask, Scheduler::Task& codependentTask ) : Scheduler::Task( dependentTask, codependentTask ) {}

// ---------------------------------------------------

	template <typename ImplementingTask>
	void CRTPTransientTask<ImplementingTask>::Finalize( Scheduler::WorkerContext& /*executingContext*/ ) {
		static_cast<ImplementingTask&>(*this).~ImplementingTask();
	}

}	// namespace Scheduler
}	// namespace Eldritch2