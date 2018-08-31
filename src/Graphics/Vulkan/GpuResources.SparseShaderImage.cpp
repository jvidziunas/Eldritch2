/*==================================================================*\
  GpuResources.SparseImage.cpp
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

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	SparseShaderImage::SparseShaderImage(SparseShaderImage&& image) :
		SparseShaderImage() {
		Swap(*this, image);
	}

	// ---------------------------------------------------

	VkResult SparseShaderImage::BindResources(Gpu& gpu, VkFormat format, VkExtent3D tileExtent, VkExtent3D extent, uint32_t mips) {
		enum : VkMemoryPropertyFlags { InferFromUsage = 0u };

		TileManager tileManager(format, tileExtent, extent);
		ET_ABORT_UNLESS(AbstractImage::BindResources(
			gpu,
			VkImageCreateInfo {
				VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				/*pNext =*/nullptr,
				VK_IMAGE_CREATE_SPARSE_BINDING_BIT | VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT,
				GetImageType(extent),
				format,
				extent,
				mips,
				/*arrayLayers =*/1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
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
		AbstractImage::FreeResources(gpu);
		// _tileManager.Clear();
	}

	// ---------------------------------------------------

	void Swap(SparseShaderImage& lhs, SparseShaderImage& rhs) {
		using ::Eldritch2::Swap;

		Detail::Swap(lhs, rhs);
		Swap(lhs._tileManager, rhs._tileManager);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
