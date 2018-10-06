/*==================================================================*\
  Context.inl
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

void* wrenGetUserData(WrenVM*);
void  wrenSetSlotHandle(WrenVM* vm, int slot, WrenHandle* handle);
void  wrenSetSlotDouble(WrenVM* vm, int slot, double value);
void  wrenSetSlotBool(WrenVM* vm, int slot, bool value);
void  wrenReleaseHandle(WrenVM* vm, WrenHandle* handle);
void  wrenEnsureSlots(WrenVM* vm, int numSlots);

namespace Eldritch2 { namespace Scripting { namespace Wren {

	template <typename... Arguments>
	ETInlineHint ETForceInlineHint bool Context::Call(WrenHandle* receiver, Arguments... arguments) {
		static_assert(sizeof...(Arguments) < ETCountOf(_callStubByArity), "Arity too high for Context implementation!");

		wrenEnsureSlots(_vm, sizeof...(Arguments) + 1u);
		wrenSetSlotHandle(_vm, 0, receiver);
		wrenCall(_vm, _callStubByArity[sizeof...(Arguments)]);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Context::Free(WrenHandle* handle) ETNoexceptHint {
		if (handle == nullptr) {
			return;
		}

		wrenReleaseHandle(_vm, handle);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint Context* GetContext(WrenVM* vm) ETNoexceptHint {
		return static_cast<Context*>(wrenGetUserData(vm));
	}

}}} // namespace Eldritch2::Scripting::Wren

template <typename Class>
ETInlineHint ETPureFunctionHint void wrenSetSlotCppTypeHandle(WrenVM* vm, int slot) ETNoexceptHint {
	const auto& classesByType(GetContext(vm)->_classByCppType);
	const auto  candidate(classesByType.Find(typeid(Class)));
	ET_ASSERT(candidate != classesByType.End(), "Attempting to use unregistered C++ type with Wren VM!");

	wrenSetSlotHandle(vm, slot, candidate->second);
}
