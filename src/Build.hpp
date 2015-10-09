/*==================================================================*\
  Build.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

#if !defined( PROJECT_NAME )
#	define PROJECT_NAME "Hero"
#endif

#if !defined( VERSION_STRING )
#	define VERSION_STRING "1.0"
#endif

#if !defined( WPROJECT_NAME )
#	define WPROJECT_NAME ET_PREPROCESSOR_JOIN( L, PROJECT_NAME )
#endif

#if !defined( WVERSION_STRING )
#	define WVERSION_STRING ET_PREPROCESSOR_JOIN( L, VERSION_STRING )
#endif

#if !defined( PROJECT_NAME_LIT )
#	define PROJECT_NAME_LIT SL( PROJECT_NAME )
#endif

#if !defined( UTF8_PROJECT_NAME )
#	define UTF8_PROJECT_NAME UTF8L( PROJECT_NAME )
#endif

#if !defined( UTF16_PROJECT_NAME )
#	define UTF16_PROJECT_NAME UTF16L( PROJECT_NAME )
#endif

#if !defined( UTF32_PROJECT_NAME )
#	define UTF32_PROJECT_NAME UTF32L( PROJECT_NAME )
#endif

#if !defined( VERSION_STRING_LIT )
#	define VERSION_STRING_LIT SL( VERSION_STRING )
#endif

#if !defined( UTF8_VERSION_STRING )
#	define UTF8_VERSION_STRING UTF8L( VERSION_STRING )
#endif

#if !defined( UTF16_VERSION_STRING )
#	define UTF16_VERSION_STRING UTF16L( VERSION_STRING )
#endif

#if !defined( UTF32_VERSION_STRING )
#	define UTF32_VERSION_STRING UTF32L( VERSION_STRING )
#endif

#if !defined( ARCHITECTURE_STRING )
#	if( ET_PLATFORM_X86 )
#		if( ET_PLATFORM_64BIT )
#			define ARCHITECTURE_STRING "x64"
#		elif( ET_PLATFORM_32BIT )
#			define ARCHITECTURE_STRING "x86"
#		else
#			define ARCHITECTURE_STRING ""
#		endif
#	elif( ET_PLATFORM_POWERPC )
#		define ARCHITECTURE_STRING "ppc"
#	endif
#endif

#if !defined( WARCHITECTURE_STRING )
#	define WARCHITECTURE_STRING ET_PREPROCESSOR_JOIN( L, ARCHITECTURE_STRING )
#endif

#if !defined( ARCHITECTURE_STRING_LIT )
#	define ARCHITECTURE_STRING_LIT SL( ARCHITECTURE_STRING )
#endif

#if !defined( UTF8_ARCHITECTURE_STRING )
#	define UTF8_ARCHITECTURE_STRING UTF8L( ARCHITECTURE_STRING )
#endif

#if !defined( UTF16_ARCHITECTURE_STRING )
#	define UTF16_ARCHITECTURE_STRING UTF16L( ARCHITECTURE_STRING )
#endif

#if !defined( UTF32_VERSION_STRING )
#	define UTF32_ARCHITECTURE_STRING UTF32L( ARCHITECTURE_STRING )
#endif

extern const ::Eldritch2::UTF8Char* const	buildDateStamp;
extern const wchar_t* const					wideBuildDateStamp;