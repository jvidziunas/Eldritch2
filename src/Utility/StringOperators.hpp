/*==================================================================*\
  StringOperators.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	template <typename Character = char>
	struct StringComparator {
		ETNoAliasHint bool	operator()( const Character* left, const Character* right ) const;
	};

// ---------------------------------------------------

	template <typename Character = char>
	struct StringEqualComparator {
		ETNoAliasHint bool	operator()( const Character* left, const Character* right ) const;
	};

// ---------------------------------------------------

	struct StringHash {
		ETInlineHint ETNoAliasHint size_t	operator()( const ::Eldritch2::UTF8Char* string, const size_t seed = 0u ) const;
		ETInlineHint ETNoAliasHint size_t	operator()( const ::Eldritch2::UTF8Char* string, const ::Eldritch2::UTF8Char* stringEnd, const size_t seed = 0u ) const;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/StringOperators.inl>
//------------------------------------------------------------------//