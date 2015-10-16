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
	ETInlineHint SpatialHash<StoredObject, Hasher, Allocator>::SpatialHash( const SizeType initialBucketCount, const ::Eldritch2::float32 horizontalCellResolution, const ::Eldritch2::float32 verticalCellResolution, AllocatorType&& allocator ) : UnderlyingContainer( initialBucketCount, ::std::move( allocator ) ), _inverseResolution( 1.0f / horizontalCellResolution, 1.0f / verticalCellResolution, 1.0f / horizontalCellResolution, 0.0f ) {}


// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	ETInlineHint SpatialHash<StoredObject, Hasher, Allocator>::SpatialHash( const SizeType initialBucketCount, const Hasher& hasher, const ::Eldritch2::float32 horizontalCellResolution, const ::Eldritch2::float32 verticalCellResolution, AllocatorType&& allocator ) : UnderlyingContainer( initialBucketCount, ::std::move( allocator ) ), _inverseResolution( 1.0f / horizontalCellResolution, 1.0f / verticalCellResolution, 1.0f / horizontalCellResolution, 0.0f ) {}


// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	ETInlineHint StoredObject& SpatialHash<StoredObject, Hasher, Allocator>::operator[]( const ::Eldritch2::Float4 position ) {
		return UnderlyingContainer::operator[]( Hasher::CellIndexForPosition( position, _inverseResolution ) );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator>
	ETInlineHint const StoredObject& SpatialHash<StoredObject, Hasher, Allocator>::operator[]( const ::Eldritch2::Float4 position ) const {
		return UnderlyingContainer::operator[]( Hasher::CellIndexForPosition( position, _inverseResolution ) );
	}

}	// namespace Eldritch2