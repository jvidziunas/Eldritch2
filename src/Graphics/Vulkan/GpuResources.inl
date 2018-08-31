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

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace Detail {

		ETInlineHint ETForceInlineHint VkBuffer AbstractBuffer::Get() const ETNoexceptHint {
			return _buffer;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint AbstractBuffer::operator VkBuffer() const ETNoexceptHint {
			return this->Get();
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint VkImage AbstractImage::Get() const ETNoexceptHint {
			return _image;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint AbstractImage::operator VkImage() const ETNoexceptHint {
			return this->Get();
		}

	} // namespace Detail

	ETInlineHint ETForceInlineHint VkBuffer VertexCache::GetVertexBuffer() const ETNoexceptHint {
		return _vertices;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkBuffer VertexCache::GetIndexBuffer() const ETNoexceptHint {
		return _indices;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkBuffer Mesh::GetVertices() const ETNoexceptHint {
		return _vertices;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkBuffer Mesh::GetIndices() const ETNoexceptHint {
		return _indices;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool SparseTileCache::IsCached(VkDeviceSize& outOffset, Tile tile) const ETNoexceptHint {
		const auto candidate(_cachedPagesByTile.Find(tile));
		if (ET_UNLIKELY(candidate == _cachedPagesByTile.End())) {
			return false;
		}

		outOffset = candidate->second;
		return true;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkOffset3D TileManager::GetTexel(Tile tile) const ETNoexceptHint {
		return VkOffset3D { int32_t(tile.x * (1ull << _logTileExtent.width)), int32_t(tile.y * (1ull << _logTileExtent.height)), int32_t(tile.z * (1ull << _logTileExtent.depth)) };
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint TileManager::Tile TileManager::GetTile(VkOffset3D texel, uint32_t subresource) const ETNoexceptHint {
		return TileManager::Tile { uint32_t(texel.x >> _logTileExtent.width), uint32_t(texel.y >> _logTileExtent.height), uint32_t(texel.z >> _logTileExtent.depth), subresource };
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkExtent3D TileManager::GetImageExtent() const ETNoexceptHint {
		return _imageExtent;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkExtent3D TileManager::GetTileExtent() const ETNoexceptHint {
		return { (1u << _logTileExtent.width), (1u << _logTileExtent.height), (1u << _logTileExtent.depth) };
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool TileManager::Touch(Tile tile) ETNoexceptHint {
		return _residentTiles.Contains(tile);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void TileManager::ReplaceLru(Tile /*tile*/) ETNoexceptHint {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const TileManager& SparseShaderImage::GetTileManager() const ETNoexceptHint {
		return _tileManager;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint SparseShaderImage::CacheResult SparseShaderImage::MakeResident(const SparseTileCache& cache, VkOffset3D texel, uint32_t mip) {
		return MakeResident(cache, _tileManager.GetTile(texel, mip));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint SparseShaderImage::CacheResult SparseShaderImage::MakeResident(const SparseTileCache& cache, TileManager::Tile tile) {
		if (_tileManager.Touch(tile)) {
			return CacheResult::Hit;
		}

		VkDeviceSize physicalTile;
		if (cache.IsCached(physicalTile, tile)) {
			_tileManager.ReplaceLru(tile);
			// -> Bind physical tile location to memory here <-
			return CacheResult::L0Miss;
		}

		return CacheResult::L1Miss;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint size_t GetHashCode(const TileManager::Tile& tile, size_t seed = 0u) {
		return seed + 31u * tile.mip + ((tile.x * 73856093u) ^ (tile.y * 19349663u) ^ (tile.z * 83492791u));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator<(const TileManager::Tile& left, const TileManager::Tile& right) {
		return reinterpret_cast<const uint64&>(left) < reinterpret_cast<const uint64&>(right);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(const TileManager::Tile& left, const TileManager::Tile& right) {
		return reinterpret_cast<const uint64&>(left) == reinterpret_cast<const uint64&>(right);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator!=(const TileManager::Tile& left, const TileManager::Tile& right) {
		return !(left == right);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
