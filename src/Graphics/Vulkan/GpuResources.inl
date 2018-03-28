/*==================================================================*\
  GpuResources.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace Detail {

	ETInlineHint VkBuffer AbstractBuffer::Get() ETNoexceptHint {
		return _buffer;
	}

// ---------------------------------------------------

	ETInlineHint AbstractBuffer::operator VkBuffer() ETNoexceptHint {
		return this->Get();
	}

// ---------------------------------------------------

	ETInlineHint VkImage AbstractImage::Get() ETNoexceptHint {
		return _image;
	}

// ---------------------------------------------------

	ETInlineHint AbstractImage::operator VkImage() ETNoexceptHint {
		return this->Get();
	}

}	// namespace Detail

	ETInlineHint VkOffset3D	SparsePageManager::GetTexel( Tile tile ) const ETNoexceptHint {
		return VkOffset3D{ int32_t( tile.x * (1 << _logTileExtent.width) ), int32_t( tile.y * (1 << _logTileExtent.height) ), int32_t( tile.z * (1 << _logTileExtent.depth) ) };
	}

// ---------------------------------------------------

	ETInlineHint SparsePageManager::Tile SparsePageManager::GetTile( VkOffset3D texel, uint32_t mip ) const ETNoexceptHint {
		return { uint32_t( texel.x >> _logTileExtent.width ), uint32_t( texel.y >> _logTileExtent.height ), uint32_t( texel.z >> _logTileExtent.depth ), mip };
	}

// ---------------------------------------------------

	ETInlineHint VkExtent3D SparsePageManager::GetImageExtentInTexels() const ETNoexceptHint {
		return _extent;
	}

// ---------------------------------------------------

	ETInlineHint VkExtent3D SparsePageManager::GetTileExtentInTexels() const ETNoexceptHint {
		return { (1u << _logTileExtent.width), (1u << _logTileExtent.height), (1u << _logTileExtent.depth) };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t GetHashCode( const SparsePageManager::Tile& tile, size_t seed = 0u ) {
		return seed + 31u * tile.mip + ((tile.x * 73856093u) ^ (tile.y * 19349663u) ^ (tile.z * 83492791u));
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool operator<( const SparsePageManager::Tile& left, const SparsePageManager::Tile& right ) {
		return reinterpret_cast<const uint64&>(left) < reinterpret_cast<const uint64&>(right);
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2