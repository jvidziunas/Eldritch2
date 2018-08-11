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

	ETInlineHint Dispatcher::Event::Event(ScriptTime when, WrenHandle* receiver) :
		when(when),
		receiver(receiver) {}

	// ---------------------------------------------------

	ETInlineHint bool Dispatcher::Event::ShouldDispatch(ScriptTime now) const {
		return when <= now;
	}

	// ---------------------------------------------------

	ETInlineHint Dispatcher::ScriptTime Dispatcher::GetNow() const {
		return AsScriptTime(_gameTime);
	}

	// ---------------------------------------------------

	ETInlineHint MicrosecondTime Dispatcher::SetGameTime(MicrosecondTime microseconds) {
		return eastl::exchange(_gameTime, microseconds);
	}

	// ---------------------------------------------------

	ETInlineHint void Dispatcher::CallAtGameTime(ScriptTime time, WrenHandle* receiver) {
		_events.Emplace(time, receiver);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool operator<(const Dispatcher::Event& event0, const Dispatcher::Event& event1) {
		/*	The comparison ordering here is somewhat unintutitive at first; conceptually we want the list ordered by ascending dispatch time such that events scheduled
		 *	to complete sooner sort before those completing later. However, the semantics of @ref PriorityQueue mean that a call to Top() returns the element with greatest
		 *	priority, i.e. with smallest timestamp. Naive semantics result in the exact opposite invariant; the elements with the greatest timestamp will be returned before
		 *	others. */
		return event1.when < event0.when;
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Dispatcher::ScriptTime AsScriptTime(MicrosecondTime when) {
		return Dispatcher::ScriptTime(when / MicrosecondsPerTick);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint MicrosecondTime AsMicroseconds(Dispatcher::ScriptTime when) {
		return when * TicksPerSecond * MicrosecondsPerSecond;
	}

}}} // namespace Eldritch2::Scripting::Wren
