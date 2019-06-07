/*==================================================================*\
  Marshal.inl
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
	template <typename T>
	void wrenSetSlotCppTypeHandle(WrenVM* vm, int slot) ETNoexceptHint;
}}} // namespace Eldritch2::Scripting::Wren

void*       wrenSetSlotNewForeign(WrenVM*, int, int, size_t);
const char* wrenGetSlotBytes(WrenVM*, int, int*);
void*       wrenGetSlotForeign(WrenVM*, int);

namespace Eldritch2 { namespace Scripting { namespace Wren {

	template <typename T, typename... Arguments>
	ETForceInlineHint T& wrenSetReturn(WrenVM* vm, int classSlot, Arguments&&... arguments) {
		return *new (wrenSetSlotNewForeign(vm, /*slot=*/0, classSlot, sizeof(T))) T(Forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	template <typename T, typename... Arguments>
	ETForceInlineHint T& wrenSetReturn(WrenVM* vm, Arguments&&... arguments) {
		wrenSetSlotCppTypeHandle<T>(vm, 0);
		return wrenSetReturn<T>(vm, /*classSlot =*/0, Forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint T& wrenGetSlotAs(WrenVM* vm, int slot) ETNoexceptHint {
		return *static_cast<T*>(wrenGetSlotForeign(vm, slot));
	}

	// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint T& wrenGetReceiver(WrenVM* vm) ETNoexceptHint {
		return wrenGetSlotAs<T>(vm, 0);
	}

	// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint StringSpan wrenGetSlotStringView(WrenVM* vm, int slot) ETNoexceptHint {
		int               length;
		const char* const data(wrenGetSlotBytes(vm, slot, ETAddressOf(length)));

		return { data, StringSpan::SizeType(length) };
	}

}}} // namespace Eldritch2::Scripting::Wren
