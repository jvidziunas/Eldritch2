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
#include <Common/Mpl/Compiler.hpp>
#include <Common/Mpl/Platform.hpp>
//------------------------------------------------------------------//
#include <EABase/eabase.h>
//------------------------------------------------------------------//

#if defined(_UNICODE) || defined(UNICODE)
#	define ET_BUILD_UNICODE 1
#	define ET_STRING_LITERAL(_Literal) L##_Literal
#else
#	define ET_BUILD_UNICODE 0
#	define ET_STRING_LITERAL(_Literal) _Literal
#endif
#if (ET_COMPILER_SUPPORTS_CPP11)

#else
#	if (ET_PLATFORM_WINDOWS)
#		define ET_UTF16_LITERAL(_Literal) L##_Literal
#	else
#		define ET_UTF16_LITERAL(_Literal) _Literal
#	endif
#	if (ET_PLATFORM_LINUX)
#		define ET_UTF32_LITERAL(_Literal) L##_Literal
#	else
#		define ET_UTF32_LITERAL(_Literal) _Literal
#	endif
#endif

#define SL(_Literal) ET_STRING_LITERAL(_Literal)
#define UTF16L(_Literal) ET_UTF16_LITERAL(_Literal)
#define UTF32L(_Literal) ET_UTF32_LITERAL(_Literal)
#define ETIsBuildUnicode() ET_BUILD_UNICODE

namespace Eldritch2 {

using Utf8Char  = char;
using Utf16Char = char16_t;
using Utf32Char = char32_t;

#if ET_PLATFORM_WINDOWS && ET_BUILD_UNICODE
using PlatformChar = wchar_t;
#else
using PlatformChar = char;
#endif

template <typename InputIterator, typename OutputIterator>
OutputIterator AsPlatformString(InputIterator begin, InputIterator end, OutputIterator output);

template <typename InputIterator, typename OutputIterator>
OutputIterator AsUtf8String(InputIterator begin, InputIterator end, OutputIterator output);

} // namespace Eldritch2
