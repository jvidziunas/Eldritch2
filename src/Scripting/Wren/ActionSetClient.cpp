/*==================================================================*\
  ActionSetClient.cpp
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
#include <Scripting/Wren/ActionSetClient.hpp>
#include <Scripting/Wren/ScriptExecutor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Input;

	// ---------------------------------------------------

	namespace {

		static ETConstexpr int32 InitialActionWeight(0);

	} // anonymous namespace

	ActionSetClient::ActionSetClient() ETNoexceptHint : ArrayList<InputAction>(MallocAllocator("Wren Action Set Allocator")), _links(MallocAllocator("Device Binding List Allocator")) {}

	// ---------------------------------------------------

	ActionSetClient::~ActionSetClient() {
		ETAssert(IsEmpty(), "Leaking Wren handles!");
	}

	// ---------------------------------------------------

	void ActionSetClient::Activate(Action id, int32 amount) {
		InputAction& action((*this)[ArrayList<InputAction>::SizeType(id)]);
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

	bool ActionSetClient::DispatchEvents(ScriptExecutor& context) {
		ETConstexpr uint64 PressedMask(~0u);
		ETConstexpr uint64 ReleasedMask(~PressedMask);

		for (InputAction& action : *this) {
			const uint64 state(action.combinedWeight.exchange(0u, std::memory_order_consume));
			const uint32 pressed(state & PressedMask);
			const uint32 released((state & ReleasedMask) >> 32u);

			if (ET_UNLIKELY(!context.Call(action.closure, /*isPressed =*/pressed != 0u, /*isReleased =*/released != 0u, /*weight =*/AsFloat64(pressed) - AsFloat64(released)))) {
				return false;
			}
		}

		return true;
	}

	// ---------------------------------------------------

	Result ActionSetClient::BindResources(ScriptExecutor& vm, int bindingsSlot) {
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

		return Result::Success;
	}

	// ---------------------------------------------------

	void ActionSetClient::FreeResources(ScriptExecutor& vm) ETNoexceptHint {
		ClearAndDispose([&](InputAction& action) ETNoexceptHint {
			vm.Free(Exchange(action.closure, nullptr));
		});
	}

}}} // namespace Eldritch2::Scripting::Wren
