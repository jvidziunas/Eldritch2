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

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ShaderImage::ShaderImage(ShaderImage&& image) :
		ShaderImage() {
		Swap(*this, image);
	}

	// ---------------------------------------------------

	VkResult ShaderImage::BindResources(Gpu& gpu, GpuFormat format, VkExtent3D extent, uint32_t mips, uint32_t arrayLayers) {
		enum : VkMemoryPropertyFlags { InferFromUsage = 0u };

		return AbstractImage::BindResources(
			gpu,
			VkImageCreateInfo{
				VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u, // VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT?
				GetImageType(extent),
				TextureFormats[size_t(format)].vkFormat,
				extent,
				mips,
				arrayLayers,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				/*queueFamilyIndexCount =*/0u,    // Exclusive sharing.
				/*pQueueFamilyIndices =*/nullptr, // Exclusive sharing.
				VK_IMAGE_LAYOUT_UNDEFINED },
			VmaAllocationCreateInfo{
				/*flags =*/0u,
				VMA_MEMORY_USAGE_GPU_ONLY,
				/*requiredFlags =*/InferFromUsage,
				/*preferredFlags =*/InferFromUsage,
				/*memoryTypeBits =*/0u,
				/*pool =*/VK_NULL_HANDLE,
				/*pUserData =*/this });
	}

	// ---------------------------------------------------

	void Swap(ShaderImage& lhs, ShaderImage& rhs) {
		Detail::Swap(lhs, rhs);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
