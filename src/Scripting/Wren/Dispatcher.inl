/*==================================================================*\
  Dispatcher.inl
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

namespace Eldritch2 { namespace Scripting { namespace Wren {

	enum : uint64 {
		MicrosecondsPerSecond = 1000000,
		TicksPerSecond        = 300,
		MicrosecondsPerTick   = (MicrosecondsPerSecond / TicksPerSecond)
	};

	// ---------------------------------------------------

	ETInlineHint Dispatcher::Event::Event(Timestamp dispatchTime, WrenHandle* receiver) :
		dispatchTime(dispatchTime),
		receiver(receiver) {}

	// ---------------------------------------------------

	ETInlineHint bool Dispatcher::Event::ShouldDispatch(Timestamp now) const {
		return dispatchTime <= now;
	}

	// ---------------------------------------------------

	ETInlineHint Dispatcher::Timestamp Dispatcher::GetNow() const {
		return AsTimestamp(_gameTime);
	}

	// ---------------------------------------------------

	ETInlineHint uint64 Dispatcher::SetGameTime(uint64 microseconds) {
		return eastl::exchange(_gameTime, microseconds);
	}

	// ---------------------------------------------------

	ETInlineHint void Dispatcher::CallAtGameTime(Timestamp time, WrenHandle* receiver) {
		_events.Emplace(time, receiver);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool operator<(const Dispatcher::Event& event0, const Dispatcher::Event& event1) {
		/*	The comparison ordering here is somewhat unintutitive at first; conceptually we want the list ordered by ascending dispatch time such that events scheduled
			 *	to complete sooner sort before those completing later. However, the semantics of @ref PriorityQueue mean that a call to Top() returns the element with greatest
			 *	priority, i.e. with lowest timestamp. Naive semantics result in the exact opposite invariant; the elements with the largest timestamp will be returned before
			 *	others. */
		return event1.dispatchTime < event0.dispatchTime;
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Dispatcher::Timestamp AsTimestamp(uint64 microseconds) {
		return static_cast<Dispatcher::Timestamp>(microseconds / MicrosecondsPerTick);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint uint64 AsMicroseconds(Dispatcher::Timestamp timestamp) {
		return timestamp * TicksPerSecond * MicrosecondsPerSecond;
	}

}}} // namespace Eldritch2::Scripting::Wren
