/*==================================================================*\
  ActionSetClient.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ActionSetClient.hpp>
#include <Scripting/Wren/Context.hpp>
#include <Input/DeviceLocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Input;

	namespace {

		static ETConstexpr int32 InitialActionWeight(0);

	} // anonymous namespace

	ActionSetClient::ActionSetClient() :
		ArrayList<InputAction>(MallocAllocator("Wren Action Set Allocator")),
		_links(MallocAllocator("Device Binding List Allocator")) {}

	// ---------------------------------------------------

	ActionSetClient::~ActionSetClient() {
		ET_ASSERT(IsEmpty(), "Leaking Wren handles!");
		ReleaseDevices();
	}

	// ---------------------------------------------------

	void ActionSetClient::Activate(ActionId id, int32 amount) {
		InputAction& action((*this)[id]);
		if (amount > 0) {
			action.weights.pressed.fetch_add(uint32(amount), std::memory_order_relaxed);
		} else {
			action.weights.released.fetch_add(uint32(-amount), std::memory_order_relaxed);
		}
	}

	// ---------------------------------------------------

	void ActionSetClient::OnConnect(const InputDevice& /*device*/) {}

	// ---------------------------------------------------

	void ActionSetClient::OnDisconnect(const InputDevice& /*device*/) {}

	// ---------------------------------------------------

	bool ActionSetClient::DispatchEvents(Context& context) {
		ETConstexpr uint64 PressedMask(~0u);
		ETConstexpr uint64 ReleasedMask(~PressedMask);

		for (InputAction& action : *this) {
			const uint64 state(action.combinedWeight.exchange(0u, std::memory_order_consume));
			const uint32 pressed(state & PressedMask);
			const uint32 released((state & ReleasedMask) >> 32u);

			if (ET_UNLIKELY(!context.Call(action.closure, pressed != 0u, released != 0u, AsFloat64(pressed) - AsFloat64(released)))) {
				return false;
			}
		}

		return true;
	}

	// ---------------------------------------------------

	void ActionSetClient::AcquireDevice(DeviceLink link) {
		_links.Append(eastl::move(link));
	}

	// ---------------------------------------------------

	void ActionSetClient::ReleaseDevices() {
		_links.Clear();
	}

	// ---------------------------------------------------

	ErrorCode ActionSetClient::BindResources(Context& vm, int bindingsSlot) {
#if 0
		WrenHandle* const   map(wrenGetSlotHandle(vm, bindingsSlot));
		const ObjMap* const bindings(AS_MAP(map->value));
		ET_AT_SCOPE_EXIT(for (InputAction& action
							  : actions)
							 wrenReleaseHandle(vm, action.closure));
		for (const MapEntry& binding : Range<const MapEntry*>(bindings->entries, bindings->entries + bindings->capacity)) {
			if (ET_LIKELY(IS_STRING(binding.key) & IS_OBJ(binding.value))) {
				//	Action names must be strings, and bindings must implement call(_).
				actions.Append({ wrenMakeHandle(vm, binding.value), InitialActionWeight, None });
			}
		}

		wrenReleaseHandle(vm, map);
		FreeResources(vm);
#endif
		ArrayList<InputAction> actions(GetAllocator());

		Swap(*this, actions);

		return Error::None;
	}

	// ---------------------------------------------------

	void ActionSetClient::FreeResources(Context& vm) {
		for (InputAction& action : *this) {
			vm.Free(eastl::exchange(action.closure, nullptr));
		}

		Clear();
	}

}}} // namespace Eldritch2::Scripting::Wren
