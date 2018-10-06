/*==================================================================*\
  GpuResourceApi.inl
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

	ETConstexpr ETInlineHint ETForceInlineHint VkBuffer GpuBuffer::Get() const ETNoexceptHint {
		return _buffer;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint GpuBuffer::operator VkBuffer() const ETNoexceptHint {
		return _buffer;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint VkImage GpuImage::Get() const ETNoexceptHint {
		return _image;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint GpuImage::operator VkImage() const ETNoexceptHint {
		return _image;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint VkBuffer VertexCache::GetVertexBuffer() const ETNoexceptHint {
		return _vertices;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint VkBuffer VertexCache::GetIndexBuffer() const ETNoexceptHint {
		return _indices;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint VkBuffer Mesh::GetVertices() const ETNoexceptHint {
		return _vertices;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint VkBuffer Mesh::GetIndices() const ETNoexceptHint {
		return _indices;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint VkExtent3D TileManager::GetTileExtent() const ETNoexceptHint {
		return _tileExtent;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool TileManager::Touch(Tile tile) ETNoexceptHint {
		return _residentTiles.Contains(tile);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void TileManager::ReplaceLru(Tile /*tile*/) ETNoexceptHint {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void HostSparseTileCache::NotifyCached(VkDeviceSize offset, TileManager::Tile tile) {
		_loadingTiles.Erase(tile);
		_cachedPagesByTile.Emplace(tile, offset);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool HostSparseTileCache::IsCached(VkDeviceSize& outOffset, TileManager::Tile tile) const ETNoexceptHint {
		const auto candidate(_cachedPagesByTile.Find(tile));
		if (ET_UNLIKELY(candidate == _cachedPagesByTile.End())) {
			return false;
		}

		outOffset = candidate->second;
		return true;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint const TileManager& SparseShaderImage::GetTileManager() const ETNoexceptHint {
		return _tileManager;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint SparseShaderImage::CacheResult SparseShaderImage::MakeResident(const HostSparseTileCache& cache, TileManager::Tile tile) {
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

	ETConstexpr ETInlineHint ETForceInlineHint Framebuffer::Image::operator VkImage() const ETNoexceptHint {
		return _image;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint VkExtent2D Framebuffer::Pass::GetRenderExtent() const ETNoexceptHint {
		return { scaler.GetCurrentWidth(), scaler.GetCurrentHeight() };
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint VkQueryPool Framebuffer::GetTimingPool() const ETNoexceptHint {
		return _timingPool;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Range<const Framebuffer::Pass*> Framebuffer::GetPasses() const ETNoexceptHint {
		return { _passes.Begin<Pass>(), _passes.End<Pass>() };
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkResult Viewport::Resize(Gpu& gpu, VkExtent2D extent, uint32 layers, ExternalImageMap externalImages) {
		const auto generator(_generator.load(std::memory_order_consume));
		return generator ? this->BindResources(gpu, *generator, extent, layers, eastl::move(externalImages)) : VK_SUCCESS;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Viewport::TryAcquire(const GraphicsPipelineBuilder& generator) ETNoexceptHint {
		const GraphicsPipelineBuilder* expected(nullptr);
		return _generator.compare_exchange_strong(expected, ETAddressOf(generator), std::memory_order_acq_rel);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Viewport::Release() ETNoexceptHint {
		_generator.store(nullptr, std::memory_order_release);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint void ViewportDisposer::operator()(Viewport* viewport) const ETNoexceptHint {
		viewport->Release();
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint size_t GetHashCode(TileManager::Tile tile, size_t seed = 0u) ETNoexceptHint {
		return ((tile.x * 73856093u) ^ (tile.y * 19349663u) ^ (tile.z * 83492791u)) + tile.subresource + 31u * seed;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator<(TileManager::Tile left, TileManager::Tile right) ETNoexceptHint {
		return reinterpret_cast<const uint64&>(left) < reinterpret_cast<const uint64&>(right);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(TileManager::Tile left, TileManager::Tile right) ETNoexceptHint {
		return reinterpret_cast<const uint64&>(left) == reinterpret_cast<const uint64&>(right);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator!=(TileManager::Tile left, TileManager::Tile right) ETNoexceptHint {
		return !(left == right);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
