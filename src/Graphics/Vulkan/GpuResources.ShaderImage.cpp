/*==================================================================*\
  GpuResources.ShaderImage.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
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

	VkResult ShaderImage::BindResources( GpuHeap& heap, VkExtent3D extent, uint32_t mips, uint32_t arrayLayers ) {
		enum : VkMemoryPropertyFlags { InferMemoryPropertiesFromUsage = 0u };

		return AbstractImage::BindResources(
			heap,
			VkImageCreateInfo{
				VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				nullptr,
				0u,						// Standard flags-- VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT?
				GetImageType( extent ),
				VK_FORMAT_UNDEFINED,
				extent,
				mips,					// Mips
				arrayLayers,			// Array layers
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
		);
	}

// ---------------------------------------------------

	void Swap( ShaderImage& image0, ShaderImage& image1 ) {
		Detail::Swap( image0, image1 );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2