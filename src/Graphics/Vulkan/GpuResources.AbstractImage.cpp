/*==================================================================*\
  GpuResources.AbstractImage.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace Detail {

	AbstractImage::AbstractImage() :
		_backing(nullptr),
		_image(nullptr) {}

	// ---------------------------------------------------

	AbstractImage::~AbstractImage() {
		ET_ASSERT(_image == nullptr, "Leaking Vulkan image!");
		ET_ASSERT(_backing == nullptr, "Leaking Vulkan allocation!");
	}

	// ---------------------------------------------------

	void AbstractImage::FreeResources(Gpu& gpu) {
		if (VkImage image = eastl::exchange(_image, nullptr)) {
			gpu.AddGarbage(image, eastl::exchange(_backing, nullptr));
		}
	}

	// ---------------------------------------------------

	VkResult AbstractImage::BindResources(Gpu& gpu, const VkImageCreateInfo& imageInfo, const VmaAllocationCreateInfo& allocationInfo) {
		using ::Eldritch2::Swap;

		VmaAllocation backing;
		VkImage       image;

		ET_FAIL_UNLESS(vmaCreateImage(gpu, &imageInfo, &allocationInfo, &image, &backing, nullptr));
		ET_AT_SCOPE_EXIT(vmaDestroyImage(gpu, image, backing));

		Swap(_image, image);
		Swap(_backing, backing);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Swap(AbstractImage& lhs, AbstractImage& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._image, rhs._image);
		Swap(lhs._backing, rhs._backing);
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::Detail
