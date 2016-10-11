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
#include <Scheduling/ThreadScheduler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {

	void ThreadScheduler::Backoff( BackoffContext& /*context*/ ) {}

// ---------------------------------------------------

	void ThreadScheduler::SetCallerExecutionPriority( ThreadExecutionPriority /*priority*/ ) {}

}	// namespace Scheduling
}	// namespace Eldritch2