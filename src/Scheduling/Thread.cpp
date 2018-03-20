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
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {

	Thread::Thread( const Thread& /*unused*/ ) : Thread() {}

// ---------------------------------------------------

	Thread::Thread() : _state( ExecutionState::Uninitialized ) {}

// ---------------------------------------------------

	Thread::~Thread() {
		ET_ASSERT( _state.load( std::memory_order_consume ) != ExecutionState::Running, "Destroying thread before it has completed!" );
	}

// ---------------------------------------------------

	bool Thread::IsRunning( std::memory_order order ) const {
		return _state.load( order ) == ExecutionState::Running;
	}

// ---------------------------------------------------

	void Thread::AwaitCompletion() {
		SetShouldShutDown();

		while (IsRunning( std::memory_order_consume )) {
		//	Busy loop waiting for join.
		}
	}

// ---------------------------------------------------

	bool Thread::HasStarted( std::memory_order order ) const {
		return _state.load( order ) != ExecutionState::Uninitialized;
	}

// ---------------------------------------------------

	void Thread::Enter() {
#	if ET_DEBUG_BUILD
		const ExecutionState previous( _state.exchange( ExecutionState::Running, std::memory_order_acq_rel ) );
		ET_ASSERT( previous == ExecutionState::Uninitialized, "Duplicate thread start operation!" );
#	else
		_state.store( ExecutionState::Running, std::memory_order_release );
#	endif

		Run();

		_state.store( ExecutionState::Done, std::memory_order_release );
	}

}	// namespace Scheduling
}	// namespace Eldritch2