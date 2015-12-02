/*==================================================================*\
  SpatialHash.inl
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	ETInlineHint size_t SpatialHasher::operator()( const CellIndex cellIndex ) const {
		return (cellIndex.coordinates[0] * 73856093u) ^ (cellIndex.coordinates[1] * 19349663u) ^ (cellIndex.coordinates[2] * 83492791u);
	}

// ---------------------------------------------------

	ETInlineHint size_t SpatialHasher::operator()( const ::Eldritch2::Float4 position ) const {
		const ::Eldritch2::Float4	cellCoordinates( ::Eldritch2::Floor( position * _inverseResolution ) );

		return (*this)( cellCoordinates );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator, int loadFactor>
	ETInlineHint SpatialHash<StoredObject, Hasher, Allocator, loadFactor>::SpatialHash( Hasher&& hasher, AllocatorType&& allocator ) : UnderlyingContainer( ::std::forward<Hasher>( hasher ), ::std::forward<AllocatorType>( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, typename Allocator, int loadFactor>
	ETInlineHint SpatialHash<StoredObject, Hasher, Allocator, loadFactor>::SpatialHash( AllocatorType&& allocator ) : UnderlyingContainer( ::std::forward<AllocatorType>( allocator ) ) {}

}	// namespace Eldritch2