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
#include <rdestl/algorithm.h>
#include <algorithm>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {
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

// ---------------------------------------------------

	template <typename InputIterator, typename OutputIterator, typename ExtraArgument, typename Predicate>
	ETInlineHint OutputIterator RemoveCopyIf( InputIterator begin, InputIterator end, OutputIterator destination, ExtraArgument extraArgument, Predicate predicate ) {
		for( ; begin != end; ++begin ) {
			if( !predicate( *begin, extraArgument ) ) { 
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

	template <typename InputIterator, typename ExtraArgument, typename Predicate>
	InputIterator Find( InputIterator begin, InputIterator end, ExtraArgument extraArgument, Predicate predicate ) {
		for( ; begin != end && !predicate( *begin++, extraArgument ); ++begin ) {};

		return begin;
	}

// ---------------------------------------------------

	template <typename InputIterator, typename Predicate>
	InputIterator FindInPlace( InputIterator begin, InputIterator end, Predicate& predicate ) {
		while( ( begin != end ) && predicate( *begin++ ) ) {};

		return begin;
	}

// ---------------------------------------------------

	template <typename InputIterator, typename ExtraArgument, typename Predicate>
	InputIterator FindInPlace( InputIterator begin, InputIterator end, ExtraArgument extraArgument, Predicate& predicate ) {
		while( ( begin != end ) && predicate( *begin++, extraArgument ) ) {};

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

	template <typename InputIterator, typename ExtraArgument, typename Predicate>
	Predicate ForEach( InputIterator begin, InputIterator end, ExtraArgument extraArgument, Predicate predicate ) {
		while( begin != end ) {
			predicate( *begin++, extraArgument );
		}

		return predicate;
	}

// ---------------------------------------------------

	template <typename ObjectType, size_t arraySize, typename Predicate>
	Predicate ForEach( ObjectType (&range)[arraySize], Predicate predicate ) {
		return Utility::ForEach( range, range + _countof( range ), predicate );
	}

// ---------------------------------------------------

	template <typename InputIterator, typename Predicate>
	Predicate& ForEachInPlace( InputIterator begin, InputIterator end, Predicate& predicate ) {
		while( begin != end ) {
			predicate( *begin++ );
		}

		return predicate;
	}

// ---------------------------------------------------

	template <typename InputIterator, typename Predicate>
	const Predicate& ForEachInPlace( InputIterator begin, InputIterator end, const Predicate& predicate ) {
		while( begin != end ) {
			predicate( *begin++ );
		}

		return predicate;
	}

// ---------------------------------------------------

	template <typename InputIterator, typename ExtraArgument, typename Predicate>
	Predicate& ForEachInPlace( InputIterator begin, InputIterator end, ExtraArgument extraArgument, Predicate& predicate ) {
		while( begin != end ) {
			predicate( *begin++, extraArgument );
		}

		return predicate;
	}

// ---------------------------------------------------

	template <typename InputIterator, typename ExtraArgument, typename Predicate>
	const Predicate& ForEachInPlace( InputIterator begin, InputIterator end, ExtraArgument extraArgument, const Predicate& predicate ) {
		while( begin != end ) {
			predicate( *begin++, extraArgument );
		}

		return predicate;
	}

// ---------------------------------------------------

	template <typename InputIterator, typename OutputIterator, typename Predicate>
	OutputIterator Transform( InputIterator begin, InputIterator end, OutputIterator outputBegin, Predicate predicate ) {
		while( begin != end ) {
			*outputBegin++ = predicate( *begin++ );
		}

		return begin;
	}

// ---------------------------------------------------

	template <typename ObjectType, size_t arraySize, typename OutputIterator, typename Predicate>
	OutputIterator Transform( ObjectType (&range)[arraySize], OutputIterator outputBegin, Predicate predicate ) {
		return Utility::Transform( range, range + _countof( range ), outputBegin, predicate );
	}

// ---------------------------------------------------

	template <typename InputIterator, typename OutputIterator, typename ExtraArgument, typename Predicate>
	OutputIterator Transform( InputIterator begin, InputIterator end, OutputIterator outputBegin, ExtraArgument extraArgument, Predicate predicate ) {
		while( begin != end ) {
			*outputBegin++ = predicate( *begin++, extraArgument );
		}

		return outputBegin;
	}

// ---------------------------------------------------

	template <typename ObjectType, size_t arraySize, typename OutputIterator, typename ExtraArgument, typename Predicate>
	OutputIterator Transform( ObjectType (&range)[arraySize], OutputIterator outputBegin, ExtraArgument extraArgument, Predicate predicate ) {
		return Utility::Transform( range, range + _countof( range ), outputBegin, extraArgument, predicate );
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
		return Utility::Transform( range, range + _countof( range ), secondaryBegin, outputBegin, predicate );
	}

// ---------------------------------------------------

	template <typename InputIterator, typename SecondaryInputIterator, typename OutputIterator, typename ExtraArgument, typename Predicate>
	OutputIterator Transform( InputIterator begin, InputIterator end, SecondaryInputIterator secondaryBegin, OutputIterator outputBegin, ExtraArgument extraArgument, Predicate predicate ) {
		while( begin != end ) {
			*outputBegin++ = predicate( *begin++, *secondaryBegin++, extraArgument );
		}

		return outputBegin;
	}

// ---------------------------------------------------

	template <typename ObjectType, size_t arraySize, typename SecondaryInputIterator, typename OutputIterator, typename ExtraArgument, typename Predicate>
	OutputIterator Transform( ObjectType (&range)[arraySize], SecondaryInputIterator secondaryBegin, OutputIterator outputBegin, ExtraArgument extraArgument, Predicate predicate ) {
		return Utility::Transform( range, range + _countof( range ), secondaryBegin, outputBegin, extraArgument, predicate );
	}

// ---------------------------------------------------

	template <typename OutputIterator, typename Generator>
	OutputIterator Generate( OutputIterator begin, OutputIterator end, Generator generator ) {
		while( begin != end ) { *begin++ = generator(); }

		return end;
	}

// ---------------------------------------------------

	template <typename OutputIterator, typename ExtraArgument, typename Generator>
	OutputIterator Generate( OutputIterator begin, OutputIterator end, ExtraArgument extraArgument, Generator generator ) {
		while( begin != end ) { 
			*begin++ = generator( extraArgument );
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
		begin = Utility::Find( begin, end, predicate );

		if( begin == end ) {
			return begin;
		}

		ForwardIterator	copyBegin( begin );
		return Detail::RemoveCopyIf( ++copyBegin, end, begin, predicate );
	}

// ---------------------------------------------------

	template <typename ForwardIterator, typename ExtraArgument, typename Predicate>
	ForwardIterator	RemoveIf( ForwardIterator begin, ForwardIterator end, ExtraArgument extraArgument, Predicate predicate ) {
		begin = Utility::Find( begin, end, extraArgument, predicate );

		if( begin == end ) {
			return begin;
		}

		// nonempty sequence, worth doing
		ForwardIterator	copyBegin( begin );
		return Detail::RemoveCopyIf( ++copyBegin, end, begin, extraArgument, predicate );
	}

// ---------------------------------------------------

	template <typename InputIterator, typename ValueType, typename ComparisonPredicate>
	InputIterator LowerBound( InputIterator rangeBegin, InputIterator rangeEnd, const ValueType& value, ComparisonPredicate predicate ) {
		return ::std::lower_bound( rangeBegin, rangeEnd, value, predicate );
	}

// ---------------------------------------------------

	template <typename InputIterator, typename ValueType, typename ComparisonPredicate>
	InputIterator UpperBound( InputIterator rangeBegin, InputIterator rangeEnd, const ValueType& value, ComparisonPredicate predicate ) {
		return ::std::upper_bound( rangeBegin, rangeEnd, value, predicate );
	}

// ---------------------------------------------------

	template <typename InputIterator, typename SortPredicate>
	void Sort( InputIterator rangeBegin, InputIterator rangeEnd, SortPredicate sortPredicate ) {
		::std::sort( rangeBegin, rangeEnd, sortPredicate );
	}

// ---------------------------------------------------

	template <typename InputIterator>
	void DestroyRange( InputIterator rangeStart, InputIterator rangeEnd ) {
		Utility::ForEach( rangeStart, rangeEnd, Utility::InvokeDestructor() );
	}

// ---------------------------------------------------

	template <typename InputIterator>
	void TrivialConstructRange( InputIterator rangeStart, InputIterator rangeEnd ) {
		Utility::ForEach( rangeStart, rangeEnd, Utility::InvokeTrivialConstructor() );
	}

}	// namespace Utility
}	// namespace Eldritch2