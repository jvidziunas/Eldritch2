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

namespace Eldritch2 {
namespace Scheduler {

	Thread::Thread() : _executionState( ExecutionState::Uninitialized ) {}

// ---------------------------------------------------

	Thread::~Thread() {
		ETRuntimeAssert( ExecutionState::Running != _executionState.load( ::std::memory_order_consume ) );
	}

// ---------------------------------------------------

	bool Thread::HasCompleted() const {
		return ExecutionState::Done == _executionState.load( ::std::memory_order_consume );
	}

// ---------------------------------------------------

	void Thread::EnsureTerminated() {
		RequestGracefulShutdown();

		while( ExecutionState::Running == _executionState.load( ::std::memory_order_consume ) ) {}
	}

// ---------------------------------------------------

	bool Thread::HasStartedExecution() const {
		return ExecutionState::Uninitialized != _executionState.load( ::std::memory_order_consume );
	}

// ---------------------------------------------------

	void Thread::SchedulerEntryPoint() {
		auto	expectedState( ExecutionState::Uninitialized );

		if( _executionState.compare_exchange_strong( expectedState, ExecutionState::Running, ::std::memory_order_acq_rel ) ) {
			Run();
			_executionState.store( ExecutionState::Done, ::std::memory_order_release );
		}	
	}

}	// namespace Scheduler
}	// namespace Eldritch2