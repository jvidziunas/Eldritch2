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
/*	(4200) Wren makes extensive use of flexible arrays for its objects, but MSVC works the way it wants.
 *	(4100) Some Wren utility functions don't use the VM parameter. */
ET_PUSH_MSVC_WARNING_STATE(disable : 4200 4100)
#include <wren/src/vm/wren_value.h>
#include <wren/src/vm/wren_vm.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//
