/*==================================================================*\
  ErrorCode.inl
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
#include <Common/Containers/AbstractStringView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETConstexpr ETInlineHint ErrorCode::ErrorCode(Error bitfield) ETNoexceptHint : _value(int32(bitfield)) {}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint int AsPosixInt(ErrorCode code) ETNoexceptHint {
	return code._value;
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint bool Succeeded(ErrorCode code) ETNoexceptHint {
	return code._value == int32(Error::None);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint bool Failed(ErrorCode code) ETNoexceptHint {
	return code._value != int32(Error::None);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint bool operator==(ErrorCode code, ErrorCode code1) ETNoexceptHint {
	return code._value == code1._value;
}

} // namespace Eldritch2

namespace fmt {
template <typename Character>
struct formatter<::Eldritch2::ErrorCode, Character> {
	template <typename ParseContext>
	ETConstexpr auto parse(ParseContext& context) {
		return context.begin();
	}

	template <typename FormatContext>
	auto format(::Eldritch2::ErrorCode error, FormatContext& context) {
		static ETConstexpr ::Eldritch2::AbstractStringView<Character> strings[] = {
			"Operation successful",
			"Unspecified error",
			"Insufficient free memory",
			"Invalid parameter sent as argument to function",
			"Null pointer argument sent to function expecting a valid target",
			"Invalid or nonexistent file path",
			"Invalid object internal state",
			"Access denied",
			"Operation not supported for this object",
			"<Undefined>"
		};

		const auto string(strings[::Eldritch2::Min(-AsPosixInt(error), int(ETCountOf(strings)))]);
		return eastl::copy(string.Begin(), string.End(), context.begin());
	}
};
} // namespace fmt
