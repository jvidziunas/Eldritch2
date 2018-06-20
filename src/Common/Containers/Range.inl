/*==================================================================*\
  Range.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Algorithms.hpp>
//------------------------------------------------------------------//
#include <eastl/iterator.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Iterator>
ETInlineHint Range<Iterator>::Range(Iterator begin, Iterator end) :
	_begin(begin),
	_end(end) {}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint Range<Iterator>::Range() :
	_begin(),
	_end() {}

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
	return eastl::distance(_begin, _end);
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
ETInlineHint void Range<Iterator>::SetBegin(Iterator begin) {
	_begin = begin;
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint void Range<Iterator>::SetEnd(Iterator end) {
	_end = end;
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint bool Range<Iterator>::Covers(const Range<Iterator>& range) const {
	return (_begin <= range._begin) & (range._end <= _end);
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint bool Range<Iterator>::Covers(Iterator element) const {
	return (_begin <= element) & (element < _end);
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint bool Range<Iterator>::CoversBegin(const Range<Iterator>& range) const {
	return _begin <= range._begin;
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint bool Range<Iterator>::CoversEnd(const Range<Iterator>& range) const {
	return range._end <= _end;
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint ETPureFunctionHint bool operator==(const Range<Iterator>& lhs, const Range<Iterator>& rhs) {
	return lhs.Begin() == rhs.Begin() && lhs.End() == rhs.End();
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint ETPureFunctionHint bool operator!=(const Range<Iterator>& lhs, const Range<Iterator>& rhs) {
	return lhs.Begin() != rhs.Begin() || lhs.End() != rhs.End();
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint void Swap(Range<Iterator>& lhs, Range<Iterator>& rhs) {
	Swap(lhs._begin, rhs._begin);
	Swap(lhs._end, rhs._end);
}

} // namespace Eldritch2
