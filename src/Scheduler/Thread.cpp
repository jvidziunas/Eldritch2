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
#include <Scheduler/Thread.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Scheduler {

	Thread::Thread() : _resultCode( Error::NONE ), _executionState( ExecutionState::UNINITIALIZED ) {}

// ---------------------------------------------------

	Thread::~Thread() {
		ETRuntimeAssert( ExecutionState::RUNNING != _executionState.load( memory_order_acquire ) );
	}

// ---------------------------------------------------

	bool Thread::HasCompleted() const {
		return ExecutionState::DONE == _executionState.load( memory_order_acquire );
	}

// ---------------------------------------------------

	void Thread::EnsureTerminated() {
		RequestGracefulShutdown();

		while( ExecutionState::RUNNING == _executionState.load( memory_order_relaxed ) ) {}
	}

// ---------------------------------------------------

	bool Thread::HasStartedExecution() const {
		return ExecutionState::UNINITIALIZED != _executionState.load( memory_order_consume );
	}

// ---------------------------------------------------

	void Thread::SchedulerEntryPoint() {
		auto	expectedState( ExecutionState::UNINITIALIZED );

		if( _executionState.compare_exchange_strong( expectedState, ExecutionState::RUNNING, memory_order_acquire ) ) {
			_resultCode = Run();
			_executionState.store( ExecutionState::DONE, memory_order_release );
		}	
	}

}	// namespace Scheduler
}	// namespace Eldritch2