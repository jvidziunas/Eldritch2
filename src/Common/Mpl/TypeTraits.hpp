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

template <typename T, T value>
struct IntegralConstant {
public:
	using ValueType = T;
	using Type      = IntegralConstant<T, value>;

	static ETConstexpr ValueType Value = value;

	ETConstexpr operator ValueType() const ETNoexceptHint {
		return Value;
	}

	ETConstexpr ValueType operator()() const ETNoexceptHint {
		return Value;
	}
};

using TrueType  = IntegralConstant<bool, true>;
using FalseType = IntegralConstant<bool, false>;

} // namespace Eldritch2
