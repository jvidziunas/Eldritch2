/*==================================================================*\
  Span.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Mpl/TypeTraits.hpp>
//------------------------------------------------------------------//
#include <eastl/iterator.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Iterator>
class Span {
	// - TYPE PUBLISHING ---------------------------------

public:
	using IteratorType   = Iterator;
	using SliceType      = Span<IteratorType>;
	using DifferenceType = typename DifferenceType<IteratorType, IteratorType>::Type;
	using Reference      = typename DereferenceType<IteratorType>::Type;
	using SizeType       = size_t;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref Range instance.
	/*!	@param[in] begin Beginning of the elements contained by the new @ref Range.
		@param[in] end One past the last valid element contained by the new @ref Range. */
	ETConstexpr Span(Iterator begin, Iterator end) ETNoexceptHint;
	//! Constructs this @ref Range instance.
	ETConstexpr Span(const Span&) ETNoexceptHint = default;
	//! Constructs this @ref Range instance.
	ETConstexpr Span() ETNoexceptHint;

	~Span() = default;

	// ---------------------------------------------------

public:
	ETConstexpr Iterator Begin() const ETNoexceptHint;

	ETConstexpr Iterator End() const ETNoexceptHint;

	SliceType Slice(SizeType offset, SizeType length) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	ETConstexpr void SetBegin(Iterator begin) ETNoexceptHint;

	ETConstexpr void SetEnd(Iterator end) ETNoexceptHint;

	// ---------------------------------------------------

public:
	//!	Tests to see if the passed-in @ref Range is a subset of this @ref Range instance.
	ETConstexpr bool Covers(const Span& range) const ETNoexceptHint;
	//!	Tests to see if the passed in @ref Iterator references an element within the bounds specified by this @ref Range.
	ETConstexpr bool Covers(Iterator element) const ETNoexceptHint;

	//!	Optimized containment check if the end of both @ref Range instances is known beforehand.
	ETConstexpr bool CoversBegin(const Span& range) const ETNoexceptHint;

	//!	Optimized containment check if the beginning of both @ref Range instances is known beforehand.
	ETConstexpr bool CoversEnd(const Span& range) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	//! Checks to see if this @ref Range contains any elements (begin and end do not point to the same element)
	ETConstexpr explicit operator bool() const ETNoexceptHint;

	//! Checks to see if this @ref Range is the empty set (begin and end point to the same element)
	ETConstexpr bool IsEmpty() const ETNoexceptHint;

	ETConstexpr SizeType GetSize() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	Span& operator=(const Span&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	Iterator _begin;
	Iterator _end;

	// ---------------------------------------------------

	template <typename Iterator>
	friend void Swap(Span<Iterator>&, Span<Iterator>&) ETNoexceptHintIf(IsNoThrowSwappable<Iterator>());
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/Span.inl>
//------------------------------------------------------------------//
