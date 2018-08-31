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
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Core;

	Dispatcher::Event::~Event() {
		ET_ASSERT(receiver == nullptr, "Leaking receiver for Wren event!");
	}

	// ---------------------------------------------------

	void Dispatcher::Event::FreeResources(WrenVM* wren) {
		wrenReleaseHandle(wren, eastl::exchange(receiver, nullptr));
	}

	// ---------------------------------------------------

	bool Dispatcher::Event::Call(WrenVM* wren, WrenHandle* arity0Stub) {
		wrenSetSlotHandle(wren, 0, receiver);
		return wrenCall(wren, arity0Stub) == WREN_RESULT_SUCCESS;
	}

	// ---------------------------------------------------

	Dispatcher::Dispatcher(World& world) :
		_world(ETAddressOf(world)),
		_gameTime(0u),
		_events(MallocAllocator("Wren Event Queue Allocator")) {
	}

	// ---------------------------------------------------

	Dispatcher::~Dispatcher() {
		ET_ASSERT(_events.IsEmpty(), "Leaking Wren event handles!");
	}

	// ---------------------------------------------------

	double Dispatcher::GetWorldTimeScalar() const {
		return _world->GetTimeScalar();
	}

	// ---------------------------------------------------

	void Dispatcher::SetWorldTimeScalar(double value) {
		_world->SetTimeScalar(float32(value));
	}

	// ---------------------------------------------------

	void Dispatcher::ExecuteScriptEvents(WrenVM* vm, MicrosecondTime deltaMicroseconds) {
		WrenHandle* const callStub(GetContext(vm)->GetCallStubForArity<0>());
		const ScriptTime  now(GetNow());

		/*	Events are sorted according to dispatch time, with events that complete at an earlier time coming before
		 *	those completing at a later time. */
		wrenEnsureSlots(vm, 1);
		while (_events) {
			Event& mostRecent(_events.Top());
			if (!mostRecent.ShouldDispatch(now)) {
				break;
			}

			mostRecent.Call(vm, callStub);
			mostRecent.FreeResources(vm);

			_events.Pop();
		}

		_gameTime += deltaMicroseconds;
	}

	// ---------------------------------------------------

	ErrorCode Dispatcher::BindResources(WrenVM* /*vm*/) {
		return Error::None;
	}

	// ---------------------------------------------------

	void Dispatcher::FreeResources(WrenVM* vm) {
		while (_events) {
			_events.Top().FreeResources(vm);
			_events.Pop();
		}
	}

	// ---------------------------------------------------

	void Dispatcher::ShutDownWorld(bool andEngine) const {
		_world->SetShouldShutDown(andEngine);
	}

}}} // namespace Eldritch2::Scripting::Wren
