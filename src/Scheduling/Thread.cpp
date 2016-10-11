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
#include <Scheduling/Thread.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {

	Thread::Thread( const Thread& /*unused*/ ) : Thread() {}

// ---------------------------------------------------

	Thread::Thread() : _executionState( ExecutionState::Uninitialized ) {}

// ---------------------------------------------------

	Thread::~Thread() {
		ETRuntimeAssert( ExecutionState::Running != _executionState.load( std::memory_order_consume ) );
	}

// ---------------------------------------------------

	bool Thread::HasCompleted() const {
		return ExecutionState::Done == _executionState.load( std::memory_order_consume );
	}

// ---------------------------------------------------

	void Thread::AwaitCompletion() {
		RequestGracefulShutdown();

		while( ExecutionState::Running == _executionState.load( std::memory_order_consume ) ) {
		//	Busy loop.
		}
	}

// ---------------------------------------------------

	bool Thread::HasStartedExecution() const {
		return ExecutionState::Uninitialized != _executionState.load( std::memory_order_consume );
	}

// ---------------------------------------------------

	void Thread::SchedulerEntryPoint() {
#	if ET_DEBUG_MODE_ENABLED
		const auto	previousState( _executionState.exchange( ExecutionState::Running, std::memory_order_acq_rel ) );
		ETRuntimeAssert( ExecutionState::Uninitialized == previousState );
#	else
		_executionState.store( ExecutionState::Running, std::memory_order_release );
#	endif

		Run();

		_executionState.store( ExecutionState::Done, std::memory_order_release );
	}

}	// namespace Scheduling
}	// namespace Eldritch2