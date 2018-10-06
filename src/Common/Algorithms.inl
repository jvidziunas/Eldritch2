/*==================================================================*\
  Algorithms.inl
  ------------------------------------------------------------------
  Purpose:
  Supplemental function collection designed to complement/augment
  the STL algorithms header.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#include <eastl/algorithm.h>
#include <eastl/sort.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename InputIterator, typename UnaryPredicate>
ETInlineHint ETForceInlineHint InputIterator FindIf(InputIterator begin, InputIterator end, UnaryPredicate condition) {
	return eastl::find_if<InputIterator, UnaryPredicate>(begin, end, eastl::move(condition));
}

// ---------------------------------------------------

template <typename InputIterator, typename Value>
ETInlineHint ETForceInlineHint InputIterator Find(InputIterator begin, InputIterator end, const Value& value) {
	return eastl::find<InputIterator, Value>(begin, end, value);
}

// ---------------------------------------------------

template <typename InputIterator, typename UnaryPredicate>
ETInlineHint ETForceInlineHint InputIterator FindIfNot(InputIterator begin, InputIterator end, UnaryPredicate condition) {
	return eastl::find_if_not<InputIterator, UnaryPredicate>(begin, end, eastl::move(condition));
}

// ---------------------------------------------------

template <typename InputIterator, typename UnaryPredicate>
ETInlineHint ETForceInlineHint UnaryPredicate ForEach(InputIterator begin, InputIterator end, UnaryPredicate predicate) {
	return eastl::for_each<InputIterator, UnaryPredicate>(begin, end, eastl::move(predicate));
}

// ---------------------------------------------------

template <typename InputIterator, typename Value, typename BinaryPredicate>
ETInlineHint ETForceInlineHint Value Reduce(InputIterator begin, InputIterator end, Value value, BinaryPredicate reducer) {
	while (begin != end) {
		value = reducer(begin++, value);
	}

	return value;
}

// ---------------------------------------------------

template <typename InputIterator, typename BinaryPredicate>
ETInlineHint ETForceInlineHint auto Reduce(InputIterator begin, InputIterator end, BinaryPredicate reducer) -> decltype(*begin) {
	using ValueType = typename eastl::iterator_traits<InputIterator>::value_type;
	return Eldritch2::Reduce(begin, end, ValueType {}, eastl::move(reducer));
}

// ---------------------------------------------------

template <typename InputIterator>
ETInlineHint ETForceInlineHint InputIterator Unique(InputIterator begin, InputIterator end) {
	return eastl::unique(begin, end, EqualTo<typename eastl::iterator_traits<InputIterator>::value_type>());
}

// ---------------------------------------------------

template <typename InputIterator, typename BinaryPredicate>
ETInlineHint ETForceInlineHint InputIterator Unique(InputIterator begin, InputIterator end, BinaryPredicate equal) {
	return eastl::unique<InputIterator, BinaryPredicate>(begin, end, eastl::move(equal));
}

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator, typename UnaryPredicate>
ETInlineHint ETForceInlineHint OutputIterator Transform(InputIterator begin, InputIterator end, OutputIterator output, UnaryPredicate transform) {
	return eastl::transform<InputIterator, OutputIterator, UnaryPredicate>(begin, end, output, eastl::move(transform));
}

// ---------------------------------------------------

template <typename InputIterator, typename InputIterator2, typename OutputIterator, typename BinaryPredicate>
ETInlineHint ETForceInlineHint OutputIterator Transform(InputIterator begin, InputIterator end, InputIterator2 begin2, OutputIterator output, BinaryPredicate transform) {
	return eastl::transform<InputIterator, InputIterator2, OutputIterator, BinaryPredicate>(begin, end, begin2, output, eastl::move(transform));
}

// ---------------------------------------------------

template <typename OutputIterator, typename Generator>
ETInlineHint ETForceInlineHint OutputIterator Generate(OutputIterator begin, OutputIterator end, Generator generator) {
	return eastl::generate<OutputIterator, Generator>(begin, end, eastl::move(generator));
}

// ---------------------------------------------------

template <typename OutputIterator, typename Generator>
ETInlineHint ETForceInlineHint OutputIterator Generate(OutputIterator begin, size_t count, Generator generator) {
	return eastl::generate_n<OutputIterator, size_t, Generator>(begin, count, eastl::move(generator));
}

// ---------------------------------------------------

template <typename ForwardIterator, typename UnaryPredicate>
ETInlineHint ETForceInlineHint ForwardIterator RemoveIf(ForwardIterator begin, ForwardIterator end, UnaryPredicate filter) {
	return eastl::remove_if<ForwardIterator, UnaryPredicate>(begin, end, eastl::move(filter));
}

// ---------------------------------------------------

template <typename ForwardIterator, typename Value>
ETInlineHint ETForceInlineHint ForwardIterator Remove(ForwardIterator begin, ForwardIterator end, const Value& value) {
	return eastl::remove<ForwardIterator, Value>(begin, end, value);
}

// ---------------------------------------------------

template <typename OutputIterator, typename Value>
ETInlineHint ETForceInlineHint void Fill(OutputIterator begin, OutputIterator end, const Value& value) {
	eastl::fill(begin, end, value);
}

// ---------------------------------------------------

template <typename InputIterator, typename ValueType, typename BinaryPredicate>
ETInlineHint ETForceInlineHint InputIterator LowerBound(InputIterator begin, InputIterator end, const ValueType& value, BinaryPredicate compare) {
	return eastl::lower_bound<InputIterator, ValueType, BinaryPredicate>(begin, end, value, eastl::move(compare));
}

// ---------------------------------------------------

template <typename InputIterator, typename ValueType, typename BinaryPredicate>
ETInlineHint ETForceInlineHint InputIterator UpperBound(InputIterator begin, InputIterator end, const ValueType& value, BinaryPredicate compare) {
	return eastl::upper_bound<InputIterator, ValueType, BinaryPredicate>(begin, end, value, eastl::move(compare));
}

// ---------------------------------------------------

template <typename InputIterator>
ETInlineHint ETForceInlineHint void Sort(InputIterator begin, InputIterator end) {
	eastl::sort(begin, end, LessThan<typename eastl::iterator_traits<InputIterator>::value_type>());
}

// ---------------------------------------------------

template <typename InputIterator, typename BinaryPredicate>
ETInlineHint ETForceInlineHint void Sort(InputIterator begin, InputIterator end, BinaryPredicate sort) {
	eastl::sort<InputIterator, BinaryPredicate>(begin, end, eastl::move(sort));
}

// ---------------------------------------------------

template <typename InputIterator>
ETInlineHint ETForceInlineHint void Destroy(InputIterator begin, InputIterator end) {
	eastl::for_each<InputIterator, InvokeDestructor>(begin, end, InvokeDestructor());
}

// ---------------------------------------------------

template <typename InputIterator>
ETInlineHint ETForceInlineHint void TrivialConstruct(InputIterator begin, InputIterator end) {
	eastl::for_each<InputIterator, InvokeTrivialConstructor>(begin, end, InvokeTrivialConstructor());
}

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator>
ETInlineHint ETForceInlineHint OutputIterator Copy(InputIterator begin, InputIterator end, OutputIterator output) {
	return eastl::copy<InputIterator, OutputIterator>(begin, end, output);
}

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator, typename Terminator>
ETInlineHint ETForceInlineHint OutputIterator CopyAndTerminate(InputIterator begin, InputIterator end, OutputIterator output, Terminator terminator) {
	auto result(Copy(begin, end, output));
	*result++ = eastl::move(terminator);

	return result;
}

// ---------------------------------------------------

template <typename Container>
ETInlineHint ETForceInlineHint typename Container::ConstIterator Begin(const Container& container) {
	return container.Begin();
}

// ---------------------------------------------------

template <typename Container>
ETInlineHint ETForceInlineHint typename Container::Iterator Begin(Container& container) {
	return container.End();
}

// ---------------------------------------------------

template <typename Value, size_t length>
ETCpp14Constexpr ETInlineHint ETForceInlineHint const Value* Begin(const Value (&array)[length]) ETNoexceptHint {
	return eastl::begin(array);
}

// ---------------------------------------------------

template <typename Value, size_t length>
ETCpp14Constexpr ETInlineHint ETForceInlineHint Value* Begin(Value (&array)[length]) ETNoexceptHint {
	return eastl::begin(array);
}

// ---------------------------------------------------

template <typename Container>
ETInlineHint ETForceInlineHint typename Container::ConstIterator End(const Container& container) {
	return container.End();
}

// ---------------------------------------------------

template <typename Container>
ETInlineHint ETForceInlineHint typename Container::Iterator End(Container& container) {
	return container.End();
}

// ---------------------------------------------------

template <typename Value, size_t length>
ETCpp14Constexpr ETInlineHint ETForceInlineHint const Value* End(const Value (&array)[length]) ETNoexceptHint {
	return eastl::end(array);
}

// ---------------------------------------------------

template <typename Value, size_t length>
ETCpp14Constexpr ETInlineHint ETForceInlineHint Value* End(Value (&array)[length]) ETNoexceptHint {
	return eastl::end(array);
}

// ---------------------------------------------------

template <typename T, size_t arraySize>
ETInlineHint ETForceInlineHint void Swap(T (&lhs)[arraySize], T (&rhs)[arraySize]) {
	for (size_t i(0u); i < arraySize; ++i) {
		Swap(lhs[i], rhs[i]);
	}
}

// ---------------------------------------------------

template <typename T>
ETInlineHint ETForceInlineHint void Swap(T& lhs, T& rhs) {
	auto _(eastl::move(lhs));
	lhs = eastl::move(rhs);
	rhs = eastl::move(_);
}

} // namespace Eldritch2
