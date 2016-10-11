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
#include <Utility/Containers/RangeAdapters.hpp>
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <eastl/iterator.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Iterator>
	class Range {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using IteratorType	= Iterator;
		using ValueType		= typename eastl::iterator_traits<Iterator>::value_type;
		using Reference		= typename eastl::iterator_traits<Iterator>::reference;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref Range instance.
	/*!	@param[in] begin Beginning of the elements contained by the new @ref Range.
		@param[in] end One past the last valid element contained by the new @ref Range. */
		Range( Iterator begin, Iterator end );
	//! Constructs this @ref Range instance.
		Range( const Range& ) = default;

		~Range() = default;

	// ---------------------------------------------------

	public:
		Range&	operator=( const Range& ) = default;

	// ---------------------------------------------------

	public:
		static Range	CreateEmptySet();

	// ---------------------------------------------------

	public:
	//! Checks to see if this @ref Range contains any elements (begin and end do not point to the same element)
		explicit operator	bool() const;

	//! Checks to see if this @ref Range is the empty set (begin and end point to the same element)
		bool				IsEmpty() const;

		size_t				GetSize() const;

	// ---------------------------------------------------

	public:
		Iterator	Begin() const;

		Iterator	End() const;

	// ---------------------------------------------------

	public:
		void	SetBegin( Iterator newValue );

		void	SetEnd( Iterator newValue );

	// ---------------------------------------------------

	public:
	// Tests to see if the passed-in @ref Range is a subset of this @ref Range instance.
		bool	IsSupersetOf( const Range& range ) const;
	// Tests to see if the passed in @ref Iterator references an element within the bounds specified by this @ref Range.
		bool	ContainsIterator( Iterator element ) const;

	// Optimized containment check if the end of both @ref Range instances is known beforehand.
		bool	ContainsBegin( const Range& range ) const;

	// Optimized containment check if the beginning of both @ref Range instances is known beforehand.
		bool	ContainsEnd( const Range& range ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		Iterator	_begin;
		Iterator	_end;
	};

}	// namespace Eldritch2


//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/Range.inl>
//------------------------------------------------------------------//