/*==================================================================*\
  Range.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//
#include <iterator>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Iterator>
	class Range {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using IteratorType	= Iterator;
		using ValueType		= typename ::std::iterator_traits<Iterator>::value_type;
		using Reference		= typename ::std::iterator_traits<Iterator>::reference;
		using DistanceType	= typename ::std::iterator_traits<Iterator>::distance_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref Range instance.
		ETForceInlineHint Range( Iterator rangeBegin, Iterator rangeEnd );
		//! Constructs this @ref Range instance.
		ETForceInlineHint Range( Iterator rangeBegin, const DistanceType rangeSizeInElements );
		//! Constructs this @ref Range instance.
		ETForceInlineHint Range( const ::Eldritch2::Range<Iterator>& rangeTemplate );

		ETForceInlineHint ~Range() = default;

	// ---------------------------------------------------

		// Finds the first element contained within the Range that compares equal to the passed-in value, or the Range end iterator in the event no match was found.
		ETForceInlineHint Iterator		Find( const ValueType& value ) const;
		// Find the first element with a 'true' return value for the operator() function on the Predicate object.
		template <typename Predicate>
		ETForceInlineHint Iterator		Find( Predicate predicate ) const;

		template <typename Predicate, typename InputIterator>
		ETForceInlineHint const ::Eldritch2::Range<Iterator>&	TransformFrom( const ::Eldritch2::Range<InputIterator>& source, Predicate predicate ) const;

		template <typename Predicate>
		ETForceInlineHint const ::Eldritch2::Range<Iterator>&	GenerateFrom( Predicate predicate ) const;

		template <typename Predicate, typename Return>
		ETForceInlineHint Return	ValidateElements( Predicate elementValidator, Return defaultValue ) const;

		template <typename Predicate, typename Return>
		ETForceInlineHint Return	Accumulate( Predicate elementSummator, Return defaultValue ) const;

	// ---------------------------------------------------

		ETInlineHint ::Eldritch2::Range<Iterator>&	operator=( const ::Eldritch2::Range<Iterator>& rangeTemplate );

	// ---------------------------------------------------

		ETInlineHint static ::Eldritch2::Range<Iterator>	EmptySet();

	// ---------------------------------------------------

		//! Checks to see if this @ref Range contains any elements (begin and end do not point to the same element)
		ETForceInlineHint operator bool() const;

		//! Checks to see if this @ref Range is the empty set (begin and end point to the same element)
		ETForceInlineHint bool IsEmpty() const;

		ETForceInlineHint DistanceType	Size() const;

	// ---------------------------------------------------

		ETForceInlineHint Reference	operator[]( DistanceType offset ) const;

	// ---------------------------------------------------

		// Tests to see if the passed-in @ref Range is a subset of this @ref Range instance.
		ETForceInlineHint bool	Contains( const ::Eldritch2::Range<Iterator>& range ) const;
		// Tests to see if the passed in @ref Iterator references an element within the bounds specified by this @ref Range.
		ETForceInlineHint bool	Contains( Iterator element ) const;

		// Tests to see if this passed-in @ref Range is *not* a subset of this @ref Range instance.
		ETForceInlineHint bool	DoesNotContain( const ::Eldritch2::Range<Iterator>& range ) const;
		// Tests to see if the passed in @ref Iterator *does not reference* an element within the bounds specified by this @ref Range.
		ETForceInlineHint bool	DoesNotContain( Iterator element ) const;

		// Optimized containment check if the end of both @ref Range instances is known beforehand.
		ETForceInlineHint bool	ContainsBegin( const ::Eldritch2::Range<Iterator>& range ) const;

		// Optimized containment check if the beginning of both @ref Range instances is known beforehand.
		ETForceInlineHint bool	ContainsEnd( const ::Eldritch2::Range<Iterator>& range ) const;

	// - DATA MEMBERS ------------------------------------

		Iterator	first;
		Iterator	onePastLast;
	};

}	// namespace Eldritch2


//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/Range.inl>
//------------------------------------------------------------------//