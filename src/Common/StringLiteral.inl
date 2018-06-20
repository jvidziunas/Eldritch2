/*==================================================================*\
  StringLiteral.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory.hpp>
//------------------------------------------------------------------//

#if defined(CompareString)
#	undef CompareString
#endif

namespace Eldritch2 {

template <typename Character>
template <size_t length>
ETInlineHint StringLiteral<Character>::StringLiteral(const Character (&string)[length]) :
	_string(string) {}

// ---------------------------------------------------

template <typename Character>
ETInlineHint StringLiteral<Character>::StringLiteral(const Character* const string) :
	_string(string) {}

// ---------------------------------------------------

template <typename Character>
ETInlineHint StringLiteral<Character>::StringLiteral() :
	_string("") {}

// ---------------------------------------------------

template <typename Character>
ETInlineHint StringLiteral<Character>::operator const Character*() const {
	return _string;
}

// ---------------------------------------------------

template <typename Character>
ETPureFunctionHint ETInlineHint bool operator==(const StringLiteral<Character>& lhs, const StringLiteral<Character>& rhs) {
	return StringsEqual(lhs, rhs);
}

// ---------------------------------------------------

template <typename Character>
ETPureFunctionHint ETInlineHint bool operator!=(const StringLiteral<Character>& lhs, const StringLiteral<Character>& rhs) {
	return !StringsEqual(lhs, rhs);
}

// ---------------------------------------------------

template <typename Character>
ETPureFunctionHint ETInlineHint bool operator<(const StringLiteral<Character>& lhs, const StringLiteral<Character>& rhs) {
	return OrderStrings(lhs, rhs) < 0;
}

// ---------------------------------------------------

template <typename Stream, typename Character>
ETPureFunctionHint ETInlineHint auto operator<<(Stream& stream, const StringLiteral<Character>& string) -> decltype(stream << static_cast<const Character*>(string)) {
	return stream << static_cast<const Character*>(string);
}

// ---------------------------------------------------

template <typename Character>
ETPureFunctionHint ETInlineHint size_t GetHashCode(StringLiteral<Character> string, size_t seed) {
	return HashMemory(string, StringLength(string) * sizeof(Character), seed);
}

} // namespace Eldritch2
