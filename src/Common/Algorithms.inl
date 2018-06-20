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

	template <typename InputIterator, typename OutputIterator, typename Predicate>
	ETInlineHint OutputIterator RemoveCopyif(InputIterator begin, InputIterator end, OutputIterator destination, Predicate predicate) {
		for (; begin != end; ++begin) {
			if (!predicate(*begin)) {
				*destination++ = *begin;
			}
		}

		return destination;
	}

} // namespace Detail

template <typename InputIterator, typename Predicate>
ETInlineHint InputIterator FindIf(InputIterator begin, InputIterator end, Predicate predicate) {
	return eastl::find_if(begin, end, predicate);
}

// ---------------------------------------------------

template <typename InputIterator, typename Value>
ETInlineHint InputIterator Find(InputIterator begin, InputIterator end, const Value& value) {
	return eastl::find(begin, end, value);
}

// ---------------------------------------------------

template <typename InputIterator, typename Predicate>
ETInlineHint Predicate ForEach(InputIterator begin, InputIterator end, Predicate predicate) {
	return eastl::for_each(begin, end, predicate);
}

// ---------------------------------------------------

template <typename ObjectType, size_t arraySize, typename Predicate>
ETInlineHint Predicate ForEach(ObjectType (&range)[arraySize], Predicate predicate) {
	return eastl::for_each(eastl::begin(range), eastl::end(range), predicate);
}

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator, typename Predicate>
ETInlineHint OutputIterator Transform(InputIterator begin, InputIterator end, OutputIterator output, Predicate predicate) {
	return eastl::transform(begin, end, output, predicate);
}

// ---------------------------------------------------

template <typename ObjectType, size_t arraySize, typename OutputIterator, typename Predicate>
ETInlineHint OutputIterator Transform(ObjectType (&objects)[arraySize], OutputIterator output, Predicate predicate) {
	return eastl::transform(eastl::begin(objects), eastl::end(objects), output, predicate);
}

// ---------------------------------------------------

template <typename InputIterator, typename SecondaryInputIterator, typename OutputIterator, typename Predicate>
ETInlineHint OutputIterator Transform(InputIterator begin, InputIterator end, SecondaryInputIterator secondaryBegin, OutputIterator output, Predicate predicate) {
	return eastl::transform(begin, end, secondaryBegin, output, predicate);
}

// ---------------------------------------------------

template <typename ObjectType, size_t arraySize, typename SecondaryInputIterator, typename OutputIterator, typename Predicate>
ETInlineHint OutputIterator Transform(ObjectType (&objects)[arraySize], SecondaryInputIterator secondaryBegin, OutputIterator output, Predicate predicate) {
	return eastl::transform(eastl::begin(objects), eastl::end(objects), secondaryBegin, output, predicate);
}

// ---------------------------------------------------

template <typename OutputIterator, typename Generator>
ETInlineHint OutputIterator Generate(OutputIterator begin, OutputIterator end, Generator generator) {
	return eastl::generate(begin, end, generator);
}

// ---------------------------------------------------

template <typename OutputIterator, typename Generator>
ETInlineHint OutputIterator Generate(OutputIterator begin, size_t count, Generator generator) {
	return eastl::generate_n(begin, count, generator);
}

// ---------------------------------------------------

template <typename ForwardIterator, typename UnaryPredicate>
ETInlineHint ForwardIterator RemoveIf(ForwardIterator begin, ForwardIterator end, UnaryPredicate filter) {
	begin = FindIf(begin, end, filter);

	if (begin == end) {
		return begin;
	}

	ForwardIterator copyBegin(begin);
	return Detail::RemoveCopyif(++copyBegin, end, begin, filter);
}

// ---------------------------------------------------

template <typename InputIterator, typename ValueType, typename ComparisonPredicate>
ETInlineHint InputIterator LowerBound(InputIterator begin, InputIterator end, const ValueType& value, ComparisonPredicate predicate) {
	return eastl::lower_bound(begin, end, value, predicate);
}

// ---------------------------------------------------

template <typename InputIterator, typename ValueType, typename ComparisonPredicate>
ETInlineHint InputIterator UpperBound(InputIterator begin, InputIterator end, const ValueType& value, ComparisonPredicate predicate) {
	return eastl::upper_bound(begin, end, value, predicate);
}

// ---------------------------------------------------

template <typename InputIterator, typename BinaryPredicate>
ETInlineHint void Sort(InputIterator begin, InputIterator end, BinaryPredicate sortPredicate) {
	eastl::sort(begin, end, sortPredicate);
}

// ---------------------------------------------------

template <typename InputIterator>
ETInlineHint void DestroyRange(InputIterator begin, InputIterator end) {
	eastl::for_each(begin, end, InvokeDestructor());
}

// ---------------------------------------------------

template <typename InputIterator>
ETInlineHint void TrivialConstructRange(InputIterator begin, InputIterator end) {
	eastl::for_each(begin, end, InvokeTrivialConstructor());
}

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator>
ETInlineHint OutputIterator Copy(InputIterator begin, InputIterator end, OutputIterator output) {
	return eastl::copy(begin, end, output);
}

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator>
ETInlineHint OutputIterator CopyAndTerminate(InputIterator begin, InputIterator end, OutputIterator output, decltype(*begin) terminator) {
	auto result(Copy(begin, end, output));

	*result++ = terminator;

	return result;
}

// ---------------------------------------------------

template <typename T, size_t arraySize>
ETInlineHint void Swap(T (&array0)[arraySize], T (&array1)[arraySize]) {
	for (size_t i(0u); i < arraySize; ++i) {
		Swap(array0[i], array1[i]);
	}
}

// ---------------------------------------------------

template <typename T>
ETInlineHint void Swap(T& t0, T& t1) {
	auto temp(eastl::move(t0));

	t0 = eastl::move(t1);
	t1 = eastl::move(temp);
}

} // namespace Eldritch2
