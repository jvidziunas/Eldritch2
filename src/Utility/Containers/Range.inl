/*==================================================================*\
  Range.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Algorithms.hpp>
//------------------------------------------------------------------//

namespace std {

	template <typename Iterator>
	ETForceInlineHint ETNoAliasHint auto begin( ::Eldritch2::Range<Iterator>& range ) -> decltype(range.first) {
		return range.first;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint ETNoAliasHint auto begin( const ::Eldritch2::Range<Iterator>& range ) -> decltype(range.first) {
		return range.first;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint ETNoAliasHint auto end( ::Eldritch2::Range<Iterator>& range ) -> decltype(range.onePastLast) {
		return range.onePastLast;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint ETNoAliasHint auto end( const ::Eldritch2::Range<Iterator>& range ) -> decltype(range.onePastLast) {
		return range.onePastLast;
	}

}	// namespace std

namespace Eldritch2 {

	template <typename Iterator>
	ETForceInlineHint Range<Iterator>::Range( Iterator begin, Iterator end ) : first( begin ), onePastLast( end ) {}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint Range<Iterator>::Range( Iterator begin, const DistanceType rangeSizeInElements ) : Range( begin, begin + rangeSizeInElements ) {}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint Range<Iterator>::Range( const ::Eldritch2::Range<Iterator>& rangeTemplate ) : Range( rangeTemplate.first, rangeTemplate.onePastLast ) {}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint ::Eldritch2::Range<Iterator>& ::Eldritch2::Range<Iterator>::operator=( const ::Eldritch2::Range<Iterator>& rangeTemplate ) {
		first		= rangeTemplate.first;
		onePastLast	= rangeTemplate.onePastLast;

		return *this;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint ::Eldritch2::Range<Iterator> Range<Iterator>::EmptySet() {
		return ::Eldritch2::Range<Iterator>( Iterator(), Iterator() );
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint typename Iterator Range<Iterator>::Find( const ValueType& value ) const {
		return this->Find( [&value] ( const ValueType& currentValue ) {
			return currentValue == value;
		} );
	}

// ---------------------------------------------------

	template <typename Iterator>
	template <typename Predicate>
	ETForceInlineHint typename Iterator Range<Iterator>::Find( Predicate predicate ) const {
		return ::Eldritch2::Utility::Find( first, onePastLast, predicate );
	}

// ---------------------------------------------------

	template <typename Iterator>
	template <typename Predicate, typename InputIterator>
	ETForceInlineHint const ::Eldritch2::Range<Iterator>& Range<Iterator>::TransformFrom( const ::Eldritch2::Range<InputIterator>& source, Predicate predicate ) const {
		ETRuntimeAssert( source.Size() == this->Size() );
		
		::Eldritch2::Utility::Transform( source.begin, source.onePastLast, first, predicate );

		return *this;
	}

// ---------------------------------------------------

	template <typename Iterator>
	template <typename Predicate>
	ETForceInlineHint const ::Eldritch2::Range<Iterator>& Range<Iterator>::GenerateFrom( Predicate predicate ) const {
		::Eldritch2::Utility::Generate( first, onePastLast, predicate );

		return *this;
	}

// ---------------------------------------------------

	template <typename Iterator>
	template <typename Predicate, typename Return>
	ETForceInlineHint Return Range<Iterator>::ValidateElements( Predicate elementValidator, Return returnValue ) const {
		for( const auto& element : (*this) ) {
			returnValue = elementValidator( element );
			if( !returnValue ) {
				break;
			}
		}

		return returnValue;
	}

// ---------------------------------------------------

	template <typename Iterator>
	template <typename Predicate, typename Return>
	ETForceInlineHint Return Range<Iterator>::Accumulate( Predicate elementSummator, Return returnValue ) const {
		for( const auto& element : (*this) ) {
			returnValue += elementSummator( element );
		}

		return returnValue;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint Range<Iterator>::operator bool() const {
		return first != onePastLast;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint bool Range<Iterator>::IsEmpty() const {
		return first == onePastLast;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint typename Range<Iterator>::DistanceType Range<Iterator>::Size() const {
		return ::std::distance( first, onePastLast );
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint typename Range<Iterator>::Reference Range<Iterator>::operator[]( DistanceType offset ) const {
		Iterator	temp( first );
		return ( ::std::advance( temp, offset ), *temp );
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint bool Range<Iterator>::Contains( const ::Eldritch2::Range<Iterator>& range ) const {
		return ( first <= range.first ) & ( range.onePastLast <= onePastLast );
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint bool Range<Iterator>::Contains( Iterator element ) const {
		return ( first <= element ) & ( element < onePastLast );
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint bool Range<Iterator>::DoesNotContain( const ::Eldritch2::Range<Iterator>& range ) const {
		return !this->Contains( range );
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint bool Range<Iterator>::DoesNotContain( Iterator element ) const {
		return !this->Contains( element );
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint bool Range<Iterator>::ContainsBegin( const ::Eldritch2::Range<Iterator>& range ) const {
		return first <= range.first;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETForceInlineHint bool Range<Iterator>::ContainsEnd( const ::Eldritch2::Range<Iterator>& range ) const {
		return range.onePastLast <= onePastLast;
	}

}	// namespace Eldritch2