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
namespace Detail {

	template <typename UnaryPredicate, typename Tuple, size_t... Indices>
	ETInlineHint ETForceInlineHint UnaryPredicate ForEach(const Tuple& tuple, IndexSequence<Indices...>, UnaryPredicate predicate) {
		DiscardArguments((predicate(Get<Indices>(tuple)), 0)...);
		return Move(predicate);
	}

	// ---------------------------------------------------

	template <typename UnaryPredicate, typename Tuple, size_t... Indices>
	ETInlineHint ETForceInlineHint UnaryPredicate ForEach(Tuple&& tuple, IndexSequence<Indices...>, UnaryPredicate predicate) {
		DiscardArguments((predicate(Get<Indices>(Move(tuple))), 0)...);
		return Move(predicate);
	}

	// ---------------------------------------------------

	template <typename UnaryPredicate, typename Tuple, size_t... Indices>
	ETInlineHint ETForceInlineHint UnaryPredicate ForEach(Tuple& tuple, IndexSequence<Indices...>, UnaryPredicate predicate) {
		DiscardArguments((predicate(Get<Indices>(tuple)), 0)...);
		return Move(predicate);
	}

} // namespace Detail

template <typename InputIterator, typename UnaryPredicate>
ETInlineHint ETForceInlineHint InputIterator FindIf(InputIterator begin, InputIterator end, UnaryPredicate condition) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, typename DereferenceType<InputIterator>::Type>()) {
	return eastl::find_if<InputIterator, UnaryPredicate>(Move(begin), Move(end), Move(condition));
}

// ---------------------------------------------------

template <typename InputIterator, typename Value>
ETInlineHint ETForceInlineHint InputIterator Find(InputIterator begin, InputIterator end, const Value& value) ETNoexceptHintIf(IsNoThrowCallable<EqualTo<Value>, const Value&, typename DereferenceType<InputIterator>::Type>()) {
	return eastl::find<InputIterator, Value>(Move(begin), Move(end), value, EqualTo<Value>());
}

// ---------------------------------------------------

template <typename InputIterator, typename UnaryPredicate>
ETInlineHint ETForceInlineHint InputIterator FindIfNot(InputIterator begin, InputIterator end, UnaryPredicate condition) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, typename DereferenceType<InputIterator>::Type>()) {
	return eastl::find_if_not<InputIterator, UnaryPredicate>(Move(begin), Move(end), Move(condition));
}

// ---------------------------------------------------

template <typename InputIterator, typename UnaryPredicate>
ETInlineHint ETForceInlineHint bool All(InputIterator begin, InputIterator end, UnaryPredicate condition) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, typename DereferenceType<InputIterator>::Type>()) {
	for (; begin != end; begin++) {
		if (!condition(*begin)) {
			return false;
		}
	}

	return true;
}

// ---------------------------------------------------

template <typename InputIterator, typename UnaryPredicate>
ETInlineHint ETForceInlineHint bool Any(InputIterator begin, InputIterator end, UnaryPredicate condition) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, typename DereferenceType<InputIterator>::Type>()) {
	for (; begin != end; begin++) {
		if (condition(*begin)) {
			return true;
		}
	}

	return false;
}

// ---------------------------------------------------

template <typename InputIterator, typename UnaryPredicate>
ETInlineHint ETForceInlineHint UnaryPredicate ForEach(InputIterator begin, InputIterator end, UnaryPredicate predicate) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, typename DereferenceType<InputIterator>::Type>()) {
	return eastl::for_each<InputIterator, UnaryPredicate>(Move(begin), Move(end), Move(predicate));
}

// ---------------------------------------------------

template <typename InputIterator, typename InputIterator2, typename BinaryPredicate>
ETInlineHint ETForceInlineHint BinaryPredicate ForEach(InputIterator begin, InputIterator end, InputIterator2 begin2, BinaryPredicate predicate) ETNoexceptHintIf(IsNoThrowCallable<BinaryPredicate, typename DereferenceType<InputIterator>::Type, typename DereferenceType<InputIterator2>::Type>()) {
	while (begin != end) {
		predicate(*begin++, *begin2++);
	}

	return Move(predicate);
}

// ---------------------------------------------------

template <typename Tuple, typename UnaryPredicate>
ETInlineHint ETForceInlineHint UnaryPredicate ForEach(const Tuple& tuple, UnaryPredicate predicate) {
	return Detail::ForEach(tuple, MakeIntegerSequence<size_t, ETCountOf(tuple)>(), Move(predicate));
}

// ---------------------------------------------------

template <typename Tuple, typename UnaryPredicate>
ETInlineHint ETForceInlineHint UnaryPredicate ForEach(Tuple&& tuple, UnaryPredicate predicate) {
	return Detail::ForEach(Move(tuple), MakeIntegerSequence<size_t, ETCountOf(tuple)>(), Move(predicate));
}

// ---------------------------------------------------

template <typename Tuple, typename UnaryPredicate>
ETInlineHint ETForceInlineHint UnaryPredicate ForEach(Tuple& tuple, UnaryPredicate predicate) {
	return Detail::ForEach(tuple, MakeIntegerSequence<size_t, ETCountOf(tuple)>(), Move(predicate));
}

// ---------------------------------------------------

template <typename InputIterator, typename Value, typename BinaryPredicate>
ETInlineHint ETForceInlineHint Value Reduce(InputIterator begin, InputIterator end, Value value, BinaryPredicate reducer) {
	while (begin != end) {
		value = reducer(*begin++, value);
	}

	return Move(value);
}

// ---------------------------------------------------

template <typename InputIterator, typename BinaryPredicate>
ETInlineHint ETForceInlineHint typename DereferenceType<InputIterator>::Type Reduce(InputIterator begin, InputIterator end, BinaryPredicate reducer) {
	using ValueType = typename eastl::iterator_traits<InputIterator>::value_type;
	return Eldritch2::Reduce(Move(begin), Move(end), ValueType{}, Move(reducer));
}

// ---------------------------------------------------

template <typename InputIterator>
ETInlineHint ETForceInlineHint InputIterator Unique(InputIterator begin, InputIterator end) {
	return eastl::unique<InputIterator>(Move(begin), Move(end), EqualTo<typename eastl::iterator_traits<InputIterator>::value_type>());
}

// ---------------------------------------------------

template <typename InputIterator, typename BinaryPredicate>
ETInlineHint ETForceInlineHint InputIterator Unique(InputIterator begin, InputIterator end, BinaryPredicate equal) {
	return eastl::unique<InputIterator, BinaryPredicate>(Move(begin), Move(end), Move(equal));
}

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator, typename UnaryPredicate>
ETInlineHint ETForceInlineHint OutputIterator Transform(InputIterator begin, InputIterator end, OutputIterator output, UnaryPredicate transform) {
	return eastl::transform<InputIterator, OutputIterator, UnaryPredicate>(Move(begin), Move(end), Move(output), Move(transform));
}

// ---------------------------------------------------

template <typename InputIterator, typename InputIterator2, typename OutputIterator, typename BinaryPredicate>
ETInlineHint ETForceInlineHint OutputIterator Transform(InputIterator begin, InputIterator end, InputIterator2 begin2, OutputIterator output, BinaryPredicate transform) {
	return eastl::transform<InputIterator, InputIterator2, OutputIterator, BinaryPredicate>(Move(begin), Move(end), Move(begin2), Move(output), Move(transform));
}

// ---------------------------------------------------

template <typename OutputIterator, typename Generator>
ETInlineHint ETForceInlineHint OutputIterator Generate(OutputIterator begin, OutputIterator end, Generator generator) {
	return eastl::generate<OutputIterator, Generator>(Move(begin), Move(end), Move(generator));
}

// ---------------------------------------------------

template <typename OutputIterator, typename Size, typename Generator>
ETInlineHint ETForceInlineHint OutputIterator GenerateN(OutputIterator begin, Size count, Generator generator) {
	return eastl::generate_n<OutputIterator, Size, Generator>(Move(begin), Move(count), Move(generator));
}

// ---------------------------------------------------

template <typename ForwardIterator, typename UnaryPredicate>
ETInlineHint ETForceInlineHint ForwardIterator RemoveIf(ForwardIterator begin, ForwardIterator end, UnaryPredicate filter) {
	return eastl::remove_if<ForwardIterator, UnaryPredicate>(Move(begin), Move(end), Move(filter));
}

// ---------------------------------------------------

template <typename ForwardIterator, typename Value>
ETInlineHint ETForceInlineHint ForwardIterator Remove(ForwardIterator begin, ForwardIterator end, const Value& value) {
	return eastl::remove<ForwardIterator, Value>(Move(begin), Move(end), value);
}

// ---------------------------------------------------

template <typename OutputIterator, typename Value>
ETInlineHint ETForceInlineHint void Fill(OutputIterator begin, OutputIterator end, const Value& value) {
	eastl::fill<OutputIterator, Value>(Move(begin), Move(end), value);
}

// ---------------------------------------------------

template <typename InputIterator, typename ValueType, typename BinaryPredicate>
ETInlineHint ETForceInlineHint InputIterator LowerBound(InputIterator begin, InputIterator end, const ValueType& value, BinaryPredicate compare) {
	return eastl::lower_bound<InputIterator, ValueType, BinaryPredicate>(Move(begin), Move(end), value, Move(compare));
}

// ---------------------------------------------------

template <typename InputIterator, typename ValueType, typename BinaryPredicate>
ETInlineHint ETForceInlineHint InputIterator UpperBound(InputIterator begin, InputIterator end, const ValueType& value, BinaryPredicate compare) {
	return eastl::upper_bound<InputIterator, ValueType, BinaryPredicate>(Move(begin), Move(end), value, Move(compare));
}

// ---------------------------------------------------

template <typename InputIterator>
ETInlineHint ETForceInlineHint void Sort(InputIterator begin, InputIterator end) {
	eastl::sort(begin, end, LessThan<typename eastl::iterator_traits<InputIterator>::value_type>());
}

// ---------------------------------------------------

template <typename InputIterator, typename BinaryPredicate>
ETInlineHint ETForceInlineHint void Sort(InputIterator begin, InputIterator end, BinaryPredicate sort) {
	eastl::sort<InputIterator, BinaryPredicate>(Move(begin), Move(end), Move(sort));
}

// ---------------------------------------------------

template <typename InputIterator>
ETInlineHint ETForceInlineHint void Destroy(InputIterator begin, InputIterator end) {
	eastl::for_each<InputIterator, InvokeDestructor>(Move(begin), Move(end), InvokeDestructor());
}

// ---------------------------------------------------

template <typename InputIterator>
ETInlineHint ETForceInlineHint void TrivialConstruct(InputIterator begin, InputIterator end) {
	eastl::for_each<InputIterator, InvokeTrivialConstructor>(Move(begin), Move(end), InvokeTrivialConstructor());
}

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator>
ETInlineHint ETForceInlineHint OutputIterator Copy(InputIterator begin, InputIterator end, OutputIterator output) {
	return eastl::copy<InputIterator, OutputIterator>(Move(begin), Move(end), Move(output));
}

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator, typename Terminator>
ETInlineHint ETForceInlineHint OutputIterator CopyAndTerminate(InputIterator begin, InputIterator end, OutputIterator output, Terminator terminator) {
	auto result(Copy(Move(begin), Move(end), Move(output)));
	*result++ = Move(terminator);

	return result;
}

// ---------------------------------------------------

template <typename Container>
ETInlineHint ETForceInlineHint typename Container::ConstIterator Begin(const Container& container) ETNoexceptHintIf(HasNoThrowMemberBegin<const Container>()) {
	return container.Begin();
}

// ---------------------------------------------------

template <typename Container>
ETInlineHint ETForceInlineHint typename Container::Iterator Begin(Container& container) ETNoexceptHintIf(HasNoThrowMemberBegin<Container>()) {
	return container.End();
}

// ---------------------------------------------------

template <typename Value, size_t ArraySize>
ETConstexpr ETForceInlineHint const Value* Begin(const Value (&array)[ArraySize]) ETNoexceptHint {
	return array;
}

// ---------------------------------------------------

template <typename Value, size_t ArraySize>
ETConstexpr ETForceInlineHint Value* Begin(Value (&array)[ArraySize]) ETNoexceptHint {
	return array;
}

// ---------------------------------------------------

template <typename Value>
ETConstexpr ETForceInlineHint const Value* Begin(const InitializerList<Value>& values) ETNoexceptHint {
	return values.begin();
}

// ---------------------------------------------------

template <typename Container>
ETInlineHint ETForceInlineHint typename Container::ConstIterator End(const Container& container) ETNoexceptHintIf(HasNoThrowMemberEnd<const Container>()) {
	return container.End();
}

// ---------------------------------------------------

template <typename Container>
ETInlineHint ETForceInlineHint typename Container::Iterator End(Container& container) ETNoexceptHintIf(HasNoThrowMemberEnd<Container>()) {
	return container.End();
}

// ---------------------------------------------------

template <typename Value, size_t ArraySize>
ETConstexpr ETForceInlineHint const Value* End(const Value (&array)[ArraySize]) ETNoexceptHint {
	return Begin(array) + ArraySize;
}

// ---------------------------------------------------

template <typename Value, size_t ArraySize>
ETConstexpr ETForceInlineHint Value* End(Value (&array)[ArraySize]) ETNoexceptHint {
	return Begin(array) + ArraySize;
}

// ---------------------------------------------------

template <typename Value>
ETConstexpr ETForceInlineHint const Value* End(const InitializerList<Value>& values) ETNoexceptHint {
	return values.end();
}

// ---------------------------------------------------

template <typename T, size_t ArraySize>
ETCpp14Constexpr ETForceInlineHint void Swap(T (&lhs)[ArraySize], T (&rhs)[ArraySize]) ETNoexceptHintIf(IsNoThrowSwappable<T>()) {
	for (size_t i(0u); i < ArraySize; ++i) {
		Swap(lhs[i], rhs[i]);
	}
}

// ---------------------------------------------------

template <typename T, ET_TEMPLATE_SFINAE_PARAM>
ETCpp14Constexpr ETForceInlineHint void Swap(T& lhs, T& rhs) ETNoexceptHintIf(IsNoThrowMoveConstructible<T>() && IsNoThrowMoveAssignable<T>()) {
	T temp(Move(lhs));
	lhs = Move(rhs);
	rhs = Move(temp);
}

// ---------------------------------------------------

template <typename T, typename U>
ETCpp14Constexpr ETForceInlineHint T Exchange(T& destination, U&& source) ETNoexceptHintIf(IsNoThrowMoveConstructible<T>() && IsNoThrowAssignable<T, U&&>()) {
	T temp(Move(destination));
	destination = Forward<U>(source);
	return Move(temp);
}

// ---------------------------------------------------

template <typename T>
ETConstexpr ETForceInlineHint CountingIterator<T> AsCounter(T value) ETNoexceptHint {
	return CountingIterator<T>(Move(value));
}

} // namespace Eldritch2
