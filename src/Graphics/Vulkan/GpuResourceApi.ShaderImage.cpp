/*==================================================================*\
  GpuResourceApi.ShaderImage.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ShaderImage::ShaderImage(ShaderImage&& image) ETNoexceptHint : ShaderImage() {
		Swap(*this, image);
	}

	// ---------------------------------------------------

	VkResult ShaderImage::BindResources(Gpu& gpu, const ImageDescriptor& description) {
		enum : VkMemoryPropertyFlags { InferFromUsage = 0u };

		const VkExtent3D extent{ description.size.width, description.size.height, description.size.depth };

		return GpuImage::BindResources(
			gpu,
			VkImageCreateInfo{
				VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u, // VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT?
				GetImageType(extent),
				AsVkFormat(description.format),
				extent,
				description.smallestMip,
				description.slices,
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
				/*pool =*/NullVulkanHandle,
				/*pUserData =*/this });
	}

	// ---------------------------------------------------

	void Swap(ShaderImage& lhs, ShaderImage& rhs) ETNoexceptHint {
		Swap(static_cast<GpuImage&>(lhs), static_cast<GpuImage&>(rhs));
	}

}}} // namespace Eldritch2::Graphics::Vulkan
