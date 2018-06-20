/*==================================================================*\
  TypeTraits.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename T, T v>
struct IntegralConstant {
	static constexpr T Value = v;

	using ValueType = T;
	using Type      = IntegralConstant<T, v>;

	constexpr operator ValueType() const ETNoexceptHint {
		return value;
	}
	constexpr ValueType operator()() const ETNoexceptHint {
		return value;
	}
};

using TrueType  = IntegralConstant<bool, true>;
using FalseType = IntegralConstant<bool, false>;

} // namespace Eldritch2
