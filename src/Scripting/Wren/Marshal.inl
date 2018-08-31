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

const char* wrenGetSlotBytes(WrenVM*, int, int*);
void*       wrenSetSlotNewForeign(WrenVM*, int, int, size_t);
void*       wrenGetSlotForeign(WrenVM*, int);

namespace Eldritch2 { namespace Scripting { namespace Wren {

	template <typename T, typename... Arguments>
	ETInlineHint T& SetReturn(WrenVM* vm, Arguments&&... arguments) {
		return *new (wrenSetSlotNewForeign(vm, 0, 0, sizeof(T))) T(eastl::forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	template <typename T>
	ETInlineHint ETPureFunctionHint T& GetSlotAs(WrenVM* vm, int slot) ETNoexceptHint {
		return *static_cast<T*>(wrenGetSlotForeign(vm, slot));
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint StringView GetSlotStringView(WrenVM* vm, int slot) ETNoexceptHint {
		int               length;
		const char* const data(wrenGetSlotBytes(vm, slot, &length));

		return { data, StringView::SizeType(length) };
	}

}}} // namespace Eldritch2::Scripting::Wren
