/*==================================================================*\
  Algorithms.inl
  ------------------------------------------------------------------
  Purpose:
  Supplemental function collection designed to complement/augment
  the STL algorithms header.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <eastl/algorithm.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename InputIterator, typename OutputIterator, typename Predicate>
	ETInlineHint OutputIterator RemoveCopyIf( InputIterator begin, InputIterator end, OutputIterator destination, Predicate predicate ) {
		for( ; begin != end; ++begin ) {
			if( !predicate( *begin ) ) {
				*destination++ = *begin;
			}
		}

		return destination;
	}

}	// namespace Detail

	template <typename InputIterator, typename Predicate>
	InputIterator Find( InputIterator begin, InputIterator end, Predicate predicate ) {
		for( ; begin != end && !predicate( *begin ); ++begin ) {};

		return begin;
	}

// ---------------------------------------------------

	template <typename InputIterator>
	InputIterator FindElement( InputIterator begin, InputIterator end, decltype(*begin) element ) {
		while( begin != end ) {
			if( *begin++ == element ) {
				break;
			}
		}

		return begin;
	}

// ---------------------------------------------------

	template <typename InputIterator, typename Predicate>
	Predicate ForEach( InputIterator begin, InputIterator end, Predicate predicate ) {
		while( begin != end ) {
			predicate( *begin++ );
		}

		return predicate;
	}

// ---------------------------------------------------

	template <typename ObjectType, size_t arraySize, typename Predicate>
	Predicate ForEach( ObjectType (&range)[arraySize], Predicate predicate ) {
		return Eldritch2::ForEach( range, range + _countof( range ), predicate );
	}

// ---------------------------------------------------

	template <typename InputIterator, typename OutputIterator, typename Predicate>
	OutputIterator Transform( InputIterator begin, InputIterator end, OutputIterator outputBegin, Predicate predicate ) {
		while( begin != end ) {
			*outputBegin++ = predicate( *begin++ );
		}

		return outputBegin;
	}

// ---------------------------------------------------

	template <typename ObjectType, size_t arraySize, typename OutputIterator, typename Predicate>
	OutputIterator Transform( ObjectType (&range)[arraySize], OutputIterator outputBegin, Predicate predicate ) {
		return Eldritch2::Transform( static_cast<ObjectType*>(range), static_cast<ObjectType*>(range) + _countof(range), outputBegin, predicate );
	}

// ---------------------------------------------------

	template <typename InputIterator, typename SecondaryInputIterator, typename OutputIterator, typename Predicate>
	OutputIterator Transform( InputIterator begin, InputIterator end, SecondaryInputIterator secondaryBegin, OutputIterator outputBegin, Predicate predicate ) {
		while( begin != end ) {
			*outputBegin++ = predicate( *begin++, *secondaryBegin++ );
		}

		return outputBegin;
	}

// ---------------------------------------------------

	template <typename ObjectType, size_t arraySize, typename SecondaryInputIterator, typename OutputIterator, typename Predicate>
	OutputIterator Transform( ObjectType (&range)[arraySize], SecondaryInputIterator secondaryBegin, OutputIterator outputBegin, Predicate predicate ) {
		return Eldritch2::Transform( static_cast<ObjectType*>(range), static_cast<ObjectType*>(range) + _countof(range), secondaryBegin, outputBegin, predicate );
	}

// ---------------------------------------------------

	template <typename OutputIterator, typename Generator>
	OutputIterator Generate( OutputIterator begin, OutputIterator end, Generator generator ) {
		while( begin != end ) {
			*begin++ = generator();
		}

		return end;
	}

// ---------------------------------------------------

	template <typename OutputIterator, typename Generator>
	OutputIterator Generate( OutputIterator begin, size_t count, Generator generator ) {
		for( ; count; --count ) { 
			*begin++ = generator();
		}

		return begin;
	}

// ---------------------------------------------------

	template <typename ForwardIterator, typename Predicate>
	ForwardIterator	RemoveIf( ForwardIterator begin, ForwardIterator end, Predicate predicate ) {
		begin = Eldritch2::Find( begin, end, predicate );

		if( begin == end ) {
			return begin;
		}

		ForwardIterator	copyBegin( begin );
		return Detail::RemoveCopyIf( ++copyBegin, end, begin, predicate );
	}

// ---------------------------------------------------

	template <typename InputIterator, typename ValueType, typename ComparisonPredicate>
	InputIterator LowerBound( InputIterator rangeBegin, InputIterator rangeEnd, const ValueType& value, ComparisonPredicate predicate ) {
		return eastl::lower_bound( rangeBegin, rangeEnd, value, predicate );
	}

// ---------------------------------------------------

	template <typename InputIterator, typename ValueType, typename ComparisonPredicate>
	InputIterator UpperBound( InputIterator rangeBegin, InputIterator rangeEnd, const ValueType& value, ComparisonPredicate predicate ) {
		return eastl::upper_bound( rangeBegin, rangeEnd, value, predicate );
	}

// ---------------------------------------------------

	template <typename InputIterator, typename SortPredicate>
	void Sort( InputIterator rangeBegin, InputIterator rangeEnd, SortPredicate sortPredicate ) {
		eastl::sort( rangeBegin, rangeEnd, sortPredicate );
	}

// ---------------------------------------------------

	template <typename InputIterator>
	void DestroyRange( InputIterator rangeStart, InputIterator rangeEnd ) {
		Eldritch2::ForEach( rangeStart, rangeEnd, Eldritch2::InvokeDestructor() );
	}

// ---------------------------------------------------

	template <typename InputIterator>
	void TrivialConstructRange( InputIterator rangeStart, InputIterator rangeEnd ) {
		Eldritch2::ForEach( rangeStart, rangeEnd, Eldritch2::InvokeTrivialConstructor() );
	}

}	// namespace Eldritch2