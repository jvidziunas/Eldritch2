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
#include <Common/Tuple.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

struct HasDistinctSimdCall : public IntegralConstant<bool, !eastl::is_same<void (*)(), void(ETSimdCall*)()>::value> {};

template <typename Function, bool enabled = true>
struct FunctionTraits;

// ---

template <typename Return, typename... Arguments>
struct FunctionTraits<Return(Arguments...), true> {
	template <size_t index>
	using ArgumentType   = typename TupleElement<index, Tuple<Arguments...>>::Type;
	using ArgumentsTuple = Tuple<Arguments...>;
	using SignatureType  = Return (*)(Arguments...);
	using ReturnType     = Return;

	static ETConstexpr size_t Arity         = IntegralConstant<size_t, sizeof...(Arguments)>::Value;
	static ETConstexpr bool   IsConstMethod = false;
};

// ---

template <typename Return, typename... Arguments>
struct FunctionTraits<Return (*)(Arguments...), true> : public FunctionTraits<Return(Arguments...)> {};

template <typename Return, typename... Arguments>
struct FunctionTraits<Return(ETSimdCall*)(Arguments...), HasDistinctSimdCall::Value> : public FunctionTraits<Return(Arguments...)> {
	using SignatureType = Return(ETSimdCall*)(Arguments...);
};

template <typename Return, class Class, typename... Arguments>
struct FunctionTraits<Return (Class::*)(Arguments...), true> : public FunctionTraits<Return(Arguments...)> {
	using SignatureType = Return (Class::*)(Arguments...);
	using ClassType     = Class;
};

template <typename Return, class Class, typename... Arguments>
struct FunctionTraits<Return (ETSimdCall Class::*)(Arguments...), HasDistinctSimdCall::Value> : public FunctionTraits<Return (Class::*)(Arguments...)> {
	using SignatureType = Return (ETSimdCall Class::*)(Arguments...);
};

template <typename Return, class Class, typename... Arguments>
struct FunctionTraits<Return (Class::*)(Arguments...) const, true> : public FunctionTraits<Return (Class::*)(Arguments...)> {
	static ETConstexpr bool IsConstMethod = true;

	using SignatureType = Return (Class::*)(Arguments...) const;
	using ClassType     = Class;
};

template <typename Return, class Class, typename... Arguments>
struct FunctionTraits<Return (ETSimdCall Class::*)(Arguments...) const, HasDistinctSimdCall::Value> : public FunctionTraits<Return (Class::*)(Arguments...) const> {
	using SignatureType = Return (ETSimdCall Class::*)(Arguments...) const;
};

} // namespace Eldritch2
