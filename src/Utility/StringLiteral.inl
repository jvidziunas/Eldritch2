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
#include <Utility/Memory/StandardLibrary.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Character>
	template <size_t stringLength>
	ETForceInlineHint StringLiteral<Character>::StringLiteral( const Character (&literal)[stringLength] ) : _string( literal ) {}

// ---------------------------------------------------

	template <typename Character>
	ETForceInlineHint StringLiteral<Character>::StringLiteral( const Character* const literal ) : _string( literal ) {}

// ---------------------------------------------------

	template <typename Character>
	ETForceInlineHint StringLiteral<Character>::StringLiteral() : _string( "" ) {}

// ---------------------------------------------------

	template <typename Character>
	ETForceInlineHint StringLiteral<Character>::operator const Character*() const {
		return _string;
	}

// ---------------------------------------------------

	template <typename Character>
	ETPureFunctionHint ETForceInlineHint bool operator==( const StringLiteral<Character>& string0, const StringLiteral<Character>& string1 ) {
		return Eldritch2::EqualityCompareString( string0, string1 );
	}

// ---------------------------------------------------

	template <typename Character>
	ETPureFunctionHint ETForceInlineHint bool operator!=( const StringLiteral<Character>& string0, const StringLiteral<Character>& string1 ) {
		return !Eldritch2::EqualityCompareString( string0, string1 );
	}

// ---------------------------------------------------

	template <typename Character>
	ETPureFunctionHint ETForceInlineHint bool operator<( const StringLiteral<Character>& string0, const StringLiteral<Character>& string1 ) {
		return Eldritch2::CompareString( string0, string1 ) < 0;
	}

// ---------------------------------------------------

	template <typename Stream, typename Character>
	ETPureFunctionHint ETForceInlineHint auto operator<<( Stream& stream, const StringLiteral<Character>& string ) -> decltype(stream << static_cast<const Character*>(string)) {
		return stream << static_cast<const Character*>( string );
	}

// ---------------------------------------------------

	template <typename Character>
	ETPureFunctionHint ETForceInlineHint size_t GetHashCode( StringLiteral<Character> string, size_t seed ) {
		return Eldritch2::HashMemory( string, Eldritch2::StringLength( string ) * sizeof(Character), seed );
	}

}	// namespace Eldritch2

