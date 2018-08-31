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
	ETInlineHint ETForceInlineHint void operator()(ObjectType* object) const {
		object->~ObjectType();
	}

	template <typename ObjectType>
	ETInlineHint ETForceInlineHint void operator()(ObjectType& object) const {
		object.~ObjectType();
	}
};

// ---------------------------------------------------

class InvokeTrivialConstructor {
public:
	template <typename ObjectType>
	ETInlineHint ETForceInlineHint void operator()(ObjectType& object) const {
		new (ETAddressOf(object)) ObjectType();
	}

	template <typename ObjectType>
	ETInlineHint ETForceInlineHint void operator()(ObjectType* object) const {
		new (object) ObjectType();
	}
};

// ---------------------------------------------------

template <typename InputIterator, typename UnaryPredicate>
InputIterator FindIf(InputIterator begin, InputIterator end, UnaryPredicate condition);

template <typename InputIterator, typename Value>
InputIterator Find(InputIterator begin, InputIterator end, const Value& value);

// ---------------------------------------------------

template <typename InputIterator, typename UnaryPredicate>
InputIterator FindIfNot(InputIterator begin, InputIterator end, UnaryPredicate condition);

// ---------------------------------------------------

//	Applies the operator() method of the specified predicate to each element in the specified range.
//	Returns a copy of the predicate after all operations have been performed.
template <typename InputIterator, typename UnaryPredicate>
UnaryPredicate ForEach(InputIterator begin, InputIterator end, UnaryPredicate predicate);

// ---------------------------------------------------

template <typename InputIterator, typename Value, typename BinaryPredicate>
Value Reduce(InputIterator begin, InputIterator end, Value initial, BinaryPredicate reducer);
template <typename InputIterator, typename BinaryPredicate>
auto Reduce(InputIterator begin, InputIterator end, BinaryPredicate reducer) -> decltype(*begin);

// ---------------------------------------------------

template <typename InputIterator>
InputIterator Unique(InputIterator begin, InputIterator end);
template <typename InputIterator, typename BinaryPredicate>
InputIterator Unique(InputIterator begin, InputIterator end, BinaryPredicate equal);

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator, typename UnaryPredicate>
OutputIterator Transform(InputIterator begin, InputIterator end, OutputIterator output, UnaryPredicate transform);
template <typename InputIterator, typename InputIterator2, typename OutputIterator, typename BinaryPredicate>
OutputIterator Transform(InputIterator begin, InputIterator end, InputIterator2 begin2, OutputIterator output, BinaryPredicate transform);

// ---------------------------------------------------

template <typename OutputIterator, typename Generator>
OutputIterator Generate(OutputIterator begin, OutputIterator end, Generator generator);
template <typename OutputIterator, typename Generator>
OutputIterator Generate(OutputIterator begin, size_t count, Generator generator);

// ---------------------------------------------------

template <typename ForwardIterator, typename UnaryPredicate>
ForwardIterator RemoveIf(ForwardIterator begin, ForwardIterator end, UnaryPredicate filter);

template <typename ForwardIterator, typename Value>
ForwardIterator Remove(ForwardIterator begin, ForwardIterator end, const Value& value);

// ---------------------------------------------------

template <typename OutputIterator, typename Value>
void Fill(OutputIterator begin, OutputIterator end, const Value& value);

// ---------------------------------------------------

template <typename InputIterator, typename ValueType, typename BinaryPredicate>
InputIterator LowerBound(InputIterator begin, InputIterator end, const ValueType& value, BinaryPredicate compare);

template <typename InputIterator, typename ValueType, typename BinaryPredicate>
InputIterator UpperBound(InputIterator begin, InputIterator end, const ValueType& value, BinaryPredicate compare);

// ---------------------------------------------------

template <typename InputIterator>
void Sort(InputIterator begin, InputIterator end);
template <typename InputIterator, typename BinaryPredicate>
void Sort(InputIterator begin, InputIterator end, BinaryPredicate compare);

// ---------------------------------------------------

template <typename InputIterator>
void TrivialConstruct(InputIterator begin, InputIterator end);

template <typename InputIterator>
void Destroy(InputIterator begin, InputIterator end);

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator>
OutputIterator Copy(InputIterator begin, InputIterator end, OutputIterator output);

template <typename InputIterator, typename OutputIterator, typename Terminator>
OutputIterator CopyAndTerminate(InputIterator begin, InputIterator end, OutputIterator output, Terminator terminator);

// ---------------------------------------------------

template <typename Container>
typename Container::ConstIterator Begin(const Container& container);
template <typename Container>
typename Container::Iterator Begin(Container& container);
template <typename Value, size_t length>
ETCpp14Constexpr const Value* Begin(const Value (&array)[length]) ETNoexceptHint;
template <typename Value, size_t length>
ETCpp14Constexpr Value* Begin(Value (&array)[length]) ETNoexceptHint;

template <typename Container>
typename Container::ConstIterator End(const Container& container);
template <typename Container>
typename Container::Iterator End(Container& container);
template <typename Value, size_t length>
ETCpp14Constexpr const Value* End(const Value (&array)[length]) ETNoexceptHint;
template <typename Value, size_t length>
ETCpp14Constexpr Value* End(Value (&array)[length]) ETNoexceptHint;

// ---------------------------------------------------

template <typename T, size_t size>
void Swap(const T (&lhs)[size], const T (&rhs)[size]) = delete;
template <typename T, size_t size>
void Swap(T (&lhs)[size], T (&rhs)[size]);
template <typename T>
void Swap(const T& lhs, const T& rhs) = delete;
template <typename T>
void Swap(T& lhs, T& rhs);

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Algorithms.inl>
//------------------------------------------------------------------//
