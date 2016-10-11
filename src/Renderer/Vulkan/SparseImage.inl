/*==================================================================*\
  SparseImage.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	ETInlineHint size_t GetHashCode( const SparseImageVirtualCoordinate& coordinate, size_t seed = 0u ) {
		return seed + 31u * coordinate.subresourceIndex + ((coordinate.index.x * 73856093u) ^ (coordinate.index.y * 19349663u) ^ (coordinate.index.z * 83492791u));
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool operator==( const SparseImageVirtualCoordinate& left, const SparseImageVirtualCoordinate& right ) {
		return 0 == Eldritch2::CompareMemory( eastl::addressof( left ), eastl::addressof( right ), sizeof( left ) );
	}

// ---------------------------------------------------

	ETInlineHint SparseImageVirtualCoordinate::SparseImageVirtualCoordinate( VkOffset3D index, uint32_t subresourceIndex ) : index( index ), subresourceIndex( subresourceIndex ) {}

// ---------------------------------------------------

	ETInlineHint SparseImagePageManager::SparseImagePageManager( VkExtent3D tileSize, VkDeviceSize tileSizeInBytes ) : _tileExtent( tileSize ), _tileSizeInBytes( tileSizeInBytes ) {}

// ---------------------------------------------------

	ETInlineHint SparseImageVirtualCoordinate SparseImagePageManager::GetTile( VkOffset3D texel, uint32_t	subresourceIndex ) const {
		return { { texel.x / _tileExtent.width, texel.y / _tileExtent.height, texel.z / _tileExtent.depth }, subresourceIndex };
	}

// ---------------------------------------------------

	ETInlineHint VkExtent3D SparseImagePageManager::GetTileExtentInTexels() const {
		return _tileExtent;
	}

// ---------------------------------------------------

	ETInlineHint VkDeviceSize SparseImagePageManager::GetTileSizeInBytes() const {
		return _tileSizeInBytes;
	}

// ---------------------------------------------------

	ETInlineHint SparseImage::CommittedTile::CommittedTile( SparseImagePhysicalCoordinate coordinate ) : coordinate( coordinate ), referenceCount( 1u ) {}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2