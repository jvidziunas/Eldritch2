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

namespace Eldritch2 { namespace Scripting { namespace Wren {

	template <typename... Arguments>
	ETInlineHint ETForceInlineHint void Context::WriteLog(Logging::Severity type, StringView format, Arguments&&... arguments) const {
		_log.Write(type, format, eastl::forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Context::WriteLog(const Utf8Char* const string, size_t lengthInOctets) const ETNoexceptHint {
		_log.Write(string, lengthInOctets);
	}

	// ---------------------------------------------------

	template <typename Type>
	ETInlineHint ETForceInlineHint WrenHandle* Context::FindForeignClass() const ETNoexceptHint {
		const auto candidate(_classesByType.Find(typeid(Type)));
		return candidate != _classesByType.End() ? candidate->second : nullptr;
	}

	// ---------------------------------------------------

	template <size_t arity>
	ETInlineHint ETForceInlineHint WrenHandle* Context::GetCallStubForArity() const ETNoexceptHint {
		static_assert(arity < ETCountOf(_callStubByArity), "Arity too high for Context implementation!");
		return _callStubByArity[arity];
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint ETForceInlineHint Context* GetContext(WrenVM* vm) ETNoexceptHint {
		return static_cast<Context*>(wrenGetUserData(vm));
	}

}}} // namespace Eldritch2::Scripting::Wren
