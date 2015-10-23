/*==================================================================*\
  Task.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduler {

	ETInlineHint bool Task::IsContinuedBy( const Scheduler::Task& task ) const {
		return &task == _continuationTask;
	}

// ---------------------------------------------------

	ETInlineHint bool Task::IsCodependentOn( const Scheduler::Task& task ) const {
		return &task == _codependentTask;
	}

}	// namespace Scheduler
}	// namespace Eldritch2