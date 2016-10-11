/*==================================================================*\
  Pair.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Hash.hpp>
#include <eastl/utility.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename T1, typename T2>
	using Pair = eastl::pair<T1, T2>;

	template <typename T1, typename T2>
	ETInlineHint ETPureFunctionHint size_t GetHashCode( const Eldritch2::Pair<T1, T2>& pair, size_t seed = 0u ) {
		return Eldritch2::Hash<T2>()( pair.second, Eldritch2::Hash<T1>()( pair.first, seed ) );
	}

}	// namespace Eldritch2