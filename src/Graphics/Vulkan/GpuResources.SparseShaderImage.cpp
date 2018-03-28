/*==================================================================*\
  ResidencyCoordinator.SparseImage.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	SparseShaderImage::SparseShaderImage( SparseShaderImage&& image ) : Detail::AbstractImage( eastl::move( image ) ), _pageManager( eastl::move( image._pageManager ) ), _cache( eastl::move( image._cache ) ) {}

// ---------------------------------------------------

	SparseShaderImage::SparseShaderImage() : _pageManager(), _cache() {}

// ---------------------------------------------------

	bool SparseShaderImage::MakeResident( SparsePageManager::Tile tile ) {
		return _pageManager.MakeResident( tile );
	}

// ---------------------------------------------------

	VkResult SparseShaderImage::BindResources( GpuHeap& heap, VkExtent3D tileExtent, VkExtent3D extent, uint32_t mips ) {
		const VkDeviceSize	tileSizeInBytes( tileExtent.width * tileExtent.height * tileExtent.depth );

		SparsePageManager pageManager( tileExtent, extent );
		TransferBuffer cache;
		ET_FAIL_UNLESS( cache.BindResources( heap, tileSizeInBytes * 128u ) );
		ET_AT_SCOPE_EXIT( cache.FreeResources( heap ) );

		Swap( _pageManager, pageManager );
		Swap( _cache,       cache );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void SparseShaderImage::FreeResources( GpuHeap& heap ) {
		AbstractImage::FreeResources( heap );
	}

// ---------------------------------------------------

	void Swap( SparseShaderImage& image0, SparseShaderImage& image1 ) {
		Detail::Swap( image0, image1 );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2