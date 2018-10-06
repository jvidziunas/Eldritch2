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
#include <Common/Algorithms.hpp>
//------------------------------------------------------------------//
#include <eastl/iterator.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint Range<Iterator>::Range(Iterator begin, Iterator end) ETNoexceptHint : _begin(begin),
																												 _end(end) {}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint Range<Iterator>::Range() ETNoexceptHint : _begin(),
																					 _end() {}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint Range<Iterator>::operator bool() const ETNoexceptHint {
	return _begin != _end;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint bool Range<Iterator>::IsEmpty() const ETNoexceptHint {
	return _end == _begin;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint size_t Range<Iterator>::GetSize() const ETNoexceptHint {
	return eastl::distance(_begin, _end);
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint Iterator Range<Iterator>::Begin() const ETNoexceptHint {
	return _begin;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint Iterator Range<Iterator>::End() const ETNoexceptHint {
	return _end;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint void Range<Iterator>::SetBegin(Iterator begin) ETNoexceptHint {
	_begin = begin;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint void Range<Iterator>::SetEnd(Iterator end) ETNoexceptHint {
	_end = end;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint bool Range<Iterator>::Covers(const Range<Iterator>& range) const ETNoexceptHint {
	return (_begin <= range._begin) & (range._end <= _end);
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint bool Range<Iterator>::Covers(Iterator element) const ETNoexceptHint {
	return (_begin <= element) & (element < _end);
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint bool Range<Iterator>::CoversBegin(const Range<Iterator>& range) const ETNoexceptHint {
	return _begin <= range._begin;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETInlineHint ETForceInlineHint bool Range<Iterator>::CoversEnd(const Range<Iterator>& range) const ETNoexceptHint {
	return range._end <= _end;
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(const Range<Iterator>& lhs, const Range<Iterator>& rhs) ETNoexceptHint {
	return lhs.Begin() == rhs.Begin() && lhs.End() == rhs.End();
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator!=(const Range<Iterator>& lhs, const Range<Iterator>& rhs) ETNoexceptHint {
	return lhs.Begin() != rhs.Begin() || lhs.End() != rhs.End();
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint ETForceInlineHint void Swap(Range<Iterator>& lhs, Range<Iterator>& rhs) ETNoexceptHint {
	Swap(lhs._begin, rhs._begin);
	Swap(lhs._end, rhs._end);
}

} // namespace Eldritch2
