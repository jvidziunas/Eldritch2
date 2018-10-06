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

	// ---

	ETConstexpr ETInlineHint ETForceInlineHint ScriptEvent::ScriptEvent(Time when, WrenHandle* receiver) ETNoexceptHint : when(when), receiver(receiver) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ScriptEvent::ScriptEvent(ScriptEvent&& event) ETNoexceptHint : when(event.when), receiver(eastl::exchange(event.receiver, nullptr)) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool ScriptEvent::ShouldDispatch(Time now) const ETNoexceptHint {
		return when <= now;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ScriptEvent::Time Dispatcher::GetNow() const ETNoexceptHint {
		return AsScriptTime(_gameTime);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint MicrosecondTime Dispatcher::SetGameTime(MicrosecondTime microseconds) ETNoexceptHint {
		return eastl::exchange(_gameTime, microseconds);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint double Dispatcher::GetTimeScalar() const ETNoexceptHint {
		return _timeScalar;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint double Dispatcher::SetTimeScalar(double value) ETNoexceptHint {
		return eastl::exchange(_timeScalar, value);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Dispatcher::CallAtGameTime(ScriptEvent::Time when, WrenHandle* receiver) {
		_events.Emplace(when, receiver);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Dispatcher::CallOnDelay(ScriptEvent::Time delay, WrenHandle* receiver) {
		CallAtGameTime(GetNow() + delay, receiver);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint bool Dispatcher::ShouldShutDownWorld() const ETNoexceptHint {
		return _requestedShutdown;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint void Dispatcher::ShutDownWorld() ETNoexceptHint {
		_requestedShutdown = true;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator<(const ScriptEvent& lhs, const ScriptEvent& rhs) ETNoexceptHint {
		/*	The comparison ordering here is somewhat unintutitive at first; conceptually we want the list ordered by ascending dispatch time such that events scheduled
		 *	to complete sooner sort before those completing later. However, the semantics of @ref PriorityQueue mean that a call to Top() returns the element with greatest
		 *	priority, i.e. with smallest timestamp. Naive semantics result in the exact opposite invariant; the elements with the greatest timestamp will be returned before
		 *	others. */
		return rhs.when < lhs.when;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint ScriptEvent::Time AsScriptTime(MicrosecondTime when) ETNoexceptHint {
		return ScriptEvent::Time(when / MicrosecondsPerTick);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint MicrosecondTime AsMicroseconds(ScriptEvent::Time when) ETNoexceptHint {
		return when * MicrosecondsPerTick;
	}

}}} // namespace Eldritch2::Scripting::Wren
