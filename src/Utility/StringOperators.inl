/*==================================================================*\
  StringOperators.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	template <typename Character>
	ETNoAliasHint bool StringComparator<Character>::operator()( const Character* left, const Character* right ) const {
		return ::Eldritch2::CompareString( left, right ) < 0;
	}

// ---------------------------------------------------

	template <typename Character>
	ETNoAliasHint bool StringEqualComparator<Character>::operator()( const Character* left, const Character* right ) const {
		return ::Eldritch2::EqualityCompareString( left, right );
	}

}	// namespace Utility
}	// namespace Eldritch2