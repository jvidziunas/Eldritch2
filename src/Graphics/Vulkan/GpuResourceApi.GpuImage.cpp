/*==================================================================*\
  GpuResourceApi.GpuImage.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
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
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	GpuImage::~GpuImage() {
		ETAssert(_image == NullVulkanHandle, "Leaking Vulkan image {}!", fmt::ptr(_image));
		ETAssert(_backing == NullVulkanHandle, "Leaking Vulkan allocation {}!", fmt::ptr(_backing));
	}

	// ---------------------------------------------------

	VkResult GpuImage::BindResources(Gpu& gpu, const VkImageCreateInfo& imageInfo, const VmaAllocationCreateInfo& allocationInfo) {
		VkImage image;
		ET_ABORT_UNLESS(vkCreateImage(gpu, ETAddressOf(imageInfo), gpu.GetAllocationCallbacks(), ETAddressOf(image)));
		ET_AT_SCOPE_EXIT(vkDestroyImage(gpu, image, gpu.GetAllocationCallbacks()));

		VmaAllocation backing;
		ET_ABORT_UNLESS(gpu.AllocateMemory(backing, allocationInfo, image));

		return GpuImage::BindResources(gpu, Exchange(image, NullVulkanHandle), backing);
	}

	// ---------------------------------------------------

	VkResult GpuImage::BindResources(Gpu& gpu, VkImage image, VmaAllocation backing) ETNoexceptHint {
		gpu.Finalize([oldImage = Exchange(_image, image), oldBacking = Exchange(_backing, backing)](Gpu& gpu) ETNoexceptHint {
			if (oldBacking) {
				vkDestroyImage(gpu, oldImage, gpu.GetAllocationCallbacks());
				gpu.DeallocateMemory(oldBacking);
			}
		});

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GpuImage::FreeResources(Gpu& gpu) ETNoexceptHint {
		GpuImage::BindResources(gpu, NullVulkanHandle, NullVulkanHandle);
	}

	// ---------------------------------------------------

	void Swap(GpuImage& lhs, GpuImage& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._image, rhs._image);
		Swap(lhs._backing, rhs._backing);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
