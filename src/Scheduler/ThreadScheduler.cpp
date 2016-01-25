/*==================================================================*\
  ThreadScheduler.cpp
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/ThreadScheduler.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scheduler {

	void ThreadScheduler::Backoff( BackoffContext& /*context*/ ) {}

// ---------------------------------------------------

	void ThreadScheduler::SetThreadPriorityForCaller( ThreadPriority /*priority*/ ) {}

}	// namespace Scheduler
}	// namespace Eldritch2