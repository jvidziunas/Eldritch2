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

namespace Eldritch2 {

	template <typename Iterator>
	ETInlineHint Range<Iterator>::Range( Iterator begin, Iterator end ) : _begin( begin ), _end( end ) {}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint Eldritch2::Range<Iterator> Range<Iterator>::CreateEmptySet() {
		return { Iterator(), Iterator() };
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint Range<Iterator>::operator bool() const {
		return _begin != _end;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint bool Range<Iterator>::IsEmpty() const {
		return _end == _begin;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint size_t Range<Iterator>::GetSize() const {
		return eastl::distance( _begin, _end );
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint Iterator Range<Iterator>::Begin() const {
		return _begin;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint Iterator Range<Iterator>::End() const {
		return _end;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint void Range<Iterator>::SetBegin( Iterator newValue ) {
		_begin = newValue;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint void Range<Iterator>::SetEnd( Iterator newValue ) {
		_end = newValue;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint bool Range<Iterator>::IsSupersetOf( const Eldritch2::Range<Iterator>& range ) const {
		return ( _begin <= range._begin ) & ( range.onePastLast <= _end );
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint bool Range<Iterator>::ContainsIterator( Iterator element ) const {
		return ( _begin <= element ) & ( element < _end );
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint bool Range<Iterator>::ContainsBegin( const Eldritch2::Range<Iterator>& range ) const {
		return _begin <= range._begin;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint bool Range<Iterator>::ContainsEnd( const Eldritch2::Range<Iterator>& range ) const {
		return range.onePastLast <= _end;
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint ETPureFunctionHint bool operator==( const Eldritch2::Range<Iterator>& lhs, const Eldritch2::Range<Iterator>& rhs ) {
		return lhs.Begin() == rhs.Begin() && lhs.End() == rhs.End();
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint ETPureFunctionHint bool operator!=( const Eldritch2::Range<Iterator>& lhs, const Eldritch2::Range<Iterator>& rhs ) {
		return lhs.Begin() != rhs.Begin() || lhs.End() != rhs.End();
	}

}	// namespace Eldritch2