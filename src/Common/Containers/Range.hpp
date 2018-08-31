/*==================================================================*\
  Range.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
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
	ETConstexpr Range(Iterator begin, Iterator end);
	//! Constructs this @ref Range instance.
	ETConstexpr Range(const Range&) = default;
	//! Constructs this @ref Range instance.
	ETConstexpr Range();

	~Range() = default;

	// ---------------------------------------------------

public:
	ETConstexpr Iterator Begin() const;

	ETConstexpr Iterator End() const;

	// ---------------------------------------------------

public:
	ETConstexpr void SetBegin(Iterator begin);

	ETConstexpr void SetEnd(Iterator end);

	// ---------------------------------------------------

public:
	//!	Tests to see if the passed-in @ref Range is a subset of this @ref Range instance.
	ETConstexpr bool Covers(const Range& range) const;
	//!	Tests to see if the passed in @ref Iterator references an element within the bounds specified by this @ref Range.
	ETConstexpr bool Covers(Iterator element) const;

	//!	Optimized containment check if the end of both @ref Range instances is known beforehand.
	ETConstexpr bool CoversBegin(const Range& range) const;

	//!	Optimized containment check if the beginning of both @ref Range instances is known beforehand.
	ETConstexpr bool CoversEnd(const Range& range) const;

	// ---------------------------------------------------

public:
	//! Checks to see if this @ref Range contains any elements (begin and end do not point to the same element)
	ETConstexpr explicit operator bool() const;

	//! Checks to see if this @ref Range is the empty set (begin and end point to the same element)
	ETConstexpr bool IsEmpty() const;

	ETConstexpr size_t GetSize() const;

	// ---------------------------------------------------

public:
	Range& operator=(const Range&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	Iterator _begin;
	Iterator _end;

	// ---------------------------------------------------

	template <typename Iterator>
	friend void Swap(Range<Iterator>&, Range<Iterator>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/Range.inl>
//------------------------------------------------------------------//
