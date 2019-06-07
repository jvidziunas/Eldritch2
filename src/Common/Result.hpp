/*==================================================================*\
  Result.hpp
  ------------------------------------------------------------------
  Purpose:
  Provides a definition/standard for an error code datatype, used
  for pinpointing failures and providing additional semantic
  information about root cause without paying the (high) cost of the
  standard C++ exception model.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

enum class Result : int {
	Timeout              = -8,
	UnsupportedOperation = -7,
	AccessDenied         = -6,
	InvalidObjectState   = -5,
	InvalidPath          = -4,
	InvalidParameter     = -3,
	OutOfMemory          = -2,
	Unspecified          = -1,
	Success              = 0
};

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint int AsPosixInt(Result) ETNoexceptHint;

ETConstexpr ETPureFunctionHint bool Succeeded(Result) ETNoexceptHint;

ETConstexpr ETPureFunctionHint bool Failed(Result) ETNoexceptHint;

} // namespace Eldritch2

#if !defined(ET_ABORT_IF)
#	define ET_ABORT_IF(condition, ...)                        \
		{                                                      \
			const auto FUNC_RESULT(condition);                 \
			if (ET_UNLIKELY(static_cast<bool>(FUNC_RESULT))) { \
				__VA_ARGS__;                                   \
				return FUNC_RESULT;                            \
			}                                                  \
		}
#endif
#if !defined(ET_ABORT_UNLESS)
#	define ET_ABORT_UNLESS(condition, ...)         \
		{                                           \
			const auto FUNC_RESULT(condition);      \
			if (ET_UNLIKELY(Failed(FUNC_RESULT))) { \
				__VA_ARGS__;                        \
				return FUNC_RESULT;                 \
			}                                       \
		}
#endif

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Result.inl>
//------------------------------------------------------------------//
