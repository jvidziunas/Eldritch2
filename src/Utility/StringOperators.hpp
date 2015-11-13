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

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/StringOperators.inl>
//------------------------------------------------------------------//