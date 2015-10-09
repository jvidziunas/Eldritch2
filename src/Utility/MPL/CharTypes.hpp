/*==================================================================*\
  CharTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  Defines character types used to provide additional semantic
  (though not necessarily type-safe) information with regard to
  parameter, return and field type data.

  ------------------------------------------------------------------
  ©2010-2011 Eldritch Entertainment, LLC.
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

	typedef char		UTF8Char;
	typedef char16_t	UTF16Char;
	typedef char32_t	UTF32Char;

#if( ET_BUILD_UNICODE )
	typedef wchar_t		SystemChar;
#else
	typedef char		SystemChar;
#endif

}	// namespace Eldritch2