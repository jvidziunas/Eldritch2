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
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#include <eastl/iterator.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Iterator>
class Range {
	// - TYPE PUBLISHING ---------------------------------

public:
	using ValueType    = typename eastl::iterator_traits<Iterator>::value_type;
	using Reference    = typename eastl::iterator_traits<Iterator>::reference;
	using IteratorType = Iterator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref Range instance.
	/*!	@param[in] begin Beginning of the elements contained by the new @ref Range.
		@param[in] end One past the last valid element contained by the new @ref Range. */
	ETConstexpr Range(Iterator begin, Iterator end) ETNoexceptHint;
	//! Constructs this @ref Range instance.
	ETConstexpr Range(const Range&) ETNoexceptHint = default;
	//! Constructs this @ref Range instance.
	ETConstexpr Range() ETNoexceptHint;

	~Range() = default;

	// ---------------------------------------------------

public:
	ETConstexpr Iterator Begin() const ETNoexceptHint;

	ETConstexpr Iterator End() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	ETConstexpr void SetBegin(Iterator begin) ETNoexceptHint;

	ETConstexpr void SetEnd(Iterator end) ETNoexceptHint;

	// ---------------------------------------------------

public:
	//!	Tests to see if the passed-in @ref Range is a subset of this @ref Range instance.
	ETConstexpr bool Covers(const Range& range) const ETNoexceptHint;
	//!	Tests to see if the passed in @ref Iterator references an element within the bounds specified by this @ref Range.
	ETConstexpr bool Covers(Iterator element) const ETNoexceptHint;

	//!	Optimized containment check if the end of both @ref Range instances is known beforehand.
	ETConstexpr bool CoversBegin(const Range& range) const ETNoexceptHint;

	//!	Optimized containment check if the beginning of both @ref Range instances is known beforehand.
	ETConstexpr bool CoversEnd(const Range& range) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	//! Checks to see if this @ref Range contains any elements (begin and end do not point to the same element)
	ETConstexpr explicit operator bool() const ETNoexceptHint;

	//! Checks to see if this @ref Range is the empty set (begin and end point to the same element)
	ETConstexpr bool IsEmpty() const ETNoexceptHint;

	ETConstexpr size_t GetSize() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	Range& operator=(const Range&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	Iterator _begin;
	Iterator _end;

	// ---------------------------------------------------

	template <typename Iterator>
	friend void Swap(Range<Iterator>&, Range<Iterator>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/Range.inl>
//------------------------------------------------------------------//
