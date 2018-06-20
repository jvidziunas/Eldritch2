/*==================================================================*\
  Algorithms.hpp
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

namespace Eldritch2 {

class InvokeDestructor {
public:
	template <typename ObjectType>
	ETInlineHint void operator()(ObjectType& object) const {
		object.~ObjectType();
	}

	template <typename ObjectType>
	ETInlineHint void operator()(ObjectType* const object) const {
		object->~ObjectType();
	}
};

// ---------------------------------------------------

class InvokeTrivialConstructor {
public:
	template <typename ObjectType>
	ETInlineHint void operator()(ObjectType& object) const {
		new (&object) ObjectType();
	}

	template <typename ObjectType>
	ETInlineHint void operator()(ObjectType* const object) const {
		new (object) ObjectType();
	}
};

// ---------------------------------------------------

template <typename InputIterator, typename Predicate>
InputIterator FindIf(InputIterator begin, InputIterator end, Predicate predicate);

template <typename InputIterator, typename Value>
InputIterator Find(InputIterator begin, InputIterator end, const Value& value);

// ---------------------------------------------------

//	Applies the operator() method of the specified predicate to each element in the specified range.
//	Returns a copy of the predicate after all operations have been performed.
template <typename InputIterator, typename Predicate>
Predicate ForEach(InputIterator begin, InputIterator end, Predicate predicate);
//	Applies the operator() method of the specified predicate to each element in the specified array.
//	Returns a copy of the predicate after all operations have been performed.
template <typename ObjectType, size_t arraySize, typename Predicate>
Predicate ForEach(ObjectType (&range)[arraySize], Predicate predicate);

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator, typename Predicate>
OutputIterator Transform(InputIterator begin, InputIterator end, OutputIterator outputBegin, Predicate predicate);

template <typename ObjectType, size_t arraySize, typename OutputIterator, typename Predicate>
OutputIterator Transform(ObjectType (&range)[arraySize], OutputIterator outputBegin, Predicate predicate);

template <typename InputIterator, typename SecondaryInputIterator, typename OutputIterator, typename Predicate>
OutputIterator Transform(InputIterator begin, InputIterator end, SecondaryInputIterator secondaryBegin, OutputIterator outputBegin, Predicate predicate);

template <typename ObjectType, size_t arraySize, typename SecondaryInputIterator, typename OutputIterator, typename Predicate>
OutputIterator Transform(ObjectType (&range)[arraySize], SecondaryInputIterator secondaryBegin, OutputIterator outputBegin, Predicate predicate);

// ---------------------------------------------------

template <typename OutputIterator, typename Generator>
OutputIterator Generate(OutputIterator begin, OutputIterator end, Generator generator);

template <typename OutputIterator, typename Generator>
OutputIterator Generate(OutputIterator begin, size_t count, Generator generator);

// ---------------------------------------------------

template <typename ForwardIterator, typename Predicate>
ForwardIterator RemoveIf(ForwardIterator begin, ForwardIterator end, Predicate predicate);

// ---------------------------------------------------

template <typename InputIterator, typename ValueType, typename ComparisonPredicate>
InputIterator LowerBound(InputIterator rangeBegin, InputIterator rangeEnd, const ValueType& value, ComparisonPredicate predicate);

template <typename InputIterator, typename ValueType, typename ComparisonPredicate>
InputIterator UpperBound(InputIterator rangeBegin, InputIterator rangeEnd, const ValueType& value, ComparisonPredicate predicate);

// ---------------------------------------------------

template <typename InputIterator, typename BinaryPredicate>
void Sort(InputIterator rangeBegin, InputIterator rangeEnd, BinaryPredicate sortPredicate);

// ---------------------------------------------------

template <typename InputIterator>
void DestroyRange(InputIterator rangeStart, InputIterator rangeEnd);

template <typename InputIterator>
void TrivialConstructRange(InputIterator rangeStart, InputIterator rangeEnd);

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator>
OutputIterator Copy(InputIterator begin, InputIterator end, OutputIterator output);

template <typename InputIterator, typename OutputIterator>
OutputIterator CopyAndTerminate(InputIterator begin, InputIterator end, OutputIterator output, decltype(*begin) terminator);

// ---------------------------------------------------

template <typename T, size_t arraySize>
void Swap(T (&array0)[arraySize], T (&array1)[arraySize]);
template <typename T>
void Swap(T& t0, T& t1);

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Algorithms.inl>
//------------------------------------------------------------------//
