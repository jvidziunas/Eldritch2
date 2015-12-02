/*==================================================================*\
  StringHash.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Character>
	template <class Allocator>
	ETNoAliasHint size_t StringHash<Character>::operator()( const String<Character, Allocator>& string, const size_t seed ) const {
		return string.GetHashCode( seed );
	}

}	// namespace Eldritch2