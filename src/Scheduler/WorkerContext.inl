/*==================================================================*\
  WorkerContext.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduler {
		
	template <size_t workItemCount>
	ETInlineHint void WorkerContext::Enqueue( FinishCounter& finishCounter, const WorkItem (&workItems)[workItemCount] ) {
		Enqueue( finishCounter, workItems, workItems + workItemCount );
	}

}	// namespace Scheduler
}	// namespace Eldritch2