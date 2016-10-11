/*==================================================================*\
  IntrusiveForwardListHook.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <eastl/bonus/intrusive_slist.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	using IntrusiveForwardListBaseHook = eastl::intrusive_slist_node;

}	// namespace Eldritch2