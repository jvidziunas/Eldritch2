/*==================================================================*\
  SpatialHash.inl
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	ETInlineHint size_t SpatialHasher::operator()( const CellIndex& cellIndex ) {
		return (cellIndex.coordinates[0] * 73856093u) ^ (cellIndex.coordinates[1] * 19349663u) ^ (cellIndex.coordinates[2] * 83492791u);
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	template <typename... AllocatorConstructorArguments>
	ETInlineHint SpatialHash<StoredObject, Hasher, Allocator>::SpatialHash( const SizeType initialBucketCount, const ::Eldritch2::float32 horizontalCellResolution, const ::Eldritch2::float32 verticalCellResolution, AllocatorConstructorArguments&&... allocatorConstructorArguments ) : _inverseResolution( 1.0f / horizontalCellResolution, 1.0f / verticalCellResolution, 1.0f / horizontalCellResolution, 0.0f ),
																																																																							_underlyingContainer( initialBucketCount, ::std::forward<AllocatorConstructorArguments>( allocatorConstructorArguments )... ) {}


// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	template <typename... AllocatorConstructorArguments>
	ETInlineHint SpatialHash<StoredObject, Hasher, Allocator>::SpatialHash( const SizeType initialBucketCount, const Hasher& hasher, const ::Eldritch2::float32 horizontalCellResolution, const ::Eldritch2::float32 verticalCellResolution, AllocatorConstructorArguments&&... allocatorConstructorArguments ) : _inverseResolution( 1.0f / horizontalCellResolution, 1.0f / verticalCellResolution, 1.0f / horizontalCellResolution, 0.0f ),
																																																																												  _underlyingContainer( initialBucketCount, ::std::forward<AllocatorConstructorArguments>( allocatorConstructorArguments )... ) {}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	ETInlineHint SpatialHash<StoredObject, Hasher, Allocator>::~SpatialHash() {}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	ETInlineHint StoredObject& SpatialHash<StoredObject, Hasher, Allocator>::operator[]( const ::Eldritch2::Float4 position ) {
		return _underlyingContainer[Hasher::CellIndexForPosition( position, _inverseResolution )];
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	ETInlineHint const StoredObject& SpatialHash<StoredObject, Hasher, Allocator>::operator[]( const ::Eldritch2::Float4 position ) const {
		return _underlyingContainer[Hasher::CellIndexForPosition( position, _inverseResolution )];
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	template <typename Predicate>
	ETInlineHint void SpatialHash<StoredObject, Hasher, Allocator>::RemoveIf( Predicate predicate ) {
		for( auto element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element ) ) {
				_underlyingContainer.erase( element++ );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	ETInlineHint void SpatialHash<StoredObject, Hasher, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	ETInlineHint void SpatialHash<StoredObject, Hasher, Allocator>::Reserve( const SizeType minimumSizeHint ) {
		return _underlyingContainer.reserve( minimumSizeHint );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	ETInlineHint typename SpatialHash<StoredObject, Hasher, Allocator>::SizeType SpatialHash<StoredObject, Hasher, Allocator>::BucketCount() const {
		return _underlyingContainer.bucket_count();
	}

// ---------------------------------------------------
	template <typename StoredObject, class Hasher, typename Allocator>
	ETInlineHint typename SpatialHash<StoredObject, Hasher, Allocator>::SizeType SpatialHash<StoredObject, Hasher, Allocator>::NonemptyBucketCount() const {
		return _underlyingContainer.nonempty_bucket_count();
	}

// ---------------------------------------------------
	template <typename StoredObject, class Hasher, typename Allocator>
	ETInlineHint typename SpatialHash<StoredObject, Hasher, Allocator>::SizeType SpatialHash<StoredObject, Hasher, Allocator>::UsedMemory() const {
		return _underlyingContainer.used_memory();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	ETInlineHint const Allocator& SpatialHash<StoredObject, Hasher, Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2