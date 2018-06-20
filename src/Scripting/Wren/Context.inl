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

	template <size_t formatSize, typename... Arguments>
	ETInlineHint void Context::WriteLog(Logging::MessageType type, const Utf8Char (&format)[formatSize], Arguments&&... arguments) const {
		_log.Write(type, format, eastl::forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	ETInlineHint void Context::WriteLog(const Utf8Char* const string, size_t lengthInOctets) const {
		_log.Write(string, lengthInOctets);
	}

	// ---------------------------------------------------

	template <typename Type>
	ETInlineHint WrenHandle* Context::FindForeignClass() const {
		const auto candidate(_classesByType.Find(typeid(Type)));

		return candidate != _classesByType.End() ? candidate->second : nullptr;
	}

	// ---------------------------------------------------

	template <size_t arity>
	ETInlineHint WrenHandle* Context::GetCallStubForArity() const {
		static_assert(arity < _countof(_callStubByArity), "Arity too high for Context implementation!");

		return _callStubByArity[arity];
	}

	// ---------------------------------------------------

	ETInlineHint Context& AsContext(WrenVM* vm) {
		return *static_cast<Context*>(wrenGetUserData(vm));
	}

}}} // namespace Eldritch2::Scripting::Wren
