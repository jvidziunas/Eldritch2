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

	ETInlineHint size_t ETSimdCall SpatialHasher::operator()( CellIndex cellIndex ) const {
		return (cellIndex.coordinates[0] * 73856093u) ^ (cellIndex.coordinates[1] * 19349663u) ^ (cellIndex.coordinates[2] * 83492791u);
	}

// ---------------------------------------------------

	ETInlineHint size_t ETSimdCall SpatialHasher::operator()( Eldritch2::Float4 position ) const {
		return (*this)( Eldritch2::Floor( position * _inverseResolution ) );
	}

// ---------------------------------------------------

	template <typename Value, class Hasher, typename Allocator>
	ETInlineHint SpatialHash<Value, Hasher, Allocator>::SpatialHash( const HashPredicateType& hashPredicate, const AllocatorType& allocator ) : UnderlyingContainer( hashPredicate, allocator ) {}

}	// namespace Eldritch2