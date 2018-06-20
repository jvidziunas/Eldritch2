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
//------------------------------------------------------------------//
#include <eastl/tuple.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Function>
struct FunctionTraits;

// ---

template <typename Return, typename... Arguments>
struct FunctionTraits<Return(Arguments...)> {
	static constexpr size_t Arity         = IntegralConstant<size_t, sizeof...(Arguments)>::Value;
	static constexpr bool   IsConstMethod = false;

	template <size_t index>
	using ArgumentType = typename eastl::tuple_element<index, eastl::tuple<Arguments...>>::type;
	using ReturnType   = Return;
};

// ---

template <typename Return, typename... Arguments>
struct FunctionTraits<Return (*)(Arguments...)> : public FunctionTraits<Return(Arguments...)> {
	using NativeSignatureType = Return (*)(Arguments...);
};

//	Specialization for member functions.
template <typename Return, class Class, typename... Arguments>
struct FunctionTraits<Return (Class::*)(Arguments...)> : public FunctionTraits<Return(Arguments...)> {
	using NativeSignatureType = Return (Class::*)(Arguments...);
	using ClassType           = Class;
};

//	Specialization for const member functions.
template <typename Return, class Class, typename... Arguments>
struct FunctionTraits<Return (Class::*)(Arguments...) const> : public FunctionTraits<Return (Class::*)(Arguments...)> {
	static constexpr bool IsConstMethod = true;

	using NativeSignatureType = Return (Class::*)(Arguments...) const;
	using ClassType           = Class;
};

#if ET_COMPILER_IS_MSVC
//	Specialization for SimdCall functions.
template <typename Return, typename... Arguments>
struct FunctionTraits<Return(ETSimdCall*)(Arguments...)> : public FunctionTraits<Return(Arguments...)> {
	using NativeSignatureType = Return(ETSimdCall*)(Arguments...);
};

//	Specialization for SimdCall member functions.
template <typename Return, class Class, typename... Arguments>
struct FunctionTraits<Return (ETSimdCall Class::*)(Arguments...)> : public FunctionTraits<Return (Class::*)(Arguments...)> {
	using NativeSignatureType = Return (ETSimdCall Class::*)(Arguments...);
};

//	Specialization for SimdCall const member functions.
template <typename Return, class Class, typename... Arguments>
struct FunctionTraits<Return (ETSimdCall Class::*)(Arguments...) const> : public FunctionTraits<Return (Class::*)(Arguments...) const> {
	using NativeSignatureType = Return (ETSimdCall Class::*)(Arguments...) const;
};
#endif // if ET_COMPILER_IS_MSVC

} // namespace Eldritch2
