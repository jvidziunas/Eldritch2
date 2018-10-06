/*==================================================================*\
  Dispatcher.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/Dispatcher.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	ScriptEvent::~ScriptEvent() {
		ET_ASSERT(receiver == nullptr, "Leaking receiver for Wren event!");
	}

	// ---------------------------------------------------

	Dispatcher::Dispatcher() :
		_requestedShutdown(false),
		_timeScalar(1.0),
		_gameTime(0u),
		_events(MallocAllocator("Wren Event Queue Allocator")) {
	}

	// ---------------------------------------------------

	Dispatcher::~Dispatcher() {
		ET_ASSERT(_events.IsEmpty(), "Leaking Wren event handles!");
	}

	// ---------------------------------------------------

	void Dispatcher::ExecuteScriptEvents(Context& context, MicrosecondTime deltaMicroseconds) {
		const ScriptEvent::Time now(AsScriptTime(eastl::exchange(_gameTime, _gameTime + deltaMicroseconds)));
		while (_events && _events.Top().ShouldDispatch(now)) {
			/*	Events are sorted according to dispatch time, with events that complete at an earlier time coming before
			 *	those completing at a later time. */
			ScriptEvent mostRecent(eastl::move(_events.Top()));
			_events.Pop();

			context.Call(mostRecent.receiver);
			context.Free(eastl::exchange(mostRecent.receiver, nullptr));
		}
	}

	// ---------------------------------------------------

	ErrorCode Dispatcher::BindResources(Context& context, double timeScalar, MicrosecondTime now) {
		using ::Eldritch2::Swap;

		EventQueue events(_events.GetContainer().GetAllocator());
		ET_AT_SCOPE_EXIT(for (ScriptEvent& event
							  : events.GetContainer()) {
			context.Free(event.receiver);
		});

		Swap(_events, events);
		SetTimeScalar(timeScalar);
		SetGameTime(now);

		return Error::None;
	}

	// ---------------------------------------------------

	void Dispatcher::FreeResources(Context& context) {
		for (ScriptEvent& event : _events.GetContainer()) {
			context.Free(eastl::exchange(event.receiver, nullptr));
		}

		_events.GetContainer().Clear();
	}

}}} // namespace Eldritch2::Scripting::Wren
