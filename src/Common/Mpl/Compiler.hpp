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
#include <EABase/eabase.h>
//------------------------------------------------------------------//

#define _NULL_MACRO ((void)0)
#define _ET_NULL_DEFINE
#define ET_CPP_JOIN(a, b) ET_CPP_JOIN2(a, b)
#define ET_CPP_JOIN2(a, b) a##b

// Since the Intel compiler tries to be a drop-in replacement for MSVC, we'll check against its properties first in order to rule it out
#if defined(__INTEL_COMPILER) || defined(__ICC) || defined(__ECC)
#	define ET_COMPILER_IS_MSVC 0
#	define ET_COMPILER_IS_GCC 0
#	define ET_COMPILER_IS_ICC 1
//	Macro helpers
#	define ET_MULTILINE_MACRO_BEGIN do {
#	define ET_MULTILINE_MACRO_END \
		}                          \
		while (0)
//	Calling conventions
#	define ETCDecl __cdecl
#	define ETStdCall __stdcall
#	define ETFastCall __fastcall
#	define ETSimdCall
//	Compiler optimization hints
#	define ETInlineHint __inline
#	define ETForceInlineHint __forceinline
#	define ETNoInlineHint __declspec(noinline)
#	define ETNakedHint __declspec(naked)
#	define ETNoReturnHint __declspec(noreturn)
#	define ETNeverThrowsHint __declspec(nothrow)
#	define ETRestrictHint __declspec(restrict)
#	define ETPureFunctionHint __declspec(noalias)
#	define ETPureAbstractHint __declspec(novtable)
#	define ETDeprecatedHint(x) __declspec(deprecated(x))
#	define ETThreadLocal __declspec(thread)
#	define ETPassInRegisterHint _ET_NULL_DEFINE
#	define ET_ASSUME(x) _NULL_MACRO
#	define ET_LIKELY(x) (x)
#	define ET_UNLIKELY(x) (x)
#	define ET_NO_DEFAULT_CASE _ET_NULL_DEFINE
//	Data alignment
#	define ET16BitAligned __declspec(align(2))
#	define ET32BitAligned __declspec(align(4))
#	define ET64BitAligned __declspec(align(8))
#	define ET16ByteAligned alignas(16)
#	define ET32ByteAligned alignas(32)
#	define ET128BitAligned alignas(16)
#	define ETHotSpotHint _ET_NULL_DEFINE
#	define ETColdSpotHint _ET_NULL_DEFINE
#	define ETDllImport __declspec(dllimport)
#	define ETDllExport __declspec(dllexport)
#	define ETSPUVector _ET_NULL_DEFINE
#	define ETAltiVecVector _ET_NULL_DEFINE
#	define TOPREPROCESSORSTRING(x) #    x
#	define STRINGIZE(x) TOPREPROCESSORSTRING(x)
#	define TOPREPROCESSORCHAR(x) #@ x
#	define CHARIZE(x) TOPREPROCESSORCHAR(x)
#	define __LINE_STRING__ STRINGIZE(__LINE__)
#	define COMPILERMESSAGEGENERIC(x) _ET_NULL_DEFINE
#	define COMPILERMESSAGENEEDSFIX(x) _ET_NULL_DEFINE
#	define COMPILERMESSAGEHACK(x) _ET_NULL_DEFINE
#	define COMPILERMESSAGETODO(x) _ET_NULL_DEFINE
#	define COMPILERMESSAGEKNOWNBUG(x) _ET_NULL_DEFINE
#	define ET_LINK_LIBRARY(x) _ET_NULL_DEFINE
#	if (ET_PLATFORM_X86)
#		define ET_TRIGGER_DEBUGBREAK() \
			{ __asm int 3 }
#	else
#		error Update Compiler.hpp to include an inline debug break for the Intel compiler collection.
#	endif
#	define ET_PUSH_MSVC_WARNING_STATE(x) _ET_NULL_DEFINE
#	define ET_POP_MSVC_WARNING_STATE() _ET_NULL_DEFINE
#	define ET_SUPPRESS_MSVC_WARNINGS(x) _ET_NULL_DEFINE
#	define abstract = 0
#	define override _ET_NULL_DEFINE
#	define ENUM_CLASS(type) enum
#	define STRONG_ENUM_CLASS(name, type) enum name
#elif defined(_MSC_VER) // Is the compiler MSVC?
#	if !defined(_MT)
#		error The engine requires that it be built with the Multithreaded CRT (debug or release) library for convenience.
#	endif
#	if (_MSC_VER >= 1700)
#		define ET_COMPILER_SUPPORTS_CPP11 1
#	else
#		define ET_COMPILER_SUPPORTS_CPP11 0
#	endif
#	define ET_COMPILER_IS_MSVC 1
#	define ET_COMPILER_IS_GCC 0
#	define ET_COMPILER_IS_ICC 0
//	Macro helpers
#	define ET_MULTILINE_MACRO_BEGIN do {
#	define ET_MULTILINE_MACRO_END            \
		__pragma(warning(push))               \
			__pragma(warning(disable : 4127)) \
		}                                     \
		while (0)                             \
		__pragma(warning(pop))
//	Calling conventions
#	define ETCDecl __cdecl
#	define ETStdCall __stdcall
#	define ETFastCall __fastcall
#	if (_MSC_VER >= 1800)
#		define ETSimdCall __vectorcall
#	else
#		define ETSimdCall
#	endif
//	Compiler optimization hints
#	define ETInlineHint __inline
#	define ETForceInlineHint __forceinline
#	define ETNoInlineHint __declspec(noinline)
#	define ETNakedHint __declspec(naked)
#	define ETNoReturnHint __declspec(noreturn)
#	define ETNeverThrowsHint __declspec(nothrow)
#	define ETRestrictHint __declspec(restrict)
#	define ETPureFunctionHint __declspec(noalias)
#	define ETPureAbstractHint __declspec(novtable)
#	define ETDeprecatedHint(x) __declspec(deprecated(x))
#	define ETThreadLocal __declspec(thread)
#	define ET_ASSUME(x) __assume(x)
#	define ET_LIKELY(x) (x)
#	define ET_UNLIKELY(x) (x)
#	define ET_NO_DEFAULT_CASE \
	default: { __assume(0); }
#	define ETRestrictPtrHint __restrict
//	Data alignment
#	define ET16BitAligned __declspec(align(2))
#	define ET32BitAligned __declspec(align(4))
#	define ET64BitAligned __declspec(align(8))
#	define ET16ByteAligned __declspec(align(16))
#	define ET32ByteAligned __declspec(align(32))
#	define ET128BitAligned __declspec(align(16))
#	define ETHotSpotHint _ET_NULL_DEFINE
#	define ETColdSpotHint _ET_NULL_DEFINE
#	define ETDllImport __declspec(dllimport)
#	define ETDllExport __declspec(dllexport)
#	define ETSPUVector _ET_NULL_DEFINE
#	define ETAltiVecVector _ET_NULL_DEFINE
#	define TOPREPROCESSORSTRING(x) #    x
#	define TOWPREPROCESSORSTRING(x) L##x
#	define STRINGIZE(x) TOPREPROCESSORSTRING(x)
#	define WSTRINGIZE(x) TOWPREPROCESSORSTRING(x)
#	define TOPREPROCESSORCHAR(x) #@ x
#	define CHARIZE(x) TOPREPROCESSORCHAR(x)
#	define __WFILE__ WSTRINGIZE(__FILE__)
#	define __LINE_STRING__ STRINGIZE(__LINE__)
#	define ET_FUNCTION_STRING __FUNCTION__
#	define COMPILERMESSAGEGENERIC(x) __pragma(message(__FILE__ "(" __LINE_STRING__ ") NOTE: " x))
#	define COMPILERMESSAGEWARNING(x) __pragma(message(__FILE__ "(" __LINE_STRING__ ") WARNING: " x))
#	define COMPILERMESSAGENEEDSFIX(x) __pragma(message(__FILE__ "(" __LINE_STRING__ ") FIX: " x))
#	define COMPILERMESSAGEHACK(x) __pragma(message(__FILE__ "(" __LINE_STRING__ ") HACK: " x))
#	define COMPILERMESSAGETODO(x) __pragma(message(__FILE__ "(" __LINE_STRING__ ") TODO: " x))
#	define COMPILERMESSAGEKNOWNBUG(x) __pragma(message(__FILE__ "(" __LINE_STRING__ ") BUG: " x))
#	define ET_LINK_LIBRARY(x) __pragma(comment(lib, x))
#	define ET_TRIGGER_DEBUGBREAK() __debugbreak()
#	define ET_PUSH_MSVC_WARNING_STATE(x) __pragma(warning(push)) __pragma(warning(x))
#	define ET_POP_MSVC_WARNING_STATE() __pragma(warning(pop))
#	define ET_SUPPRESS_MSVC_WARNINGS(x) __pragma(warning(suppress \
														  : x))
//	We don't need to do anything for the abstract/override keyword support.
#	if 0
#		define abstract abstract
#		if (!ET_COMPILER_SUPPORTS_CPP11)
#			define override override
#		endif
#	endif
#elif defined(__GNUG__) || (defined(__GNUC__ && defined(__cplusplus))) // Is the compiler GCC?
#	define ET_COMPILER_IS_MSVC 0
#	define ET_COMPILER_IS_GCC 1
#	define ET_COMPILER_IS_ICC 0
//	Macro helpers
#	define ET_MULTILINE_MACRO_BEGIN do {
#	define ET_MULTILINE_MACRO_END \
		}                          \
		while (0)
//	Calling conventions
#	define ETCDecl __attribute__((cdecl))
#	define ETStdCall __attribute__((stdcall))
#	define ETFastCall __attribute__((fastcall))
#	define ETSimdCall _ET_NULL_DEFINE
//	Compiler optimization hints
#	define ETInlineHint inline
#	define ETForceInlineHint __attribute__((always_inline))
#	define ETNoInlineHint __attribute__((noinline))
#	define ETNakedHint __attribute__((naked))
#	define ETNoReturnHint __attribute__((noreturn))
#	define ETNeverThrowsHint __attribute__((nothrow))
#	define ETRestrictHint __attribute__((malloc))
#	define ETPureFunctionHint __attribute__((pure))
#	define ETPureAbstractHint _ET_NULL_DEFINE
#	define ETDeprecatedHint(x) __attribute__((deprecated(x)))
#	define ETThreadLocal __thread
#	define ETPassInRegisterHint _ET_NULL_DEFINE
#	define ET_ASSUME(x) _NULL_MACRO
#	define ET_LIKELY(x) __builtin_expect((x), 1)
#	define ET_UNLIKELY(x) __builtin_expect((x), 0)
#	define ET_NO_DEFAULT_CASE _ET_NULL_DEFINE
#	define ETRestrictPtrHint __restrict__
//	Data alignment
#	define ET16BitAligned __attribute__((aligned(2)))
#	define ET32BitAligned __attribute__((aligned(4)))
#	define ET64BitAligned __attribute__((aligned(8)))
#	define ET16ByteAligned __attribute__((aligned(16)))
#	define ET32ByteAligned __attribute__((aligned(32)))
#	define ET128BitAligned __attribute__((aligned(16)))
#	define ETHotSpotHint __attribute__((hot))
#	define ETColdSpotHint __attribute__((cold))
#	if ET_PLATFORM_WINDOWS
#		define ETDllImport __attribute__((dllimport))
#		define ETDllExport __attribute__((dllexport))
#	else
#		define ETDllImport _ET_NULL_DEFINE
#		define ETDllExport _ET_NULL_DEFINE
#	endif
#	define ETAltiVecVector __attribute__((altivec(vector__)))
#	define TOPREPROCESSORSTRING(x) #    x
#	define STRINGIZE(x) TOPREPROCESSORSTRING(x)
#	define __LINE_STRING__ STRINGIZE(__LINE__)
#	define COMPILERMESSAGEGENERIC(x) _ET_NULL_DEFINE
#	define COMPILERMESSAGENEEDSFIX(x) _ET_NULL_DEFINE
#	define COMPILERMESSAGEHACK(x) _ET_NULL_DEFINE
#	define COMPILERMESSAGETODO(x) _ET_NULL_DEFINE
#	define COMPILERMESSAGEKNOWNBUG(x) _ET_NULL_DEFINE
#	define ET_PUSH_MSVC_WARNING_STATE(x) _ET_NULL_DEFINE
#	define ET_POP_MSVC_WARNING_STATE() _ET_NULL_DEFINE
#	define ET_SUPPRESS_MSVC_WARNINGS(x) _ET_NULL_DEFINE
#	define ET_LINK_LIBRARY(x) _ET_NULL_DEFINE
#	if ET_PLATFORM_POWERPC
#		define ET_TRIGGER_DEBUGBREAK() asm(".long 0")
#	else
#		define ET_TRIGGER_DEBUGBREAK() __builtin_trap()
#	endif
#	define abstract = 0
#	if !ET_COMPILER_SUPPORTS_CPP11
#		define override _ET_NULL_DEFINE
#	endif
#endif

#if !defined(ET_COMPILER_SUPPORTS_CPP11)
#	if defined(__cplusplus) && (__cplusplus > 199711L)
#		define ET_COMPILER_SUPPORTS_CPP11 1
#	else
#		define ET_COMPILER_SUPPORTS_CPP11 0
#	endif
#endif

#if !defined(ET_COMPILER_SUPPORTS_CPP14)
#	if defined(__cplusplus) && (__cplusplus > 199711L)
#		define ET_COMPILER_SUPPORTS_CPP14 1
#	else
#		define ET_COMPILER_SUPPORTS_CPP14 0
#	endif
#endif

#define ETUnreferencedParameter(parameter) static_cast<void>(parameter)

/* Syntatic sugar. The function-style #define can be used to make platform-dependent code and if/else
 * blocks look nice, while the compiler symbol-style #define can be used to control typedefs and similar. */
#define ETCompilerSupportsCpp11() ET_COMPILER_SUPPORTS_CPP11
#define ETCompilerSupportsCpp14() ET_COMPILER_SUPPORTS_CPP14
#define ETIsCompilerMsvc() ET_COMPILER_IS_MSVC
#define ETIsCompilerGcc() ET_COMPILER_IS_GCC
#define ETIsCompilerIcc() ET_COMPILER_IS_ICC
//-------------------------------------------------------

// Debug mode check and unified interface
#if defined(_DEBUG) || defined(DEBUG)
#	define ET_DEBUG_BUILD 1
#	define ET_RELEASE_BUILD 0
#	define ETReleaseToggle(release, debug) debug
#else
#	define ET_DEBUG_BUILD 0
#	define ET_RELEASE_BUILD 1
#	define ETReleaseToggle(release, debug) release
#endif
#define ETIsDebugBuild() ET_DEBUG_BUILD
#define ETIsReleaseBuild() ET_RELEASE_BUILD

//-------------------------------------------------------

#define ETCacheLineAligned ET16ByteAligned

#if ET_DEBUG_BUILD
#	define ET_LINK_DEBUGGABLE_LIBRARY(debugLib, releaseLib) ET_LINK_LIBRARY(debugLib)
#else
#	define ET_LINK_DEBUGGABLE_LIBRARY(debugLib, releaseLib) ET_LINK_LIBRARY(releaseLib)
#endif

#if ET_COMPILER_SUPPORTS_CPP11
#	define ETNoexceptHint noexcept
#	define ETNoexceptHintIf(...) noexcept(__VA_ARGS__)
#	include <initializer_list>
#else
#	define ETNoexceptHint
#	define ETNoexceptHintIf(...)
#endif

#define ETConstexpr constexpr
#define ETCpp14Constexpr constexpr

#define ETInfiniteLoop for (;;)

using ETPostfixSemantics = int;

namespace Eldritch2 {

template <typename T, T Expansion>
struct IntegralConstant {
	// - TYPE PUBLISHING ---------------------------------

public:
	using ValueType = T;
	using Type      = IntegralConstant<T, Expansion>;

	// ---------------------------------------------------

public:
	static ETConstexpr T Value = Expansion;

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint operator ValueType() const ETNoexceptHint {
		return Value;
	}

	ETConstexpr ETForceInlineHint ValueType operator()() const ETNoexceptHint {
		return Value;
	}
};

struct ETPureAbstractHint EmptyBase {};

using FalseType = IntegralConstant<bool, false>;
using TrueType  = IntegralConstant<bool, true>;

using YesType = char;
using NoType  = char[2];

template <typename T>
struct Identity { using Type = T; };

template <typename...>
struct MakeVoid { using Type = void; };
template <typename... Ts>
using Void_t = typename MakeVoid<Ts...>::Type;

template <typename T>
struct RemoveArray : public Identity<T> {};
template <typename T, size_t ArraySize>
struct RemoveArray<const volatile T[ArraySize]> : public Identity<const volatile T> {};
template <typename T, size_t ArraySize>
struct RemoveArray<volatile T[ArraySize]> : public Identity<volatile T> {};
template <typename T, size_t ArraySize>
struct RemoveArray<const T[ArraySize]> : public Identity<const T> {};
template <typename T, size_t ArraySize>
struct RemoveArray<T[ArraySize]> : public Identity<T> {};
template <typename T>
struct RemoveArray<const volatile T[]> : public Identity<const volatile T> {};
template <typename T>
struct RemoveArray<volatile T[]> : public Identity<volatile T> {};
template <typename T>
struct RemoveArray<const T[]> : public Identity<const T> {};
template <typename T>
struct RemoveArray<T[]> : public Identity<T> {};

template <typename T>
struct RemovePointer : public Identity<T> {};
template <typename T>
struct RemovePointer<T* const volatile> : public Identity<T> {};
template <typename T>
struct RemovePointer<T* volatile> : public Identity<T> {};
template <typename T>
struct RemovePointer<T* const> : public Identity<T> {};
template <typename T>
struct RemovePointer<T*> : public Identity<T> {};

template <typename T>
struct RemoveReference : public Identity<T> {};
template <typename T>
struct RemoveReference<const volatile T&> : public Identity<const volatile T> {};
template <typename T>
struct RemoveReference<volatile T&> : public Identity<volatile T> {};
template <typename T>
struct RemoveReference<const T&> : public Identity<const T> {};
template <typename T>
struct RemoveReference<T&> : public Identity<T> {};

template <typename T>
struct RemoveConst : public Identity<T> {};
template <typename T>
struct RemoveConst<const T> : public Identity<T> {};

template <typename T>
struct RemoveVolatile : public Identity<T> {};
template <typename T>
struct RemoveVolatile<volatile T> : public Identity<T> {};

template <typename T>
struct RemoveConstVolatile : public Identity<typename RemoveConst<typename RemoveVolatile<T>::Type>::Type> {};

template <typename>
struct TupleSize : public IntegralConstant<size_t, 1u> {};
template <typename T>
struct TupleSize<T[]> { static_assert(sizeof(T) == 0u, "TupleSize cannot be used with arrays of unknown size"); };
template <typename T, size_t Size>
struct TupleSize<T[Size]> : public IntegralConstant<size_t, Size> {};
template <typename T, size_t Size>
struct TupleSize<const volatile T (&)[Size]> : public TupleSize<T[Size]> {};
template <typename T, size_t Size>
struct TupleSize<volatile T (&)[Size]> : public TupleSize<T[Size]> {};
template <typename T, size_t Size>
struct TupleSize<const T (&)[Size]> : public TupleSize<T[Size]> {};
template <typename T, size_t Size>
struct TupleSize<T (&)[Size]> : public TupleSize<T[Size]> {};

template <typename T>
ETConstexpr ETForceInlineHint void ExpandPack() ETNoexceptHint{};

template <typename T>
ETConstexpr T Declval() ETNoexceptHint;

template <size_t Index, typename T, size_t Size>
ETConstexpr ETForceInlineHint const T& Get(const T (&elements)[Size]) ETNoexceptHint {
	static_assert(Index < Size, "compile-time array index out of bounds");
	return elements[Index];
}

template <size_t Index, typename T, size_t Size>
ETConstexpr ETForceInlineHint T&& Get(T(&&elements)[Size]) ETNoexceptHint {
	static_assert(Index < Size, "compile-time array index out of bounds");
	return elements[Index];
}

template <size_t Index, typename T, size_t Size>
ETConstexpr ETForceInlineHint T& Get(T (&elements)[Size]) ETNoexceptHint {
	static_assert(Index < Size, "compile-time array index out of bounds");
	return elements[Index];
}

#if ET_COMPILER_SUPPORTS_CPP11
template <typename T>
ETConstexpr ETForceInlineHint T&& Move(T& value) ETNoexceptHint {
	return static_cast<T&&>(value);
}

template <typename T>
ETConstexpr ETForceInlineHint T&& Forward(typename RemoveReference<T>::Type&& value) ETNoexceptHint {
	return static_cast<T&&>(value);
}

template <typename T>
ETConstexpr ETForceInlineHint T&& Forward(typename RemoveReference<T>::Type& value) ETNoexceptHint {
	return static_cast<T&&>(value);
}

template <typename... Ts>
ETConstexpr ETForceInlineHint void DiscardArguments(Ts&&... /*discarded*/) ETNoexceptHint {}

template <typename T>
using InitializerList = std::initializer_list<T>;
#endif

} // namespace Eldritch2

#define ETAlignOf(type) alignof(type)
#define ETAddressOf(value) __builtin_addressof(value)
#define ETCountOf(value) ::Eldritch2::TupleSize<typename ::Eldritch2::RemoveReference<decltype(value)>::Type>::Value
#define ETOffsetOf(structure, field) ((size_t)&reinterpret_cast<char const volatile&>((((structure*)0)->field)))

#define ET_TEMPLATE_SFINAE(...) typename = eastl::enable_if_t<__VA_ARGS__>
#define ET_TEMPLATE_SFINAE_PARAM typename

/*	Since the override specifiers and enum class language extensions are safely wrapped away in here, turn off the warnings.
 *	(4505) Some template classes will have unreferenced inline members. The optimizer/linker will strip these out, so we don't need to worry.
 *	(4127) Some libraries test compile-time constants against one another inside an if statement. The optimizer will realistically catch this and remove any runtime cost.
 *	(4510 4610 4512) Disable complaints about default constructors/assignment operators being inaccessible due to the noncopyable pattern;
 *	this will just result in an error if you try to do it. Why Microsoft thinks they need to warn you on top of this is baffling.
 *	(4324) Another annoying warning of questionable utility. This one is about the compiler doing what you asked it to re: data alignment.
 *	(6255) _alloca() usage is assumed to be the informed result of a deliberate tradeoff; we understand stack overflows are possible. */
ET_PUSH_MSVC_WARNING_STATE(disable : 4480 4481 4505 4127 4510 4610 4512 4324 6255)

//==================================================================//
// STATIC ANALYSIS
//==================================================================//
#if (ET_COMPILER_IS_MSVC)
#	include <sal.h>
#	define ETFormatStringHint(_String) __format_string _String
#else
#	define ETFormatStringHint(_String) _String
#endif
//------------------------------------------------------------------//
