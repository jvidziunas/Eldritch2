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

struct	WrenHandle;

void*	wrenSetSlotNewForeign( WrenVM*, int, int, size_t );
void*	wrenGetSlotForeign( WrenVM*, int );
void	wrenSetSlotHandle( WrenVM*, int, WrenHandle* );

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	template <typename T, typename... Arguments>
	ETInlineHint T& SetReturn( WrenVM* vm, Arguments&&... arguments ) {
		return *new(wrenSetSlotNewForeign( vm, 0, 0, sizeof(T) )) T( eastl::forward<Arguments>( arguments )... );
	}

// ---------------------------------------------------

	template <typename T>
	ETPureFunctionHint ETInlineHint T& GetSlotAs( WrenVM* vm, int slot ) {
		return *static_cast<T*>(wrenGetSlotForeign( vm, slot ));
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2