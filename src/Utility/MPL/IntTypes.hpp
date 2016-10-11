/*==================================================================*\
  IntTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  As C++ does not currently have a standardized inttypes.h header
  for working with fixed-width types, we'll just roll our own.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <EABase/eabase.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	using int8		= int8_t;
	using uint8		= uint8_t;
	using int16		= int16_t;
	using uint16	= uint16_t;
	using int32		= int32_t;
	using uint32	= uint32_t;
	using int64		= int64_t;
	using uint64	= uint64_t;
	using intmax	= intmax_t;
	using uintmax	= uintmax_t;

	using ptrdiff	= ptrdiff_t;
	using intptr	= intptr_t;
	using uintptr	= uintptr_t;

}	// namespace Eldritch2