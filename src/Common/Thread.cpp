/*==================================================================*\
  Thread.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Thread.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

Thread::Thread(const Thread& /*unused*/) ETNoexceptHint : Thread() {}

// ---------------------------------------------------

Thread::Thread() ETNoexceptHint : _state(ExecutionState::Uninitialized) {}

// ---------------------------------------------------

Thread::~Thread() {
	ET_ASSERT(GetState() != ExecutionState::Running, "Destroying thread before it has completed!");
}

// ---------------------------------------------------

void Thread::AwaitCompletion() {
	SetShouldShutDown();
	while (GetState() == ExecutionState::Running) {
		//	Busy loop waiting for join.
	}
}

} // namespace Eldritch2
