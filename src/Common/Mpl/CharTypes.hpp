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
#define U16L(_Literal) ET_UTF16_LITERAL(_Literal)
#define U32L(_Literal) ET_UTF32_LITERAL(_Literal)
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

template <typename OutCharacter, size_t StagingGranularity = 64u, typename InputIterator, typename OutputIterator>
OutputIterator TranscodeString(InputIterator begin, InputIterator end, OutputIterator output) ETNoexceptHint;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Mpl/CharTypes.inl>
//------------------------------------------------------------------//
