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
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class InvokeDestructor {
	public:
		template <typename ObjectType>
		ETForceInlineHint void operator()( ObjectType& object ) const { object.~ObjectType(); }

		template <typename ObjectType>
		ETForceInlineHint void operator()( ObjectType* const object ) const { object->~ObjectType(); }
	};

// ---------------------------------------------------

	class InvokeTrivialConstructor {
	public:
		template <typename ObjectType>
		ETForceInlineHint void operator()( ObjectType& object ) const { new( &object ) ObjectType(); }

		template <typename ObjectType>
		ETForceInlineHint void operator()( ObjectType* const object ) const { new( object ) ObjectType(); }
	};

// ---------------------------------------------------

	class IsEquivalent {
	public:
		template <typename ObjectType>
		ETForceInlineHint bool	operator()( const ObjectType& object0, const ObjectType& object1 ) const { return object0 == object1; }
	};

// ---------------------------------------------------

	template <typename InputIterator, typename Predicate>
	InputIterator	Find( InputIterator begin, InputIterator end, Predicate predicate );

	template <typename InputIterator, typename ExtraArgument, typename Predicate>
	InputIterator	Find( InputIterator begin, InputIterator end, ExtraArgument extraArgument, Predicate predicate );

	template <typename InputIterator, typename Predicate>
	InputIterator	FindInPlace( InputIterator begin, InputIterator end, Predicate& predicate );

	template <typename InputIterator, typename ExtraArgument, typename Predicate>
	InputIterator	FindInPlace( InputIterator begin, InputIterator end, ExtraArgument extraArgument, Predicate& predicate );

	template <typename InputIterator>
	InputIterator	FindElement( InputIterator begin, InputIterator end, decltype(*begin) element );

// ---------------------------------------------------

	// Applies the operator() method of the specified predicate to each element in the specified range.
	// Returns a copy of the predicate after all operations have been performed.
	template <typename InputIterator, typename Predicate>
	Predicate			ForEach( InputIterator begin, InputIterator end, Predicate predicate );
	// Applies the operator() method of the specified predicate to each element in the specified range.
	// Returns a copy of the predicate after all operations have been performed.
	template <typename InputIterator, typename ExtraArgument, typename Predicate>
	Predicate			ForEach( InputIterator begin, InputIterator end, ExtraArgument extraArgument, Predicate predicate );
	// Applies the operator() method of the specified predicate to each element in the specified array.
	// Returns a copy of the predicate after all operations have been performed.
	template <typename ObjectType, size_t arraySize, typename Predicate>
	Predicate			ForEach( ObjectType (&range)[arraySize], Predicate predicate );

	// Applies the operator() method of the specified predicate to each element in the specified range.
	// Returns a reference to the passed-in predicate.
	template <typename InputIterator, typename Predicate>
	Predicate&			ForEachInPlace( InputIterator begin, InputIterator end, Predicate& predicate );
	// Applies the operator() method of the specified predicate to each element in the specified range.
	// Returns a reference to the passed-in predicate.
	// Note that operator() of the predicate object *must* be callable when the target object is const!
	template <typename InputIterator, typename Predicate>
	const Predicate&	ForEachInPlace( InputIterator begin, InputIterator end, const Predicate& predicate );
	// Applies the operator() method of the specified predicate to each element in the specified range.
	// Returns a reference to the passed-in predicate.
	template <typename InputIterator, typename ExtraArgument, typename Predicate>
	Predicate&			ForEachInPlace( InputIterator begin, InputIterator end, ExtraArgument extraArgument, Predicate& predicate );
	// Applies the operator() method of the specified predicate to each element in the specified range.
	// Returns a reference to the passed-in predicate.
	// Note that operator() of the predicate object *must* be callable when the target object is const!
	template <typename InputIterator, typename ExtraArgument, typename Predicate>
	const Predicate&	ForEachInPlace( InputIterator begin, InputIterator end, ExtraArgument extraArgument, const Predicate& predicate );

// ---------------------------------------------------

	template <typename InputIterator, typename OutputIterator, typename Predicate>
	OutputIterator	Transform( InputIterator begin, InputIterator end, OutputIterator outputBegin, Predicate predicate );

	template <typename ObjectType, size_t arraySize, typename OutputIterator, typename Predicate>
	OutputIterator	Transform( ObjectType (&range)[arraySize], OutputIterator outputBegin, Predicate predicate );

	template <typename InputIterator, typename OutputIterator, typename ExtraArgument, typename Predicate>
	OutputIterator	Transform( InputIterator begin, InputIterator end, OutputIterator outputBegin, ExtraArgument extraArgument, Predicate predicate );

	template <typename ObjectType, size_t arraySize, typename OutputIterator, typename ExtraArgument, typename Predicate>
	OutputIterator	Transform( ObjectType (&range)[arraySize], OutputIterator outputBegin, ExtraArgument extraArgument, Predicate predicate );

	template <typename InputIterator, typename SecondaryInputIterator, typename OutputIterator, typename Predicate>
	OutputIterator	Transform( InputIterator begin, InputIterator end, SecondaryInputIterator secondaryBegin, OutputIterator outputBegin, Predicate predicate );

	template <typename ObjectType, size_t arraySize, typename SecondaryInputIterator, typename OutputIterator, typename Predicate>
	OutputIterator	Transform( ObjectType (&range)[arraySize], SecondaryInputIterator secondaryBegin, OutputIterator outputBegin, Predicate predicate );

	template <typename InputIterator, typename SecondaryInputIterator, typename OutputIterator, typename ExtraArgument, typename Predicate>
	OutputIterator	Transform( InputIterator begin, InputIterator end, SecondaryInputIterator secondaryBegin, OutputIterator outputBegin, ExtraArgument extraArgument, Predicate predicate );

	template <typename ObjectType, size_t arraySize, typename SecondaryInputIterator, typename OutputIterator, typename ExtraArgument, typename Predicate>
	OutputIterator	Transform( ObjectType (&range)[arraySize], SecondaryInputIterator secondaryBegin, OutputIterator outputBegin, ExtraArgument extraArgument, Predicate predicate );

// ---------------------------------------------------

	template <typename OutputIterator, typename Generator>
	OutputIterator	Generate( OutputIterator begin, OutputIterator end, Generator generator );

	template <typename OutputIterator, typename ExtraArgument, typename Generator>
	OutputIterator	Generate( OutputIterator begin, OutputIterator end, ExtraArgument extraArgument, Generator generator );

	template <typename OutputIterator, typename Generator>
	OutputIterator	Generate( OutputIterator begin, size_t count, Generator generator );

	template <typename OutputIterator, typename ExtraArgument, typename Generator>
	OutputIterator	Generate( OutputIterator begin, size_t count, ExtraArgument extraArgument, Generator generator );

// ---------------------------------------------------

	template <typename ForwardIterator, typename Predicate>
	ForwardIterator	RemoveIf( ForwardIterator begin, ForwardIterator end, Predicate predicate );

	template <typename ForwardIterator, typename ExtraArgument, typename Predicate>
	ForwardIterator	RemoveIf( ForwardIterator begin, ForwardIterator end, ExtraArgument extraArgumentument, Predicate predicate );

// ---------------------------------------------------

	template <typename InputIterator, typename ValueType, typename ComparisonPredicate>
	InputIterator	LowerBound( InputIterator rangeBegin, InputIterator rangeEnd, const ValueType& value, ComparisonPredicate predicate );

	template <typename InputIterator, typename ValueType, typename ComparisonPredicate>
	InputIterator	UpperBound( InputIterator rangeBegin, InputIterator rangeEnd, const ValueType& value, ComparisonPredicate predicate );

// ---------------------------------------------------

	template <typename InputIterator, typename SortPredicate>
	void	Sort( InputIterator rangeBegin, InputIterator rangeEnd, SortPredicate sortPredicate );

// ---------------------------------------------------

	template <typename InputIterator>
	void	DestroyRange( InputIterator rangeStart, InputIterator rangeEnd );

	template <typename InputIterator>
	void	TrivialConstructRange( InputIterator rangeStart, InputIterator rangeEnd );

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Algorithms.inl>
//------------------------------------------------------------------//