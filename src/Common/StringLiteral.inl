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

#if defined( CompareString )
#	undef CompareString
#endif

namespace Eldritch2 {

	template <typename Character>
	template <size_t stringLength>
	ETInlineHint StringLiteral<Character>::StringLiteral( const Character (&literal)[stringLength] ) : _string( literal ) {}

// ---------------------------------------------------

	template <typename Character>
	ETInlineHint StringLiteral<Character>::StringLiteral( const Character* const literal ) : _string( literal ) {}

// ---------------------------------------------------

	template <typename Character>
	ETInlineHint StringLiteral<Character>::StringLiteral() : _string( "" ) {}

// ---------------------------------------------------

	template <typename Character>
	ETInlineHint StringLiteral<Character>::operator const Character*() const {
		return _string;
	}

// ---------------------------------------------------

	template <typename Character>
	ETPureFunctionHint ETInlineHint bool operator==( const StringLiteral<Character>& string0, const StringLiteral<Character>& string1 ) {
		return StringsEqual( string0, string1 );
	}

// ---------------------------------------------------

	template <typename Character>
	ETPureFunctionHint ETInlineHint bool operator!=( const StringLiteral<Character>& string0, const StringLiteral<Character>& string1 ) {
		return !StringsEqual( string0, string1 );
	}

// ---------------------------------------------------

	template <typename Character>
	ETPureFunctionHint ETInlineHint bool operator<( const StringLiteral<Character>& string0, const StringLiteral<Character>& string1 ) {
		return OrderStrings( string0, string1 ) < 0;
	}

// ---------------------------------------------------

	template <typename Stream, typename Character>
	ETPureFunctionHint ETInlineHint auto operator<<( Stream& stream, const StringLiteral<Character>& string ) -> decltype(stream << static_cast<const Character*>(string)) {
		return stream << static_cast<const Character*>( string );
	}

// ---------------------------------------------------

	template <typename Character>
	ETPureFunctionHint ETInlineHint size_t GetHashCode( StringLiteral<Character> string, size_t seed ) {
		return HashMemory( string, StringLength( string ) * sizeof(Character), seed );
	}

}	// namespace Eldritch2

