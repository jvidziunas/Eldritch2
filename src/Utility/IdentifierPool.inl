/*==================================================================*\
  IdentifierPool.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Assert.hpp>
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	template <typename Identifier, class Allocator>
	ETForceInlineHint IdentifierPool<Identifier, Allocator>::AvailableRange::AvailableRange( IdentifierType initialElement, IdentifierType endElement ) : begin( initialElement ), end( endElement ) {}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	ETForceInlineHint bool IdentifierPool<Identifier, Allocator>::AvailableRange::ContainsElements() const {
		return begin <= end;
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	IdentifierPool<Identifier, Allocator>::IdentifierPool( IdentifierType maximumIdentifier, AllocatorType&& allocator ) : _availableRanges( ::std::move( allocator ) ) {
		_availableRanges.Reserve( 16u );
		_availableRanges.PushBack( { static_cast<IdentifierType>(0), maximumIdentifier } );
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	typename IdentifierPool<Identifier, Allocator>::AllocationResult IdentifierPool<Identifier, Allocator>::AllocateIdentifier() {
		AllocationResult	result { 0u, true };
		AvailableRange&		activeRange( _availableRanges.Front() );

		if( activeRange.ContainsElements() ) {
			result.identifier = activeRange.begin;

			// If current range is full and there is another one, that will become the new current range
			if( activeRange.begin == activeRange.end && ( _availableRanges.Size() > 1 ) ) {
				_availableRanges.Erase( _availableRanges.Begin() );
			} else {
				++activeRange.begin;
			}
		} else {
			// No available ID left
			result.successful = false;
		}
		
		return result;
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	typename IdentifierPool<Identifier, Allocator>::AllocationResult IdentifierPool<Identifier, Allocator>::AllocateIdentifierRange( const DifferenceType rangeSizeInElements ) {
		class FindAppropriateRangePredicate {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			ETForceInlineHint FindAppropriateRangePredicate( const DifferenceType rangeSizeInElements ) : _rangeSizeInElements( rangeSizeInElements ) {}

		// ---------------------------------------------------

			ETForceInlineHint bool	operator()( typename const IdentifierPool<Identifier, Allocator>::AvailableRange& range ) const {
				return _rangeSizeInElements <= ( 1 + range.end - range.begin );
			}

		// - DATA MEMBERS ------------------------------------

		private:
			const DifferenceType	_rangeSizeInElements;
		};

	// ---

		AllocationResult	result { 0u, true };
		const auto			range( Find( _availableRanges.Begin(), _availableRanges.End(), FindAppropriateRangePredicate( rangeSizeInElements ) ) );

		if( _availableRanges.End() != range ) {
			result.identifier = range->begin;

			if( rangeSizeInElements == ( 1 + range->end - range->begin ) && ( ( range + 1 ) < _availableRanges.End() ) ) {
				// If current range is full and there is another one, that will become the new current range
				_availableRanges.Erase( range );
			} else {
				range->begin += rangeSizeInElements;
			}
		} else {
			// No range of free IDs was large enough to create the requested continuous ID sequence
			result.successful = false;
		}

		return result;
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	void IdentifierPool<Identifier, Allocator>::ReleaseIdentifier( const IdentifierType identifier ) {
		return this->ReleaseIdentifierRange( identifier, identifier + 1u );
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	void IdentifierPool<Identifier, Allocator>::ReleaseIdentifierRange( const IdentifierType rangeBegin, const IdentifierType rangeEnd ) {
		// Binary search of the range list
		AvailableRangeCollection::SizeType	i0( 0 );
		AvailableRangeCollection::SizeType	i1( _availableRanges.Size() - 1 );

		for( ; ; ) {
			const AvailableRangeCollection::SizeType	i( ( i0 + i1 ) / 2 );

			if( rangeBegin < _availableRanges[i].begin ) {
				// Before current range, check if neighboring
				if( rangeEnd >= _availableRanges[i].begin ) {
					// Overlaps a range of free IDs, thus (at least partially) invalid IDs
					ETRuntimeAssert( rangeEnd == _availableRanges[i].begin );

					// Neighbor id, check if neighboring previous range too
					if( i > i0 && rangeBegin - 1 == _availableRanges[i - 1].end ) {
						// Merge with previous range
						_availableRanges[i - 1].end = _availableRanges[i].end;
						_availableRanges.Erase( _availableRanges.Begin() + i );
					} else {
						// Just grow range
						_availableRanges[i].begin = rangeBegin;
					}

					break;
				} else {
					// Non-neighbor id
					if( i != i0 ) {
						// Cull upper half of list
						i1 = i - 1;
					} else {
						// Found our position in the list, insert the deleted range here
						_availableRanges.Insert( _availableRanges.Begin() + i, AvailableRange( rangeBegin, rangeEnd - 1 ) );
						break;
					}
				}
			} else if( rangeBegin > _availableRanges[i].end ) {
				// After current range, check if neighboring
				if( rangeBegin - 1 == _availableRanges[i].end ) {
					// Neighbor id, check if neighboring next range too
					if( i < i1 && rangeEnd == _availableRanges[i + 1].begin ) {
						// Merge with next range
						_availableRanges[i].end = _availableRanges[i + 1].end;
						_availableRanges.Erase( _availableRanges.Begin() + i + 1 );
					} else {
						// Just grow range
						_availableRanges[i].end += (rangeEnd - rangeBegin);
					}

					break;
				} else {
					// Non-neighbor id
					if( i != i1 ) {
						// Cull bottom half of list
						i0 = i + 1;
					} else {
						// Found our position in the list, insert the deleted range here
						_availableRanges.Insert( _availableRanges.Begin() + i + 1, AvailableRange( rangeBegin, rangeEnd - 1 ) );
						break;
					}
				}
			} else {
				// Inside a free block, not a valid ID
				ETRuntimeAssert( false );
			}
		}
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	typename IdentifierPool<Identifier, Allocator>::DifferenceType IdentifierPool<Identifier, Allocator>::GetLargestAvailableBlockLengthInElements() const {
		DifferenceType	largestRangeSizeInElements( 0 );

		ForEachAvailableIdentifierRange( [&largestRangeSizeInElements] ( const AvailableRange& range ) {
			const auto	rangeLength( range.end - range.begin );

			if( largestRangeSizeInElements < rangeLength ) {
				largestRangeSizeInElements = rangeLength;
			}
		} );
			
		return largestRangeSizeInElements;
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	template <typename Predicate>
	Predicate IdentifierPool<Identifier, Allocator>::ForEachAvailableIdentifierRange( Predicate predicate ) const {
		for( const auto& range : _availableRanges ) {
			predicate( range );
		}
	}

}	// namespace Utility
}	// namespace Eldritch2