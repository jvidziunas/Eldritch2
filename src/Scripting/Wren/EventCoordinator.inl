/*==================================================================*\
  EventCoordinator.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	enum : uint64 {
		MicrosecondsPerSecond	= 1000000,
		TicksPerSecond			= 300,

		MicrosecondsPerTick		= (MicrosecondsPerSecond / TicksPerSecond)
	};

// ---------------------------------------------------

	ETInlineHint EventCoordinator::ScriptEvent::ScriptEvent( GameTimestamp dispatchTime, WrenHandle* receiver ) : dispatchTime( dispatchTime ), receiver( receiver ) {}

// ---------------------------------------------------

	ETInlineHint bool EventCoordinator::ScriptEvent::ShouldDispatch( GameTimestamp now ) const {
		return dispatchTime <= now;
	}

// ---------------------------------------------------

	ETInlineHint EventCoordinator::GameTimestamp EventCoordinator::GetNow() const {
		return AsTimestamp( _gameTime );
	}

// ---------------------------------------------------

	ETInlineHint void EventCoordinator::SetPaused( bool value ) {
		_isPaused = value;
	}

// ---------------------------------------------------

	ETInlineHint bool EventCoordinator::IsPaused() const {
		return _isPaused;
	}

// ---------------------------------------------------

	ETInlineHint void EventCoordinator::CallAtGameTime( GameTimestamp time, WrenHandle* function ) {
		_events.Emplace( time, function );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool operator<( const EventCoordinator::ScriptEvent& event0, const EventCoordinator::ScriptEvent& event1 ) {
	/*	The comparison ordering here is somewhat unintutitive at first; conceptually we want the list ordered by ascending dispatch time such that events scheduled
	 *	to complete sooner sort before those completing later. However, the semantics of @ref PriorityQueue mean that a call to Top() returns the element with greatest
	 *	priority, i.e. with lowest timestamp. Naive semantics result in the exact opposite invariant; the elements with the largest timestamp will be returned before
	 *	others. */	
		return event1.dispatchTime < event0.dispatchTime;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint EventCoordinator::GameTimestamp AsTimestamp( uint64 microseconds ) {
		return static_cast<EventCoordinator::GameTimestamp>(microseconds / MicrosecondsPerTick);
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint uint64 AsMicroseconds( EventCoordinator::GameTimestamp timestamp ) {
		return timestamp * TicksPerSecond * MicrosecondsPerSecond;
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2