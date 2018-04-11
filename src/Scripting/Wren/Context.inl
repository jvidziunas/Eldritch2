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

void*	wrenGetUserData( WrenVM* );

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	template <size_t formatSize, typename... Arguments>
	ETInlineHint void Context::WriteLog( Logging::MessageType type, const Utf8Char (&format)[formatSize], Arguments&&... arguments ) const {
		_log.Write( type, format, eastl::forward<Arguments>( arguments )... );
	}

// ---------------------------------------------------

	template <typename Type>
	ETInlineHint WrenHandle* Context::FindForeignClass() const {
		const auto	candidate( _classesByType.Find( typeid(Type) ) );

		return candidate != _classesByType.End() ? candidate->second : nullptr;
	}

// ---------------------------------------------------

	ETInlineHint WrenHandle* Context::GetUnaryCallStub() const {
		return _unaryCallStub;
	}

// ---------------------------------------------------

	ETInlineHint WrenVM* Context::GetVm() {
		return _vm;
	}
	
// ---------------------------------------------------

	ETInlineHint Context& AsContext( WrenVM* vm ) {
		return *static_cast<Context*>(wrenGetUserData( vm ));
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2