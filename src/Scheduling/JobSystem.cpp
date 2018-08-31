/*==================================================================*\
  JobSystem.cpp
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobSystem.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling {

	void JobSystem::BackOff(BackoffContext& /*context*/) ETNoexceptHint {}

	// ---------------------------------------------------

	void JobSystem::SetCallerExecutionPriority(ThreadExecutionPriority /*priority*/) {}

}} // namespace Eldritch2::Scheduling
