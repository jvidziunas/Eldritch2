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
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename Identifier>
	ETForceInlineHint IdentifierPoolBase<Identifier>::IdentifierRange::IdentifierRange( IdentifierType firstIdentifier, IdentifierType lastIdentifier ) : firstIdentifier( firstIdentifier ), lastIdentifier( lastIdentifier ) {}

// ---------------------------------------------------

	template <typename Identifier>
	ETInlineHint bool IdentifierPoolBase<Identifier>::IdentifierRange::CanMergeForwardWith( const IdentifierRange& range ) const {
		return lastIdentifier == range.firstIdentifier;
	}

// ---------------------------------------------------

	template <typename Identifier>
	ETInlineHint bool IdentifierPoolBase<Identifier>::IdentifierRange::CanAccomodateRangeOfSize( IdentifierType size ) const {
		return size <= GetSize();
	}

// ---------------------------------------------------

	template <typename Identifier>
	typename IdentifierPoolBase<Identifier>::IdentifierType IdentifierPoolBase<Identifier>::IdentifierRange::GetSize() const {
		return lastIdentifier - firstIdentifier;
	}

// ---------------------------------------------------

	template <typename Identifier>
	ETInlineHint bool IdentifierPoolBase<Identifier>::IdentifierRange::Contains( IdentifierType identifier ) const {
		return (firstIdentifier <= identifier) & (identifier < lastIdentifier);
	}

// ---------------------------------------------------

	template <typename Identifier>
	ETInlineHint bool IdentifierPoolBase<Identifier>::IdentifierRange::IsEmpty() const {
		return firstIdentifier == lastIdentifier;
	}

}	// namespace Detail

	template <typename Identifier, class Allocator>
	ETInlineHint IdentifierPool<Identifier, Allocator>::IdentifierPool( std::initializer_list<IdentifierRange> ranges, const AllocatorType& allocator ) : IdentifierPool( allocator ) {
		for( const auto& range : ranges ) {
			ReleaseRange( range );
		}
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	ETInlineHint IdentifierPool<Identifier, Allocator>::IdentifierPool( IdentifierRange initialRange, const AllocatorType& allocator ) : IdentifierPool( allocator ) {
		ReleaseRange( initialRange );
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	ETInlineHint IdentifierPool<Identifier, Allocator>::IdentifierPool( IdentifierType maximumIdentifier, const AllocatorType& allocator ) : IdentifierPool( { static_cast<IdentifierType>( 0 ), maximumIdentifier }, allocator ) {}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	ETInlineHint IdentifierPool<Identifier, Allocator>::IdentifierPool( const AllocatorType& allocator ) : _availableRanges( allocator ) {}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	ETInlineHint IdentifierPool<Identifier, Allocator>::IdentifierPool( IdentifierPool<Identifier, Allocator>&& pool ) : _availableRanges( eastl::move( pool._availableRanges ) ) {}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	Eldritch2::Pair<Identifier, bool> IdentifierPool<Identifier, Allocator>::AllocateIdentifier() {
		return AllocateRangeOfSize( static_cast<Identifier>(1) );
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	Eldritch2::Pair<Identifier, bool> IdentifierPool<Identifier, Allocator>::AllocateRangeOfSize( IdentifierType rangeSizeInElements ) {
		const auto	candidate( eastl::find_if( _availableRanges.Begin(), _availableRanges.End(), [=] ( const IdentifierRange& range ) {
			return range.CanAccomodateRangeOfSize( rangeSizeInElements );
		} ) );

		if( _availableRanges.End() == candidate ) {
			return { 0, false };
		}

		const auto	returnedIdentifier( candidate->firstIdentifier );

		candidate->firstIdentifier += rangeSizeInElements;

		if( candidate->IsEmpty() && ( ( candidate + 1 ) < _availableRanges.End() ) ) {
		//	If current range is full and there is another one, that will become the new current range.
			_availableRanges.Erase( candidate );
		}

		return { returnedIdentifier, true };
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	void IdentifierPool<Identifier, Allocator>::ReleaseIdentifier( IdentifierType identifier ) {
		return ReleaseRange( { identifier, identifier + static_cast<IdentifierType>(1) } );
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	void IdentifierPool<Identifier, Allocator>::ReleaseRange( IdentifierRange releasedRange ) {
		const auto	candidate( eastl::lower_bound( _availableRanges.Begin(), _availableRanges.End(), releasedRange, [] ( const IdentifierRange& lhs, const IdentifierRange& rhs ) {
			return lhs.lastIdentifier <= rhs.firstIdentifier;
		} ) );

		if( candidate == _availableRanges.End() ) {
			_availableRanges.PushBack( releasedRange );
			return;
		}

		if( releasedRange.CanMergeForwardWith( *candidate ) ) {
			candidate->lastIdentifier = releasedRange.lastIdentifier;

			if( candidate + 1 != _availableRanges.End() && candidate->CanMergeForwardWith( candidate[1] ) ) {
				candidate->lastIdentifier = candidate[1].lastIdentifier;
				_availableRanges.Erase( candidate + 1 );
			}

			return;
		} 
		
		if( candidate->CanMergeForwardWith( releasedRange ) ) {
			candidate->firstIdentifier = releasedRange.firstIdentifier;

			if( candidate != _availableRanges.Begin() && candidate[-1].CanMergeForwardWith( *candidate ) ) {
				candidate[-1].lastIdentifier = candidate->firstIdentifier;
				_availableRanges.Erase( candidate - 1 );
			}

			return;
		}

		_availableRanges.Insert( candidate, releasedRange );

#if 0
		// Binary search of the range list
		AvailableRangeCollection::SizeType	i0( 0 );
		AvailableRangeCollection::SizeType	i1( _availableRanges.Size() - 1 );

		for( ; ; ) {
			const AvailableRangeCollection::SizeType	i( ( i0 + i1 ) / 2 );

			if( releasedRange.firstIdentifier < _availableRanges[i].begin ) {
				// Before current range, check if neighboring
				if( releasedRange.lastIdentifier >= _availableRanges[i].begin ) {
					// Overlaps a range of free IDs, thus (at least partially) invalid IDs
					ETRuntimeAssert( releasedRange.lastIdentifier == _availableRanges[i].begin );

					// Neighbor id, check if neighboring previous range too
					if( i > i0 && releasedRange.firstIdentifier - 1 == _availableRanges[i - 1].end ) {
						// Merge with previous range
						_availableRanges[i - 1].end = _availableRanges[i].end;
						_availableRanges.Erase( _availableRanges.Begin() + i );
					} else {
						// Just grow range
						_availableRanges[i].begin = releasedRange.firstIdentifier;
					}

					break;
				} else {
					// Non-neighbor id
					if( i != i0 ) {
						// Cull upper half of list
						i1 = i - 1;
					} else {
						// Found our position in the list, insert the deleted range here
						_availableRanges.Insert( _availableRanges.Begin() + i, AvailableRange( releasedRange.firstIdentifier, releasedRange.lastIdentifier - 1 ) );
						break;
					}
				}
			} else if( releasedRange.firstIdentifier > _availableRanges[i].end ) {
				// After current range, check if neighboring
				if( releasedRange.firstIdentifier - 1 == _availableRanges[i].end ) {
					// Neighbor id, check if neighboring next range too
					if( i < i1 && releasedRange.lastIdentifier == _availableRanges[i + 1].begin ) {
						// Merge with next range
						_availableRanges[i].end = _availableRanges[i + 1].end;
						_availableRanges.Erase( _availableRanges.Begin() + i + 1 );
					} else {
						// Just grow range
						_availableRanges[i].end += (releasedRange.lastIdentifier - releasedRange.firstIdentifier);
					}

					break;
				} else {
					// Non-neighbor id
					if( i != i1 ) {
						// Cull bottom half of list
						i0 = i + 1;
					} else {
						// Found our position in the list, insert the deleted range here
						_availableRanges.Insert( _availableRanges.Begin() + i + 1, AvailableRange( releasedRange.firstIdentifier, releasedRange.lastIdentifier - 1 ) );
						break;
					}
				}
			} else {
				// Inside a free block, not a valid ID
				ETRuntimeAssert( false );
			}
		}
#endif
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	ETInlineHint bool IdentifierPool<Identifier, Allocator>::IsEmpty() const {
		return _availableRanges.IsEmpty();
	}

// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	typename IdentifierPool<Identifier, Allocator>::IdentifierType IdentifierPool<Identifier, Allocator>::GetLargestAvailableBlockLengthInElements() const {
		IdentifierType	largestRangeSizeInElements( 0 );

		ForEachAvailableIdentifierRange( [&] ( const AvailableRange& range ) {
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
	void IdentifierPool<Identifier, Allocator>::ForEachAvailableIdentifierRange( Predicate predicate ) const {
		for( const auto& range : _availableRanges ) {
			predicate( range );
		}
	}

}	// namespace Eldritch2