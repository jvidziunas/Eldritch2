/*==================================================================*\
  Dispatcher.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ScriptExecutor.hpp>
#include <Scripting/Wren/Dispatcher.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	ScriptEvent::~ScriptEvent() {
		ETAssert(receiver == nullptr, "Leaking receiver for Wren event!");
	}

	// ---------------------------------------------------

	Dispatcher::Dispatcher() ETNoexceptHint : _requestedShutdown(false), _targetTimeScalar(1.0), _gameTime(MicrosecondTime::Uninitialized), _events(EventQueue::AllocatorType("Wren Event Queue Allocator")) {}

	// ---------------------------------------------------

	Dispatcher::~Dispatcher() {
		ETAssert(_events.IsEmpty(), "Leaking Wren event handles!");
	}

	// ---------------------------------------------------

	void Dispatcher::ExecuteScriptEvents(ScriptExecutor& context, MicrosecondTime delta) {
		const GameTime now(AsGameTime(Exchange(_gameTime, _gameTime + delta)));
		while (_events && _events.Top().ShouldDispatch(now)) {
			/*	Events are sorted according to dispatch time, with events that complete at an earlier time coming before
			 *	those completing at a later time. */
			ScriptEvent earliest(Move(_events.Top()));
			_events.Pop();

			context.Call(earliest.receiver);
			context.Free(Exchange(earliest.receiver, nullptr));
		}
	}

	// ---------------------------------------------------

	Result Dispatcher::BindResources(ScriptExecutor& context, MicrosecondTime now) {
		using ::Eldritch2::Swap;

		EventQueue events(_events.GetContainer().GetAllocator());
		ET_AT_SCOPE_EXIT(events.GetContainer().ClearAndDispose([&](ScriptEvent& event) ETNoexceptHint {
			context.Free(Exchange(event.receiver, nullptr));
		}));

		SetGameTime(now);
		Swap(_events, events);

		return Result::Success;
	}

	// ---------------------------------------------------

	void Dispatcher::FreeResources(ScriptExecutor& context) ETNoexceptHint {
		_events.GetContainer().ClearAndDispose([&](ScriptEvent& event) ETNoexceptHint {
			context.Free(Exchange(event.receiver, nullptr));
		});
	}

}}} // namespace Eldritch2::Scripting::Wren
