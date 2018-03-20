/*==================================================================*\
  Marshal.hpp
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

struct	WrenVM;

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	template <typename T, typename... Arguments>
	T&	SetReturn( WrenVM* vm, Arguments&&... arguments );

	template <typename T>
	ETPureFunctionHint T&	GetSlotAs( WrenVM* vm, int slot );

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/Marshal.inl>
//------------------------------------------------------------------//