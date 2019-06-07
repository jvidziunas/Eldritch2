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
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#include <eastl/tuple.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename... Types>
using Tuple = eastl::tuple<Types...>;

template <typename... Types>
struct TupleSize<Tuple<Types...>> : public IntegralConstant<size_t, sizeof...(Types)> {};

template <size_t Index, typename Tuple>
using TupleElement = typename eastl::tuple_element_t<Index, Tuple>;

template <size_t Index, typename Tuple>
using ConstTupleElement = typename eastl::const_tuple_element_t<Index, Tuple>;

template <typename... Tuples>
using TupleCat = eastl::Internal::TupleCat<Tuples...>;

template <typename T>
using TupleDecay = eastl::decay_t<T>;

static const decltype(eastl::ignore) ignore;

template <typename... Types>
ETConstexpr Tuple<Types&...> Tie(Types&...) ETNoexceptHint;

template <typename... Types>
ETConstexpr Tuple<Types&&...> ForwardTuple(Types&&...) ETNoexceptHint;

template <typename... Types>
ETConstexpr Tuple<TupleDecay<Types>...> MakeTuple(Types&&...);

template <size_t I, typename... Types>
ETConstexpr TupleElement<I, Tuple<Types...>>& Get(Tuple<Types...>&) ETNoexceptHint;

template <size_t I, typename... Types>
ETConstexpr ConstTupleElement<I, Tuple<Types...>>& Get(const Tuple<Types...>&) ETNoexceptHint;

template <size_t I, typename... Types>
ETConstexpr TupleElement<I, Tuple<Types...>>&& Get(Tuple<Types...>&&) ETNoexceptHint;

template <typename T, typename... Types>
ETConstexpr T& Get(Tuple<Types...>&) ETNoexceptHint;

template <typename T, typename... Types>
ETConstexpr const T& Get(const Tuple<Types...>&) ETNoexceptHint;

template <typename T, typename... Types>
ETConstexpr T&& Get(Tuple<Types...>&&) ETNoexceptHint;

template <typename... Types>
ETConstexpr void Swap(Tuple<Types...>&, Tuple<Types...>&);

template <typename... Tuples>
ETConstexpr typename TupleCat<Tuples...>::ResultType Concatenate(Tuples&&... tuples);

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Mpl/Tuple.inl>
//------------------------------------------------------------------//
