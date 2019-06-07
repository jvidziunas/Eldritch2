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
	T& wrenSetReturn(WrenVM* vm, int classSlot, Arguments&&... arguments);
	template <typename T, typename... Arguments>
	T& wrenSetReturn(WrenVM* vm, Arguments&&... arguments);

	ETPureFunctionHint StringSpan wrenGetSlotStringView(WrenVM* vm, int slot) ETNoexceptHint;

	template <typename T>
	ETPureFunctionHint T& wrenGetSlotAs(WrenVM* vm, int slot) ETNoexceptHint;
	template <typename T>
	ETPureFunctionHint T& wrenGetReceiver(WrenVM* vm) ETNoexceptHint;

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/Marshal.inl>
//------------------------------------------------------------------//
