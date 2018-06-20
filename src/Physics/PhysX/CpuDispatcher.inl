/*==================================================================*\
  CpuDispatcher.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	ETNeverThrowsHint ETInlineHint const Scheduling::JobFence& CpuDispatcher::GetTasksCompletedFence() const {
		return _tasksCompleted;
	}

}}} // namespace Eldritch2::Physics::PhysX
