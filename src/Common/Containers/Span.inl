/*==================================================================*\
  Span.inl
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
ETConstexpr ETForceInlineHint Span<Iterator>::Span(Iterator begin, Iterator end) ETNoexceptHint : _begin(begin), _end(end) {}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETForceInlineHint Span<Iterator>::Span() ETNoexceptHint : _begin(), _end() {}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETForceInlineHint Span<Iterator>::operator bool() const ETNoexceptHint {
	return _begin != _end;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETForceInlineHint bool Span<Iterator>::IsEmpty() const ETNoexceptHint {
	return _begin == _end;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETForceInlineHint typename Span<Iterator>::SizeType Span<Iterator>::GetSize() const ETNoexceptHint {
	return eastl::distance(_begin, _end);
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETForceInlineHint Iterator Span<Iterator>::Begin() const ETNoexceptHint {
	return _begin;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETForceInlineHint Iterator Span<Iterator>::End() const ETNoexceptHint {
	return _end;
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint ETForceInlineHint typename Span<Iterator>::SliceType Span<Iterator>::Slice(SizeType offset, SizeType length) const ETNoexceptHint {
	return SliceType(eastl::next(_begin, offset), eastl::next(_begin, Minimum(length, this->GetSize() - offset)));
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETForceInlineHint void Span<Iterator>::SetBegin(Iterator begin) ETNoexceptHint {
	_begin = begin;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETForceInlineHint void Span<Iterator>::SetEnd(Iterator end) ETNoexceptHint {
	_end = end;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETForceInlineHint bool Span<Iterator>::Covers(const Span<Iterator>& range) const ETNoexceptHint {
	return (_begin <= range._begin) & (range._end <= _end);
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETForceInlineHint bool Span<Iterator>::Covers(Iterator element) const ETNoexceptHint {
	return (_begin <= element) & (element < _end);
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETForceInlineHint bool Span<Iterator>::CoversBegin(const Span<Iterator>& range) const ETNoexceptHint {
	return _begin <= range._begin;
}

// ---------------------------------------------------

template <typename Iterator>
ETConstexpr ETForceInlineHint bool Span<Iterator>::CoversEnd(const Span<Iterator>& range) const ETNoexceptHint {
	return range._end <= _end;
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(const Span<Iterator>& lhs, const Span<Iterator>& rhs) ETNoexceptHint {
	return lhs.Begin() == rhs.Begin() && lhs.End() == rhs.End();
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator!=(const Span<Iterator>& lhs, const Span<Iterator>& rhs) ETNoexceptHint {
	return lhs.Begin() != rhs.Begin() || lhs.End() != rhs.End();
}

// ---------------------------------------------------

template <typename Iterator>
ETInlineHint ETForceInlineHint void Swap(Span<Iterator>& lhs, Span<Iterator>& rhs) ETNoexceptHintIf(IsNoThrowSwappable<Iterator>()) {
	Swap(lhs._begin, rhs._begin);
	Swap(lhs._end, rhs._end);
}

} // namespace Eldritch2
