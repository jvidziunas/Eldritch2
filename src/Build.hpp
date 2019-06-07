/*==================================================================*\
  Build.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/CharTypes.hpp>
//------------------------------------------------------------------//

#if !defined(PROJECT_NAME)
#	define PROJECT_NAME "Hero"
#endif

#if !defined(VERSION_STRING)
#	define VERSION_STRING "1.0"
#endif

#if !defined(PROJECT_NAME_LIT)
#	define PROJECT_NAME_LIT SL(PROJECT_NAME)
#endif

#if !defined(UTF16_PROJECT_NAME)
#	define UTF16_PROJECT_NAME U16L(PROJECT_NAME)
#endif

#if !defined(UTF32_PROJECT_NAME)
#	define UTF32_PROJECT_NAME U32L(PROJECT_NAME)
#endif

#if !defined(VERSION_STRING_LIT)
#	define VERSION_STRING_LIT SL(VERSION_STRING)
#endif

#if !defined(U16_VERSION_STRING)
#	define U16_VERSION_STRING U16L(VERSION_STRING)
#endif

#if !defined(U32_VERSION_STRING)
#	define U32_VERSION_STRING U32L(VERSION_STRING)
#endif

#if !defined(ARCHITECTURE_STRING)
#	if (ET_PLATFORM_X86)
#		if (ET_PLATFORM_64BIT)
#			define ARCHITECTURE_STRING "x64"
#		elif (ET_PLATFORM_32BIT)
#			define ARCHITECTURE_STRING "x86"
#		else
#			define ARCHITECTURE_STRING ""
#		endif
#	elif (ET_PLATFORM_POWERPC)
#		define ARCHITECTURE_STRING "ppc"
#	endif
#endif

#if !defined(ARCHITECTURE_STRING_LIT)
#	define ARCHITECTURE_STRING_LIT SL(ARCHITECTURE_STRING)
#endif

#if !defined(U16_ARCHITECTURE_STRING)
#	define U16_ARCHITECTURE_STRING U16L(ARCHITECTURE_STRING)
#endif

#if !defined(U32_ARCHITECTURE_STRING)
#	define U32_ARCHITECTURE_STRING U32L(ARCHITECTURE_STRING)
#endif

extern const ::Eldritch2::Utf8Char* const buildDate;
extern const wchar_t* const               wideBuildDate;
