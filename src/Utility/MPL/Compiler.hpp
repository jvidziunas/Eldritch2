/*==================================================================*\
  Compiler.hpp
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

//------------------------------------------------------------------//

#if defined( __cplusplus ) && ( __cplusplus != 199711L )
#	define ET_COMPILER_SUPPORTS_CPP11 1
#else
#	define ET_COMPILER_SUPPORTS_CPP11 0
#endif

#define _NULL_MACRO                    ((void)0)
#define _ET_NULL_DEFINE
#define ET_PREPROCESSOR_JOIN(a, b)     ET_PREPROCESSOR_JOIN1(a, b)
#define ET_PREPROCESSOR_JOIN1(a, b)    ET_PREPROCESSOR_JOIN2(a, b)
#define ET_PREPROCESSOR_JOIN2(a, b)    a##b

// Since the Intel compiler tries to be a drop-in replacement
// for MSVC, we'll check against its properties first
// in order to rule it out
#if defined( __INTEL_COMPILER ) || defined( __ICC ) || defined( __ECC )

#	define ET_COMPILER_IS_MSVC           0
#	define ET_COMPILER_IS_GCC            0
#	define ET_COMPILER_IS_INTEL          1
	// Macro helpers
#	define ET_MULTILINE_MACRO_BEGIN      do {
#	define ET_MULTILINE_MACRO_END        } while( 0 )
	// Calling conventions
#	define ETCDecl                       __cdecl
#	define ETStdCall                     __stdcall
#	define ETFastCall                    __fastcall
#	define ETSimdCall
	// Compiler optimization hints
#	define ETInlineHint                  __inline
#	define ETForceInlineHint             __forceinline
#	define ETNoInlineHint                __declspec(noinline)
#	define ETNakedHint                   __declspec(naked)
#	define ETNoReturnHint                __declspec(noreturn)
#	define ETNoThrowHint                 __declspec(nothrow)
#	define ETRestrictHint                __declspec(restrict)
#	define ETPureFunctionHint                 __declspec(noalias)
#	define ETPureAbstractHint            __declspec(novtable)
#	define ETDeprecatedHint(x)           __declspec(deprecated(x))
#	define ETThreadLocal                 __declspec(thread)
#	define ETPassInRegisterHint          _ET_NULL_DEFINE
#	define ETAssumeHint(x)               _NULL_MACRO
#	define ETBranchLikelyHint(x)         (x)
#	define ETBranchUnlikelyHint(x)       (x)
#	define ETNoDefaultCaseHint           _ET_NULL_DEFINE
	// Data alignment
#	define ET16BitAligned                __declspec(align(2))
#	define ET32BitAligned                __declspec(align(4))
#	define ET64BitAligned                __declspec(align(8))
#	define ET16ByteAligned               alignas(16)
#	define ET32ByteAligned               alignas(32)
#	define ET128BitAligned               alignas(16)
#	define ETHotSpotHint                 _ET_NULL_DEFINE
#	define ETColdSpotHint                _ET_NULL_DEFINE
#	define ETDLLImport                   __declspec(dllimport)
#	define ETDLLExport                   __declspec(dllexport)
#	define ETSPUVector                   _ET_NULL_DEFINE
#	define ETAltiVecVector               _ET_NULL_DEFINE
#	define TOPREPROCESSORSTRING(x)       #x
#	define STRINGIZE(x)                  TOPREPROCESSORSTRING(x)
#	define TOPREPROCESSORCHAR(x)         #@x
#	define CHARIZE(x)                    TOPREPROCESSORCHAR(x)
#	define __LINE_STRING__               STRINGIZE(__LINE__)
#	define COMPILERMESSAGEGENERIC(x)     _ET_NULL_DEFINE
#	define COMPILERMESSAGENEEDSFIX(x)    _ET_NULL_DEFINE
#	define COMPILERMESSAGEHACK(x)        _ET_NULL_DEFINE
#	define COMPILERMESSAGETODO(x)        _ET_NULL_DEFINE
#	define COMPILERMESSAGEKNOWNBUG(x)    _ET_NULL_DEFINE
#	define ET_LINK_LIBRARY(x)            _ET_NULL_DEFINE
#	if( ET_PLATFORM_X86 )
#		define ET_TRIGGER_DEBUGBREAK()   { __asm int 3 }
#	else
#		error Update Compiler.hpp to include an inline debug break for the Intel compiler collection.
#	endif
#	define abstract                      =0
#	define override                      _ET_NULL_DEFINE
#	define ENUM_CLASS(type)              enum
#	define STRONG_ENUM_CLASS(name, type) enum name

#elif defined( _MSC_VER ) // Is the compiler MSVC?
#if !defined( _MT )
#	error The engine requires that it be built with the Multithreaded CRT (debug or release) library for convenience.
#endif
#	define ET_COMPILER_IS_MSVC         1
#	define ET_COMPILER_IS_GCC          0
#	define ET_COMPILER_IS_INTEL        0
	// Macro helpers
#	define ET_MULTILINE_MACRO_BEGIN    do {
#	define ET_MULTILINE_MACRO_END      __pragma( warning( push ) ) \
                                       __pragma( warning( disable : 4127 ) ) \
                                       } while( 0 ) \
                                       __pragma( warning( pop ) )
	// Calling conventions
#	define ETCDecl                     __cdecl
#	define ETStdCall                   __stdcall
#	define ETFastCall                  __fastcall
#if( _MSC_VER >= 1800 )
#	define ETSimdCall                  __vectorcall
#else
#	define ETSimdCall
#endif
	// Compiler optimization hints
#	define ETInlineHint                __inline
#	define ETForceInlineHint           __forceinline
#	define ETNoInlineHint              __declspec(noinline)
#	define ETNakedHint                 __declspec(naked)
#	define ETNoReturnHint              __declspec(noreturn)
#	define ETNoThrowHint               __declspec(nothrow)
#	define ETRestrictHint              __declspec(restrict)
#	define ETPureFunctionHint          __declspec(noalias)
#	define ETPureAbstractHint          __declspec(novtable)
#	define ETDeprecatedHint(x)         __declspec(deprecated(x))
#	define ETThreadLocal               __declspec(thread)
#	if( ET_PLATFORM_XBOX360 )
#		define ETPassInRegisterHint	   __declspec(passinreg)
#	else
#		define ETPassInRegisterHint    _ET_NULL_DEFINE
#	endif
#	define ETAssumeHint(x)             __assume(x)
#	define ETBranchLikelyHint(x)       (x)
#	define ETBranchUnlikelyHint(x)     (x)
#	define ETNoDefaultCaseHint         __assume(0)
#	define ETRestrictPtrHint	       __restrict
	// Data alignment
#	define ET16BitAligned              __declspec(align(2))
#	define ET32BitAligned              __declspec(align(4))
#	define ET64BitAligned              __declspec(align(8))
#	define ET16ByteAligned             __declspec(align(16))
#	define ET32ByteAligned             __declspec(align(32))
#	define ET128BitAligned             __declspec(align(16))
#	define ETHotSpotHint               _ET_NULL_DEFINE
#	define ETColdSpotHint              _ET_NULL_DEFINE
#	define ETDLLImport                 __declspec(dllimport)
#	define ETDLLExport                 __declspec(dllexport)
#	define ETSPUVector                 _ET_NULL_DEFINE
#	define ETAltiVecVector             _ET_NULL_DEFINE
#	define TOPREPROCESSORSTRING(x)     #x
#	define TOWPREPROCESSORSTRING(x)    L##x		
#	define STRINGIZE(x)                TOPREPROCESSORSTRING(x)
#	define WSTRINGIZE(x)               TOWPREPROCESSORSTRING(x)
#	define TOPREPROCESSORCHAR(x)       #@x
#	define CHARIZE(x)                  TOPREPROCESSORCHAR(x)
#	define __WFILE__                   WSTRINGIZE(__FILE__)
#	define __LINE_STRING__             STRINGIZE(__LINE__)
#	define ET_FUNCTION_STRING          __FUNCTION__
#	define COMPILERMESSAGEGENERIC(x)   __pragma( message (__FILE__  "(" __LINE_STRING__ ") NOTE: " x ) )
#	define COMPILERMESSAGEWARNING(x)   __pragma( message (__FILE__  "(" __LINE_STRING__ ") WARNING: " x ) )
#	define COMPILERMESSAGENEEDSFIX(x)  __pragma( message (__FILE__  "(" __LINE_STRING__ ") FIX: " x) )
#	define COMPILERMESSAGEHACK(x)      __pragma( message (__FILE__  "(" __LINE_STRING__ ") HACK: " x) )
#	define COMPILERMESSAGETODO(x)      __pragma( message (__FILE__  "(" __LINE_STRING__ ") TODO: " x) )
#	define COMPILERMESSAGEKNOWNBUG(x)  __pragma( message (__FILE__  "(" __LINE_STRING__ ") BUG: " x) )
#	define ET_LINK_LIBRARY(x)          __pragma( comment( lib, x ) )
#	define ET_TRIGGER_DEBUGBREAK()     __debugbreak()
// Since the override specifiers and enum class language extensions
// are safely wrapped away in here, turn off the warnings.
#	pragma warning( disable : 4480 )
#	pragma warning( disable : 4481 )
// Some template classes will have unreferenced inline members. The optimizer/linker will strip these out, so we don't need to worry.
#	pragma warning( disable : 4505 )
// Some libraries test compile-time constants against one another inside an if statement. The optimizer will realistically catch this and remove any runtime cost.
#	pragma warning( disable : 4127 )
// We don't need to do anything for the abstract/override keyword support.
#if 0
#		define abstract                    abstract
#		if( ! ET_COMPILER_SUPPORTS_CPP11 )
#			define override                override
#		endif
#endif
#	if( _MSC_VER >= 1400 )
#		define ENUM_CLASS(type)              enum : type
#		define STRONG_ENUM_CLASS(name, type) enum name : type
#	else
#		define ENUM_CLASS(type)              enum
#		define STRONG_ENUM_CLASS(name, type) enum x
#	endif
#	pragma warning( push )
// Disable complaints about default constructors/assignment operators being inaccessible due to the noncopyable pattern;
// this will just result in an error if you try to do it anyway. Why Microsoft thinks it needs to warn you on top of this is baffling
#		pragma warning( disable : 4510 4610 4512 )
// Another annoying warning of questionable utility. This one is about the compiler doing what you asked it to re: data alignment.
#		pragma warning( disable : 4324 )

#elif defined( __GNUG__ ) || ( defined( __GNUC__ && defined( __cplusplus ) ) ) // Is the compiler GCC?

#	define ET_COMPILER_IS_MSVC           0
#	define ET_COMPILER_IS_GCC            1
#	define ET_COMPILER_IS_INTEL          0
	// Macro helpers
#	define ET_MULTILINE_MACRO_BEGIN      do {
#	define ET_MULTILINE_MACRO_END        } while( 0 )
	// Calling conventions
#	define ETCDecl                       __attribute__((cdecl))
#	define ETStdCall                     __attribute__((stdcall))
#	define ETFastCall                    __attribute__((fastcall))
#	define ETSimdCall
	// Compiler optimization hints
#	define ETInlineHint                  inline
#	define ETForceInlineHint             __attribute__((always_inline))
#	define ETNoInlineHint                __attribute__((noinline))
#	define ETNakedHint                   __attribute__((naked))
#	define ETNoReturnHint                __attribute__((noreturn))
#	define ETNoThrowHint                 __attribute__((nothrow))
#	define ETRestrictHint                __attribute__((malloc))
#	define ETPureFunctionHint            __attribute__((pure))
#	define ETPureAbstractHint            _ET_NULL_DEFINE
#	define ETDeprecatedHint(x)           __attribute__((deprecated(x)))
#	define ETThreadLocal                 __thread
#	define ETPassInRegisterHint          _ET_NULL_DEFINE
#	define ETAssumeHint(x)               _NULL_MACRO
#	define ETBranchLikelyHint(x)         __builtin_expect((x), 1)
#	define ETBranchUnlikelyHint(x)       __builtin_expect((x), 0)
#	define ETNoDefaultCaseHint           _ET_NULL_DEFINE
#	define ETRestrictPtrHint	         __restrict__
	// Data alignment
#	define ET16BitAligned                __attribute__((aligned(2)))
#	define ET32BitAligned                __attribute__((aligned(4)))
#	define ET64BitAligned                __attribute__((aligned(8)))
#	define ET16ByteAligned               __attribute__((aligned(16)))
#	define ET32ByteAligned               __attribute__((aligned(32)))
#	define ET128BitAligned               __attribute__((aligned(16)))
#	define ETHotSpotHint                 __attribute__((hot))
#	define ETColdSpotHint                __attribute__((cold))
#	if( ET_PLATFORM_WINDOWS )
#		define ETDLLImport               __attribute__((dllimport))
#		define ETDLLExport               __attribute__((dllexport))
#	else
#		define ETDLLImport               _ET_NULL_DEFINE
#		define ETDLLExport               _ET_NULL_DEFINE
#	endif
#	if( ET_PLATFORM_PLAYSTATION3 )
#		define ETSPUVector               __attribute__((spu_vector))
#	else
#		define ETSPUVector               _ET_NULL_DEFINE
#	endif
#	define ETAltiVecVector               __attribute__((altivec(vector__)))
#	define TOPREPROCESSORSTRING(x)       #x
#	define STRINGIZE(x)                  TOPREPROCESSORSTRING(x)
#	define __LINE_STRING__               STRINGIZE(__LINE__)
#	define COMPILERMESSAGEGENERIC(x)     _ET_NULL_DEFINE
#	define COMPILERMESSAGENEEDSFIX(x)    _ET_NULL_DEFINE
#	define COMPILERMESSAGEHACK(x)        _ET_NULL_DEFINE
#	define COMPILERMESSAGETODO(x)        _ET_NULL_DEFINE
#	define COMPILERMESSAGEKNOWNBUG(x)    _ET_NULL_DEFINE
#	define ET_LINK_LIBRARY(x)            _ET_NULL_DEFINE
#	if( ET_PLATFORM_PS3 )
#		define ET_TRIGGER_DEBUGBREAK()   asm volatile("tw 31,1,1")
#	elif( ET_PLATFORM_POWERPC )
#		define ET_TRIGGER_DEBUGBREAK()   asm(".long 0")
#	else
#		define ET_TRIGGER_DEBUGBREAK()   __builtin_trap()
#	endif
#	define abstract                      =0
#	if( !ET_COMPILER_SUPPORTS_CPP11 )
#		define override                  _ET_NULL_DEFINE
#	endif
#	define ENUM_CLASS(type)              enum
#	define STRONG_ENUM_CLASS(name, type) enum name

#endif

#define ETUnreferencedParameter(ParameterName)         static_cast<void>( ParameterName )

// Syntatic sugar. The function-style #define can be
// used to make platform-dependent code and if/else
// blocks look nice, while the compiler symbol-style
// #define can be used to control typedefs and similar.
#define ETCompilerSupportsCPP11()	   ET_COMPILER_SUPPORTS_CPP11
#define ETIsCompilerMSVC()			   ET_COMPILER_IS_MSVC
#define ETIsCompilerGCC()			   ET_COMPILER_IS_GCC
#define ETIsCompilerIntel()			   ET_COMPILER_IS_INTEL
//-------------------------------------------------------

// Debug mode check and unified interface
#if defined( _DEBUG ) || defined( DEBUG )
#	define ET_DEBUG_MODE_ENABLED       1
#	define ET_RELEASE_MODE_ENABLED     0
#else
#	define ET_DEBUG_MODE_ENABLED       0
#	define ET_RELEASE_MODE_ENABLED     1
#endif
#define ETIsDebugModeEnabled()		   ET_DEBUG_MODE_ENABLED
#define ETIsReleaseModeEnabled()	   ET_RELEASE_MODE_ENABLED
//-------------------------------------------------------

#define ETCacheLineAligned             ET16ByteAligned

using ETPostfixOperatorHint = int;

//==================================================================//
// STATIC ANALYSIS
//==================================================================//
#if( ET_COMPILER_IS_MSVC )
#	include <sal.h>
#	define	ETFormatStringHint(_String) __format_string _String
#else
#	define	ETFormatStringHint(_String) _String
#endif
//------------------------------------------------------------------//
