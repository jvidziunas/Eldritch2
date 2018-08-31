/*==================================================================*\
  Tuple.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <eastl/tuple.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename... Elements>
using Tuple = eastl::tuple<Elements...>;

template <size_t index, typename Tuple>
struct TupleElement {
	using Type = typename eastl::tuple_element<index, Tuple>::value;
};

} // namespace Eldritch2
