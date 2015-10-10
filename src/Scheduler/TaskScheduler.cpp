/*==================================================================*\
  TaskScheduler.cpp
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/TaskScheduler.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scheduler {

	void TaskScheduler::Backoff( BackoffContext& /*context*/ ) {}

// ---------------------------------------------------

	void TaskScheduler::SetThreadPriorityForCaller( ThreadPriority /*priority*/ ) {}

}	// namespace Scheduler
}	// namespace Eldritch2