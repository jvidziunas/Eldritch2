/*==================================================================*\
  ScriptExecutor.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	namespace {

		ETForceInlineHint void wrenSetSlot(WrenVM* vm, int slot, nullptr_t /*value*/) ETNoexceptHint {
			wrenSetSlotNull(vm, slot);
		}

		// ---------------------------------------------------

		ETForceInlineHint void wrenSetSlot(WrenVM* vm, int slot, WrenHandle* value) ETNoexceptHint {
			wrenSetSlotHandle(vm, slot, value);
		}

		// ---------------------------------------------------

		ETForceInlineHint void wrenSetSlot(WrenVM* vm, int slot, bool value) ETNoexceptHint {
			wrenSetSlotBool(vm, slot, value);
		}

		// ---------------------------------------------------

		ETForceInlineHint void wrenSetSlot(WrenVM* vm, int slot, double value) ETNoexceptHint {
			wrenSetSlotDouble(vm, slot, value);
		}

		// ---------------------------------------------------

		template <typename T>
		ETConstexpr void wrenSetSlot(WrenVM* /*vm*/, int /*slot*/, T /*unused*/) ETNoexceptHint {
			/* Wren only knows about the types overloaded above. If the compiler is complaining you're trying to call this function, it's because you've passed an argument to
			 * @ref CallVariadic() that Wren does not support as a primitive type. Usually this is because an object reference/pointer is being used directly; in this case
			 * consider obtaining a Wren handle to the object and sending the handle instead. (e.g. use `wrenGetSlotHandle()` instead of `wrenGetSlotForeign()`) */
			static_assert(sizeof(T) == 0, "Unsupported Wren type passed as method call argument!");
		}

		// ---------------------------------------------------

		template <int... Indices, typename... Arguments>
		ETForceInlineHint bool CallVariadic(WrenVM* vm, WrenHandle* stub, IntegerSequence<int, Indices...>, Arguments... arguments) ETNoexceptHint {
			wrenEnsureSlots(vm, sizeof...(Arguments));
			DiscardArguments((wrenSetSlot(vm, Indices, arguments), 0)...);

			return wrenCall(vm, stub) == WREN_RESULT_SUCCESS;
		}

	} // anonymous namespace

	template <typename... Arguments>
	ETInlineHint ETForceInlineHint bool ScriptExecutor::Call(WrenHandle* receiver, Arguments... arguments) ETNoexceptHint {
		static_assert(sizeof...(Arguments) < ETCountOf(_callStubByArity), "Arity too high for Context implementation!");
		// We're a little clever here and sneak the receiver to the variadic call function as the 0th argument (it doesn't need to be treated any differently)
		return CallVariadic(_vm, _callStubByArity[sizeof...(Arguments)], IntegerSequenceFor<int, WrenHandle*, Arguments...>(), receiver, Forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	template <typename... Arguments>
	ETInlineHint ETForceInlineHint bool ScriptExecutor::New(WrenHandle* wrenClass, Arguments... arguments) ETNoexceptHint {
		static_assert(sizeof...(Arguments) < ETCountOf(_newStubByArity), "Arity too high for Context implementation!");
		// We're a little clever here and sneak the receiver/class to the variadic call function as the 0th argument (it doesn't need to be treated any differently)
		return CallVariadic(_vm, _newStubByArity[sizeof...(Arguments)], IntegerSequenceFor<int, WrenHandle*, Arguments...>(), wrenClass, Forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	ETInlineHint WrenHandle* ScriptExecutor::BindCallStub(const char* signature) ETNoexceptHint {
		return wrenMakeCallHandle(_vm, signature);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ScriptExecutor::Free(WrenHandle* handle) ETNoexceptHint {
		if (handle == nullptr) {
			return;
		}

		wrenReleaseHandle(_vm, handle);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint ScriptExecutor* GetExecutor(WrenVM* vm) ETNoexceptHint {
		return static_cast<ScriptExecutor*>(wrenGetUserData(vm));
	}

	// ---------------------------------------------------

	template <typename Class>
	ETInlineHint ETForceInlineHint void wrenSetSlotCppTypeHandle(WrenVM* vm, int slot) ETNoexceptHint {
		const auto& classesByType(GetExecutor(vm)->_classByCppType);
		const auto  candidate(classesByType.Find(GetType<Class>()));
		ETAssert(candidate != classesByType.End(), "Attempting to use unregistered C++ type with Wren VM!");

		wrenSetSlotHandle(vm, slot, candidate->second);
	}

}}} // namespace Eldritch2::Scripting::Wren
