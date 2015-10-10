/*==================================================================*\
  Platform.hpp
  ------------------------------------------------------------------
  Purpose:
  Provides a centralized feature set and emulation capabilities for
  many compiler-specific features and additionally performs checks
  and defines convenience macros for other aspects of the
  application to change their behavior based on the compiler used
  to build the source.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
// _WINDOWS can be defined to force a build, or Windows.h can be included
#if defined( _WINDOWS ) || defined( _WIN32 ) || defined( _WIN64 )
#	include <Utility/MPL/Platform.Win.hpp>
#endif
// Mac stuff
#if defined( _MAC ) || defined( __APPLE__ ) || defined( __CARBON__ )
#	include <Utility/MPL/Platform.MacOS.hpp>
#endif
// Linux and derivatives are slightly more involved
#if defined( _LINUX )	|| defined( __linux__ ) || defined(	__unix__ ) || defined( __LP64__ )
#	include <Utility/MPL/Platform.Linux.hpp>
#endif
// Xbox is fairly straightforward
#if defined( _X360 ) || defined( _XBOX360 ) || defined( _XBOX )
#	include <Utility/MPL/Platform.Xenon.hpp>
#endif
// PS3 requires some manual defines
#if defined( _PS3 ) || defined( _PLAYSTATION3 ) || defined( __CELLOS_LV2__ )
#	include <Utility/MPL/Platform.PS3.hpp>
#endif
//------------------------------------------------------------------//

// Aliases for certain platform types
#define ET_PLATFORM_XENON                                ET_PLATFORM_XBOX360
#define ET_PLATFORM_PS3                                  ET_PLATFORM_PLAYSTATION3

// Function-style checks. These could theoretically be used
// in C/C++-level 'if/else' blocks, as opposed to the preprocessor.
// Obviously, these are certainly usable within the preprocessor
// regardless, if such is the coding style.
#define ETIsPlatformWindows()                            ET_PLATFORM_WINDOWS
#define ETIsPlatformMac()                                ET_PLATFORM_MAC
#define ETIsPlatformLinux()                              ET_PLATFORM_LINUX
#define ETIsPlatformXbox360()                            ET_PLATFORM_XBOX360
#define ETIsPlatformXenon()                              ET_PLATFORM_XENON
#define ETIsPlatformPlaystation3()                       ET_PLATFORM_PLAYSTATION3
#define ETIsPlatformPS3()                                ET_PLATFORM_PS3
#define ETIsPlatformUnknown()                            ET_PLATFORM_UNKNOWN

#define ETIsPlatformConsole()                            ( ETIsPlatformXenon() || ETIsPlatformPS3() )

#if defined( _X64 ) || defined( _x64 ) || defined( _M_X64 ) || defined( __x86_64__ )
#	define ET_PLATFORM_64BIT                             1
#	define ET_PLATFORM_32BIT                             0
#	define ET_PLATFORM_X86                               1
#	define ET_PLATFORM_POWERPC                           0
#elif defined( _X86 ) || defined( _x86 ) || defined( __i386__ ) || defined( _M_IX86 )
#	define ET_PLATFORM_64BIT                             0
#	define ET_PLATFORM_32BIT                             1
#	define ET_PLATFORM_X86                               1
#	define ET_PLATFORM_POWERPC                           0
#endif

// Function-style checks. These could theoretically be used
// in C/C++-level 'if/else' blocks, as opposed to the preprocessor.
// Obviously, these are certainly usable within the preprocessor
// regardless, if such is the coding style.
#define ETIsPlatform64Bit()                              ET_PLATFORM_64BIT
#define ETIsPlatform32Bit()                              ET_PLATFORM_32BIT
#define ETIsPlatformX86Derivative()                      ET_PLATFORM_X86
#define ETIsPlatformPowerPCDerivative()                  ET_PLATFORM_POWERPC