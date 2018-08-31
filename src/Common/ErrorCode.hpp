/*==================================================================*\
  ErrorCode.hpp
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
#include <Common/Mpl/CharTypes.hpp>
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
template <typename Character>
class AbstractStringView;
}

namespace Eldritch2 {

enum class Error : int32 {
	Timeout              = -8,
	UnsupportedOperation = -7,
	AccessDenied         = -6,
	InvalidObjectState   = -5,
	InvalidFileName      = -4,
	InvalidParameter     = -3,
	OutOfMemory          = -2,
	Unspecified          = -1,
	None                 = 0
};

// ---------------------------------------------------

class ErrorCode {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ErrorCode instance.
	ETConstexpr ErrorCode(Error bitfield = Error::None) ETNoexceptHint;
	//!	Constructs this @ref ErrorCode instance.
	ETConstexpr ErrorCode(const ErrorCode&) ETNoexceptHint = default;

	~ErrorCode() = default;

	// - ASSIGNMENT --------------------------------------

public:
	ETConstexpr ErrorCode& operator=(const ErrorCode&) ETNoexceptHint = default;

	// - DEBUG/LOGGING INFORMATION -----------------------

	friend ETConstexpr ETPureFunctionHint int AsPosixInt(ErrorCode code) ETNoexceptHint;

	friend ETConstexpr ETPureFunctionHint bool Succeeded(ErrorCode code) ETNoexceptHint;

	friend ETConstexpr ETPureFunctionHint bool Failed(ErrorCode code) ETNoexceptHint;

	friend ETConstexpr ETPureFunctionHint bool operator==(ErrorCode code, ErrorCode code1) ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	int32 _value;
};

} // namespace Eldritch2

#if !defined(ET_ABORT_UNLESS)
#	define ET_ABORT_UNLESS(condition)         \
		{                                      \
			const auto result(condition);      \
			if (ET_UNLIKELY(Failed(result))) { \
				return result;                 \
			}                                  \
		}
#endif

#if !defined(ET_ABORT_UNLESS_MSG)
#	define ET_ABORT_UNLESS_MSG(condition, log, ...)                           \
		{                                                                      \
			const auto result(condition);                                      \
			if (ET_UNLIKELY(Failed(result))) {                                 \
				log.Write(::Eldritch2::Logging::Severity::Error, __VA_ARGS__); \
				return result;                                                 \
			}                                                                  \
		}
#endif

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/ErrorCode.inl>
//------------------------------------------------------------------//
