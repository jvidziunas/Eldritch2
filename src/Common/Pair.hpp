/*==================================================================*\
  Pair.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <eastl/utility.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename T1, typename T2>
using Pair = eastl::pair<T1, T2>;

template <typename T1, typename T2>
ETInlineHint ETForceInlineHint size_t GetHashCode(const Pair<T1, T2>& pair, size_t seed = 0u) {
	using ::Eldritch2::GetHashCode;
	return GetHashCode(pair.second, GetHashCode(pair.first, seed));
}

} // namespace Eldritch2
