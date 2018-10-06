/*==================================================================*\
  GpuResourceApi.SparseShaderImage.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	SparseShaderImage::SparseShaderImage(SparseShaderImage&& image) :
		SparseShaderImage() {
		Swap(*this, image);
	}

	// ---------------------------------------------------

	VkResult SparseShaderImage::BindResources(Gpu& gpu, VkFormat format, VkExtent3D extent, uint32 mips, uint32 layers) {
		enum : VkMemoryPropertyFlags { InferFromUsage = 0u };
		static ETConstexpr VkSampleCountFlagBits SampleFlags(VK_SAMPLE_COUNT_1_BIT);
		static ETConstexpr VkImageUsageFlags UsageFlags(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
		static ETConstexpr VkImageTiling ImageTiling(VK_IMAGE_TILING_OPTIMAL);
		const VkImageType                imageType(GetImageType(extent));
		uint32_t                         propertyCount(0u);

		vkGetPhysicalDeviceSparseImageFormatProperties(gpu, format, imageType, SampleFlags, UsageFlags, ImageTiling, ETAddressOf(propertyCount), /*pProperties =*/nullptr);
		const auto properties(ETStackAlloc(VkSparseImageFormatProperties, propertyCount));
		vkGetPhysicalDeviceSparseImageFormatProperties(gpu, format, imageType, SampleFlags, UsageFlags, ImageTiling, ETAddressOf(propertyCount), properties);
		const auto imageProperties(Find(properties, properties + propertyCount, [](const VkSparseImageFormatProperties& format) {
			return (format.aspectMask & VK_IMAGE_ASPECT_COLOR_BIT) != 0u;
		}));

		TileManager tileManager;
		ET_ABORT_UNLESS(tileManager.BindResources(gpu, extent, imageProperties->imageGranularity));
		ET_ABORT_UNLESS(GpuImage::BindResources(
			gpu,
			VkImageCreateInfo {
				VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				/*pNext =*/nullptr,
				VK_IMAGE_CREATE_SPARSE_BINDING_BIT | VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT,
				imageType,
				format,
				extent,
				mips,
				layers,
				SampleFlags,
				ImageTiling,
				UsageFlags,
				VK_SHARING_MODE_EXCLUSIVE,
				/*queueFamilyIndexCount =*/0u,    // Exclusive sharing.
				/*pQueueFamilyIndices =*/nullptr, // Exclusive sharing.
				VK_IMAGE_LAYOUT_UNDEFINED },
			VmaAllocationCreateInfo {
				/*flags =*/0u,
				VMA_MEMORY_USAGE_GPU_ONLY,
				/*requiredFlags =*/InferFromUsage,
				/*preferredFlags =*/InferFromUsage,
				/*memoryTypeBits =*/0u,
				/*pool =*/VK_NULL_HANDLE,
				/*pUserData =*/this }));
		Swap(_tileManager, tileManager);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void SparseShaderImage::FreeResources(Gpu& gpu) {
		_tileManager.FreeResources(gpu);
		GpuImage::FreeResources(gpu);
	}

	// ---------------------------------------------------

	void Swap(SparseShaderImage& lhs, SparseShaderImage& rhs) {
		using ::Eldritch2::Swap;

		Swap(static_cast<GpuImage&>(lhs), rhs);
		Swap(lhs._tileManager, rhs._tileManager);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
