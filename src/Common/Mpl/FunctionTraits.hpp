/*==================================================================*\
  FunctionTraits.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/TypeTraits.hpp>
#include <Common/Mpl/Tuple.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Function, bool Enabled = true>
struct FunctionTraits;

// ---

template <typename Return, typename... Arguments>
struct FunctionTraits<Return(Arguments...), true> {
	static ETConstexpr size_t Arity         = IntegralConstant<size_t, sizeof...(Arguments)>::Value;
	static ETConstexpr bool   IsConstMethod = false;

	using ArgumentsType = Tuple<Arguments...>;
	using ReturnType    = Return;
	using Type          = Return (*)(Arguments...);
};

template <typename Return, typename... Arguments>
struct FunctionTraits<Return (*)(Arguments...), true> : public FunctionTraits<Return(Arguments...)> {};

template <typename Return, class Class, typename... Arguments>
struct FunctionTraits<Return (Class::*)(Arguments...), true> : public FunctionTraits<Return(Arguments...)> {
	using ClassType = Class;
	using Type      = Return (Class::*)(Arguments...);
};

template <typename Return, class Class, typename... Arguments>
struct FunctionTraits<Return (Class::*)(Arguments...) const, true> : public FunctionTraits<Return (Class::*)(Arguments...)> {
	static ETConstexpr bool IsConstMethod = true;

	using Type = Return (Class::*)(Arguments...) const;
};

template <typename Return, typename... Arguments>
struct FunctionTraits<Return(ETSimdCall*)(Arguments...), IsDistinct<void (*)(), void(ETSimdCall*)()>::Value> : public FunctionTraits<Return(Arguments...)> {
	using Type = Return(ETSimdCall*)(Arguments...);
};

template <typename Return, class Class, typename... Arguments>
struct FunctionTraits<Return (ETSimdCall Class::*)(Arguments...), IsDistinct<void (*)(), void(ETSimdCall*)()>::Value> : public FunctionTraits<Return (Class::*)(Arguments...)> {
	using Type = Return (ETSimdCall Class::*)(Arguments...);
};

template <typename Return, class Class, typename... Arguments>
struct FunctionTraits<Return (ETSimdCall Class::*)(Arguments...) const, IsDistinct<void (*)(), void(ETSimdCall*)()>::Value> : public FunctionTraits<Return (Class::*)(Arguments...) const> {
	using Type = Return (ETSimdCall Class::*)(Arguments...) const;
};

} // namespace Eldritch2
