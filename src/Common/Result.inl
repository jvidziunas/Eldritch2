/*==================================================================*\
  Result.inl
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
#include <Common/Containers/AbstractStringSpan.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETConstexpr ETForceInlineHint ETPureFunctionHint int AsPosixInt(Result code) ETNoexceptHint {
	return int(code);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint bool Succeeded(Result code) ETNoexceptHint {
	return code != Result::Success;
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint bool Failed(Result code) ETNoexceptHint {
	return !Succeeded(code);
}

} // namespace Eldritch2

namespace fmt {

template <typename Character>
struct formatter<Eldritch2::Result, Character> : public formatter<Eldritch2::AbstractStringSpan<Eldritch2::Utf8Char>, Character> {
public:
	using StringFormatter = formatter<Eldritch2::AbstractStringSpan<Eldritch2::Utf8Char>, Character>;

	ETConstexpr ETForceInlineHint ETPureFunctionHint Eldritch2::AbstractStringSpan<Eldritch2::Utf8Char> AsString(Eldritch2::Result error) ETNoexceptHint {
		switch (error) {
		case Eldritch2::Result::Timeout: return "operation timed out";
		case Eldritch2::Result::UnsupportedOperation: return "operation not supported for this object";
		case Eldritch2::Result::AccessDenied: return "access denied";
		case Eldritch2::Result::InvalidObjectState: return "invalid object internal state";
		case Eldritch2::Result::InvalidPath: return "invalid/nonexistent file path";
		case Eldritch2::Result::InvalidParameter: return "invalid function argument";
		case Eldritch2::Result::OutOfMemory: return "insufficient free memory";
		case Eldritch2::Result::Unspecified: return "unspecified error";
		case Eldritch2::Result::Success: return "operation succeeded";
		};
	}

	template <typename FormatContext>
	ETForceInlineHint auto format(Eldritch2::Result error, FormatContext& context) {
		return StringFormatter::format(AsString(error), context);
	}
};

} // namespace fmt
