/*==================================================================*\
  CharTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  Defines character types used to provide additional semantic
  (though not necessarily type-safe) information with regard to
  parameter, return and field type data.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
#if( ET_COMPILER_IS_MSVC )
// MSVC currently does not correctly implement char16_t and char32_t; they are implemented as typedefs
// inside a compiler header instead of as distinct integral/primitive types.
#	include <yvals.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {

	using UTF8Char		= char;
	using UTF16Char		= char16_t;
	using UTF32Char		= char32_t;

#if( ET_PLATFORM_WINDOWS )
#	if( ET_BUILD_UNICODE )
	using SystemChar	= wchar_t;
#	else
	using SystemChar	= char;
#	endif
#else
	using SystemChar	= char;
#endif

}	// namespace Eldritch2