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

	SparseShaderImage::SparseShaderImage( SparseShaderImage&& image ) : Detail::AbstractImage( eastl::move( image ) ), _tileManager( eastl::move( image._tileManager ) ), _pageCache( eastl::move( image._pageCache ) ) {}

// ---------------------------------------------------

	VkResult SparseShaderImage::BindResources( GpuHeap& heap, VkExtent3D tileExtent, VkExtent3D extent, uint32_t mips ) {
		enum : VkMemoryPropertyFlags { InferMemoryPropertiesFromUsage = 0u };

		const VkDeviceSize	tileSizeInBytes( tileExtent.width * tileExtent.height * tileExtent.depth );

		TileManager tileManager( tileExtent, extent );
		TransferBuffer tileCache;
		ET_FAIL_UNLESS( tileCache.BindResources( heap, tileSizeInBytes * 128u ) );
		ET_AT_SCOPE_EXIT( tileCache.FreeResources( heap ) );

		ET_FAIL_UNLESS( AbstractImage::BindResources(
			heap,
			VkImageCreateInfo{
				VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				nullptr,
				VK_IMAGE_CREATE_SPARSE_BINDING_BIT | VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT,
				GetImageType( extent ),
				VK_FORMAT_UNDEFINED,
				extent,
				mips,
				1u,						// Array layers
				VK_SAMPLE_COUNT_1_BIT,	// Shader resources are always 1 sample/texel.
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0u, nullptr,			// Exclusive sharing.
				VK_IMAGE_LAYOUT_UNDEFINED
			},
			VmaAllocationCreateInfo{
				0u,
				VMA_MEMORY_USAGE_GPU_ONLY,
				InferMemoryPropertiesFromUsage,
				InferMemoryPropertiesFromUsage,
				0u,				//	No type bits.
				VK_NULL_HANDLE,	//	Use default pooling scheme.
				nullptr			//	No user data.
			}
		) );

		Swap( _tileManager, tileManager );
		Swap( _pageCache,   tileCache );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void SparseShaderImage::FreeResources( GpuHeap& heap ) {
		AbstractImage::FreeResources( heap );
	}

// ---------------------------------------------------

	void Swap( SparseShaderImage& lhs, SparseShaderImage& rhs ) {
		using ::Eldritch2::Swap;

		Detail::Swap( lhs, rhs );

		Swap( lhs._tileManager, rhs._tileManager );
		Swap( lhs._pageCache,   rhs._pageCache );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2