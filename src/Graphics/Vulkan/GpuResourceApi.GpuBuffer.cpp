/*==================================================================*\
  GpuResourceApi.GpuBuffer.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	GpuBuffer::GpuBuffer() :
		_backing(nullptr),
		_buffer(nullptr) {}

	// ---------------------------------------------------

	GpuBuffer::~GpuBuffer() {
		ET_ASSERT(_buffer == nullptr, "Leaking Vulkan buffer!");
		ET_ASSERT(_backing == nullptr, "Leaking Vulkan allocation!");
	}

	// ---------------------------------------------------

	void GpuBuffer::FreeResources(Gpu& gpu) {
		if (VkBuffer buffer = eastl::exchange(_buffer, nullptr)) {
			gpu.AddGarbage(buffer, eastl::exchange(_backing, nullptr));
		}
	}

	// ---------------------------------------------------

	VkResult GpuBuffer::BindResources(Gpu& gpu, const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocationInfo) {
		using ::Eldritch2::Swap;

		VmaAllocation backing;
		VkBuffer      buffer;

		ET_ABORT_UNLESS(vmaCreateBuffer(gpu, ETAddressOf(bufferInfo), ETAddressOf(allocationInfo), ETAddressOf(buffer), ETAddressOf(backing), nullptr));
		ET_AT_SCOPE_EXIT(vmaDestroyBuffer(gpu, buffer, backing));

		Swap(_buffer, buffer);
		Swap(_backing, backing);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Swap(GpuBuffer& lhs, GpuBuffer& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._buffer, rhs._buffer);
		Swap(lhs._backing, rhs._backing);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
