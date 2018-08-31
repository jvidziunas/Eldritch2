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

struct WrenVM;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	template <typename T, typename... Arguments>
	T& SetReturn(WrenVM* vm, Arguments&&... arguments);

	template <typename T>
	ETPureFunctionHint T& GetSlotAs(WrenVM* vm, int slot) ETNoexceptHint;

	ETPureFunctionHint StringView GetSlotStringView(WrenVM* vm, int slot) ETNoexceptHint;

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/Marshal.inl>
//------------------------------------------------------------------//
