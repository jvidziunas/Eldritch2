/*==================================================================*\
  ActionSet.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenInternal.hpp>
#include <Scripting/Wren/ActionSet.hpp>
#include <Input/DeviceLocator.hpp>
//------------------------------------------------------------------//

void wrenSetSlotHandle(WrenVM* vm, int slot, WrenHandle* handle);
void wrenSetSlotDouble(WrenVM* vm, int slot, double value);
void wrenReleaseHandle(WrenVM* vm, WrenHandle* handle);

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Input;

	ActionSet::ActionSet() :
		_actions(MallocAllocator("Wren Action List Allocator")),
		_bindings(MallocAllocator("Device Binding List Allocator")) {
	}

	// ---------------------------------------------------

	ActionSet::~ActionSet() {
		ET_ASSERT(_actions.IsEmpty(), "Leaking Wren handles!");
		ReleaseDevices();
	}

	// ---------------------------------------------------

	void ActionSet::Activate(ActionId id, int32 amount) {
		Action& action(_actions[id]);

		action.weight += amount;
		action.flags |= Pressed;
	}

	// ---------------------------------------------------

	void ActionSet::OnConnect() {}

	// ---------------------------------------------------

	void ActionSet::OnDisconnect() {}

	// ---------------------------------------------------

	bool ActionSet::DispatchEvents(WrenVM* vm, WrenHandle* arity3Call) {
		wrenEnsureSlots(vm, 4);

		for (Action& action : _actions) {
			const uint32 flags(eastl::exchange(action.flags, 0u));

			wrenSetSlotHandle(vm, 0, action.closure);
			wrenSetSlotBool(vm, 1, flags & Pressed);
			wrenSetSlotBool(vm, 2, flags & Released);
			wrenSetSlotDouble(vm, 3, AsFloat64(eastl::exchange(action.weight, 0)));

			if (ET_UNLIKELY(wrenCall(vm, arity3Call) != WREN_RESULT_SUCCESS)) {
				return false;
			}
		}

		return true;
	}

	// ---------------------------------------------------

	bool ActionSet::TryAcquireDevice(DeviceLocator& locator, DeviceId id) {
		return locator.TryAcquireDevice(id, InputDevice::BindingMap<>(MallocAllocator("asdf")), *this);
	}

	// ---------------------------------------------------

	void ActionSet::ReleaseDevices() {
		for (const DeviceBinding& binding : _bindings) {
			binding.locator->ReleaseDevice(binding.id);
		}
	}

	// ---------------------------------------------------

	void ActionSet::BindResources(WrenVM* vm, int bindingsSlot) {
		WrenHandle* const   map(wrenGetSlotHandle(vm, bindingsSlot));
		const ObjMap* const bindings(AS_MAP(map->value));
		ArrayList<Action>   actions(_actions.GetAllocator());

		for (const MapEntry& binding : Range<const MapEntry*>(bindings->entries, bindings->entries + bindings->capacity)) {
			if (ET_LIKELY(IS_STRING(binding.key) & IS_OBJ(binding.value))) {
				//	Action names must be strings, and bindings must implement call(_).
				actions.Append(Action { wrenMakeHandle(vm, binding.value), InitialWeight, None });
			}
		}

		wrenReleaseHandle(vm, map);
		FreeResources(vm);

		Swap(_actions, actions);
	}

	// ---------------------------------------------------

	void ActionSet::FreeResources(WrenVM* vm) {
		for (Action& action : _actions) {
			wrenReleaseHandle(vm, action.closure);
		}

		_actions.Clear();
	}

}}} // namespace Eldritch2::Scripting::Wren
