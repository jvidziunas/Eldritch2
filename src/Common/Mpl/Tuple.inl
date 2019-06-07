/*==================================================================*\
  Tuple.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename... Types>
	ETConstexpr ETForceInlineHint Tuple<Types&...> Tie(Types&... types) ETNoexceptHint {
		return Tuple<Types&...>(types...);
	}

	// ---------------------------------------------------

	template <typename... Types>
	ETConstexpr ETForceInlineHint Tuple<TupleDecay<Types>...> MakeTuple(Types&&... types) {
		return Tuple<TupleDecay<Types>...>(Forward<Types>(types)...);
	}

	// ---------------------------------------------------

	template <typename... Types>
	ETConstexpr ETForceInlineHint Tuple<Types&&...> ForwardTuple(Types&&... types) ETNoexceptHint {
		return Tuple<Types&&...>(Forward<Types>(types)...);
	}

	// ---------------------------------------------------

	template <size_t I, typename... Types>
	ETConstexpr ETForceInlineHint TupleElement<I, Tuple<Types...>>& Get(Tuple<Types...>& tuple) ETNoexceptHint {
		return eastl::get<I>(tuple);
	}

	// ---------------------------------------------------

	template <size_t I, typename... Types>
	ETConstexpr ETForceInlineHint ConstTupleElement<I, Tuple<Types...>>& Get(const Tuple<Types...>& tuple) ETNoexceptHint {
		return eastl::get<I>(tuple);
	}

	// ---------------------------------------------------

	template <size_t I, typename... Types>
	ETConstexpr ETForceInlineHint TupleElement<I, Tuple<Types...>>&& Get(Tuple<Types...>&& tuple) ETNoexceptHint {
		return eastl::get<I>(Move(tuple));
	}

	// ---------------------------------------------------

	template <typename T, typename... Types>
	ETConstexpr ETForceInlineHint T& Get(Tuple<Types...>& tuple) ETNoexceptHint {
		return eastl::get<T>(tuple);
	}

	// ---------------------------------------------------

	template <typename T, typename... Types>
	ETConstexpr ETForceInlineHint const T& Get(const Tuple<Types...>& tuple) ETNoexceptHint {
		return eastl::get<T>(tuple);
	}

	// ---------------------------------------------------

	template <typename T, typename... Types>
	ETConstexpr ETForceInlineHint T&& Get(Tuple<Types...>&& tuple) ETNoexceptHint {
		return eastl::get<T>(Move(tuple));
	}

	// ---------------------------------------------------

	template <typename Tuple, size_t... Indices>
	ETConstexpr ETForceInlineHint void Swap(Tuple& lhs, Tuple& rhs, IndexSequence<Indices...>) {
		DiscardArguments((Swap(Get<Indices>(lhs), Get<Indices>(rhs)), 0)...);
	}

	// ---------------------------------------------------

	template <typename... Types>
	ETConstexpr ETForceInlineHint void Swap(Tuple<Types...>& lhs, Tuple<Types...>& rhs) {
		Swap(lhs, rhs, IndexSequenceFor<Types...>());
	}

	// ---------------------------------------------------

	template <typename... Tuples>
	ETConstexpr typename TupleCat<Tuples...>::ResultType Concatenate(Tuples&&... tuples) {
		return TupleCat<Tuples...>::DoCat(Move(tuples)...);
	}

}	// namespace Eldritch2
