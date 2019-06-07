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

	ETConstexpr ETForceInlineHint GpuBuffer::operator VkBuffer() const ETNoexceptHint {
		return _buffer;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkBuffer GpuBuffer::Get() const ETNoexceptHint {
		return _buffer;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint bool GpuBuffer::IsBound() const ETNoexceptHint {
		return _buffer != VK_NULL_HANDLE;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint GpuImage::GpuImage() ETNoexceptHint : _backing(VK_NULL_HANDLE), _image(VK_NULL_HANDLE) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint GpuImage::operator VkImage() const ETNoexceptHint {
		return _image;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkImage GpuImage::Get() const ETNoexceptHint {
		return _image;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint bool GpuImage::IsBound() const ETNoexceptHint {
		return _image != VK_NULL_HANDLE;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkBuffer GeometryCache::GetVertexBuffer() const ETNoexceptHint {
		return _vertices;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkBuffer GeometryCache::GetIndexBuffer() const ETNoexceptHint {
		return _indices;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkBuffer Mesh::GetVertices() const ETNoexceptHint {
		return _vertices;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkBuffer Mesh::GetIndices() const ETNoexceptHint {
		return _indices;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkExtent3D TileManager::GetTileExtent() const ETNoexceptHint {
		return _tileExtent;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool HostSparseTileCache::ShouldDecode(VkDeviceSize& outOffset, ImageTile tile) const ETNoexceptHint {
		return true;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void HostSparseTileCache::NotifyCached(VkDeviceSize /*offset*/, ImageTile tile) {
		_loadingTiles.Erase(tile);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint const TileManager& SparseShaderImage::GetTileManager() const ETNoexceptHint {
		return _tileManager;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint SparseShaderImage::CacheResult SparseShaderImage::MakeResident(const HostSparseTileCache& cache, ImageTile tile) {
		if (_tileManager.Touch(tile)) {
			return CacheResult::Hit;
		}

		VkDeviceSize physicalTile;
		if (cache.ShouldDecode(physicalTile, tile)) {
			_tileManager.ReplaceOldest(tile, physicalTile);
			// -> Bind physical tile location to memory here <-
			return CacheResult::L0Miss;
		}

		return CacheResult::L1Miss;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkDescriptorSet DescriptorTable::GetDescriptorSet() const ETNoexceptHint {
		return _descriptors;
	}

	// ---------------------------------------------------

	template <typename InputIterator>
	ETForceInlineHint DescriptorTable::ResourceIndex DescriptorTable::ReserveSlots(InputIterator begin, InputIterator end) {
		const ResourceIndex slot(_images.GetSize());
		while (begin != end) {
			_images.EmplaceBack(static_cast<const GpuImage*>(*begin++), VkImageView(VK_NULL_HANDLE));
		}

		return slot;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Framebuffer::Image::Image(Image&& image) ETNoexceptHint : Image() {
		Swap(*this, image);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkExtent2D Framebuffer::StageFramebuffer::GetRenderExtent() const ETNoexceptHint {
		return { scaler.GetWidth(), scaler.GetHeight() };
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint Framebuffer::StageImageSet::StageImageSet() ETNoexceptHint : views{ VkImageView(VK_NULL_HANDLE) } {}
	
	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Framebuffer::StageImageSet::StageImageSet(StageImageSet&& images) ETNoexceptHint : StageImageSet() {
		Swap(*this, images);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint const DescriptorTable& Framebuffer::GetShaderResources() const ETNoexceptHint {
		return _shaderResources;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Span<const Framebuffer::StageFramebuffer*> Framebuffer::GetStages() const ETNoexceptHint {
		return { _stageResources.Begin<StageFramebuffer>(), _stageResources.End<StageFramebuffer>() };
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkQueryPool Framebuffer::GetTimingPool() const ETNoexceptHint {
		return _timingPool;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
