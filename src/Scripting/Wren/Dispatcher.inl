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

	ETConstexpr ETInlineHint ETForceInlineHint ScriptEvent::ScriptEvent(GameTime when, WrenHandle* receiver) ETNoexceptHint : when(when), receiver(receiver) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ScriptEvent::ScriptEvent(ScriptEvent&& event) ETNoexceptHint : when(event.when), receiver(Exchange(event.receiver, nullptr)) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool ScriptEvent::ShouldDispatch(GameTime now) const ETNoexceptHint {
		return when <= now;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ScriptEvent& ScriptEvent::operator=(ScriptEvent&& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(when, rhs.when);
		Swap(receiver, rhs.receiver);

		return *this;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint GameTime Dispatcher::GetNow() const ETNoexceptHint {
		return AsGameTime(_gameTime);
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETInlineHint ETForceInlineHint MicrosecondTime Dispatcher::SetGameTime(MicrosecondTime microseconds) ETNoexceptHint {
		return Exchange(_gameTime, microseconds);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint double Dispatcher::GetTimeScalar() const ETNoexceptHint {
		return _targetTimeScalar;
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETInlineHint ETForceInlineHint double Dispatcher::SetTimeScalar(double value) ETNoexceptHint {
		const double _(_targetTimeScalar);
		_targetTimeScalar = value;

		return _;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Dispatcher::CallAtGameTime(GameTime when, WrenHandle* receiver) {
		_events.Emplace(when, receiver);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Dispatcher::CallOnDelay(GameTime delay, WrenHandle* receiver) {
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

	ETConstexpr ETForceInlineHint ETPureFunctionHint bool operator<(const ScriptEvent& lhs, const ScriptEvent& rhs) ETNoexceptHint {
		/*	The comparison ordering here is somewhat unintutitive at first; conceptually we want the list ordered by ascending dispatch time such that events scheduled
		 *	to complete sooner sort before those completing later. However, the semantics of @ref PriorityQueue mean that a call to Top() returns the element with greatest
		 *	priority, i.e. with smallest timestamp. Naive semantics result in the exact opposite invariant; the elements with the greatest timestamp will be returned before
		 *	others. */
		return rhs.when < lhs.when;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint GameTime operator+(GameTime lhs, GameTime rhs) ETNoexceptHint {
		return GameTime(uint64(lhs) + uint64(rhs));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint GameTime operator-(GameTime lhs, GameTime rhs) ETNoexceptHint {
		return GameTime(uint64(lhs) - uint64(rhs));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint GameTime& operator+=(GameTime& lhs, GameTime rhs) ETNoexceptHint {
		return lhs = (lhs + rhs);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint GameTime& operator-=(GameTime& lhs, GameTime rhs) ETNoexceptHint {
		return lhs = (lhs - rhs);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint GameTime AsGameTime(MicrosecondTime when) ETNoexceptHint {
		return GameTime(uint64(when) / MicrosecondsPerTick);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint MicrosecondTime AsMicrosecondTime(GameTime when) ETNoexceptHint {
		return MicrosecondTime(uint64(when) * MicrosecondsPerTick);
	}

}}} // namespace Eldritch2::Scripting::Wren
