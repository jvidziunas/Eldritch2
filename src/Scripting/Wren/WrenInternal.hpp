/*==================================================================*\
  WrenInternal.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//
/*	(4200) Wren makes extensive use of flexible arrays for its objects, but MSVC works the way it wants.
 *	(4100) Some Wren utility functions don't use the VM parameter. */
ET_PUSH_MSVC_WARNING_STATE(disable : 4200 4100)
#include <wren/src/vm/wren_value.h>
#include <wren/src/vm/wren_vm.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	template <typename Type>
	ETPureFunctionHint Type& AsForeign(WrenHandle* objectHandle) ETNoexceptHint;

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/WrenInternal.inl>
//------------------------------------------------------------------//
