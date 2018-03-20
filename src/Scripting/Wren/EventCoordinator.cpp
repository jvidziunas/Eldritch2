/*==================================================================*\
  EventCoordinator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/EventCoordinator.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	EventCoordinator::ScriptEvent::~ScriptEvent() {
		ET_ASSERT( receiver == nullptr, "Leaking receiver for Wren script event!" );
	}

// ---------------------------------------------------

	void EventCoordinator::ScriptEvent::FreeResources( WrenVM* wren ) {
		wrenReleaseHandle( wren, eastl::exchange( receiver, nullptr ) );
	}

// ---------------------------------------------------

	bool EventCoordinator::ScriptEvent::Dispatch( WrenVM* wren, WrenHandle* callStub ) {
		wrenSetSlotHandle( wren, 0, receiver );
		return WREN_RESULT_SUCCESS == wrenCall( wren, callStub );
	}

// ---------------------------------------------------

	EventCoordinator::EventCoordinator() : _gameTime( 0u ), _isPaused( false ), _events( MallocAllocator( "Wren Game Event Queue Allocator" ) ), _dispatchStub( nullptr ) {}

// ---------------------------------------------------

	EventCoordinator::~EventCoordinator() {
		ET_ASSERT( _dispatchStub == nullptr,	"Leaking Wren handle!" );
		ET_ASSERT( _events.IsEmpty(),			"Leaking Wren event handles!" );
	}

// ---------------------------------------------------

	void EventCoordinator::ProcessTick( WrenVM* wren, uint64 deltaMicroseconds ) {
		if (!IsPaused()) {
			_gameTime += deltaMicroseconds;
		}

		WrenHandle* const	dispatchStub( _dispatchStub );
		const GameTimestamp	now( GetNow() );

		wrenEnsureSlots( wren, 1 );

	/*	Events are sorted according to dispatch time, with events that complete at an earlier time coming before those completing at a later time. */
		while (_events) {
			ScriptEvent& mostRecent( _events.Top() );

			if (mostRecent.ShouldDispatch( now )) {
				mostRecent.Dispatch( wren, dispatchStub );
				mostRecent.FreeResources( wren );
			
				_events.Pop();	
			} else {
				break;
			}
		}
	}

// ---------------------------------------------------

	ErrorCode EventCoordinator::BindResources( WrenVM* wren ) {
		_dispatchStub = wrenMakeCallHandle( wren, "call(_)" );

		return Error::None;
	}

// ---------------------------------------------------

	void EventCoordinator::FreeResources( WrenVM* wren ) {
		while (_events) {
			_events.Top().FreeResources( wren );

			_events.Pop();
		}

		if (WrenHandle* stub = eastl::exchange( _dispatchStub, nullptr )) {
			wrenReleaseHandle( wren, stub );
		}
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2