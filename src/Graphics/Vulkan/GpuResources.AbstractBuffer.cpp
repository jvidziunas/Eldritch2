/*==================================================================*\
  GpuResources.AbstractBuffer.cpp
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

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	AbstractBuffer::AbstractBuffer() :
		_backing(nullptr),
		_buffer(nullptr) {}

	// ---------------------------------------------------

	AbstractBuffer::~AbstractBuffer() {
		ET_ASSERT(_buffer == nullptr, "Leaking Vulkan buffer!");
		ET_ASSERT(_backing == nullptr, "Leaking Vulkan allocation!");
	}

	// ---------------------------------------------------

	void AbstractBuffer::FreeResources(Gpu& gpu) {
		if (VkBuffer buffer = eastl::exchange(_buffer, nullptr)) {
			gpu.AddGarbage(buffer, eastl::exchange(_backing, nullptr));
		}
	}

	// ---------------------------------------------------

	VkResult AbstractBuffer::BindResources(Gpu& gpu, const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocationInfo) {
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

	void AbstractBuffer::GetAllocationInfo(Gpu& gpu, VmaAllocationInfo& outInfo) {
		vmaGetAllocationInfo(gpu, _backing, ETAddressOf(outInfo));
	}

	// ---------------------------------------------------

	VkResult AbstractBuffer::MapHostPointer(Gpu& gpu, void*& outBase) const {
		return vmaMapMemory(gpu, _backing, ETAddressOf(outBase));
	}

	// ---------------------------------------------------

	void AbstractBuffer::UnmapHostPointer(Gpu& gpu) const {
		vmaUnmapMemory(gpu, _backing);
	}

	// ---------------------------------------------------

	void Swap(AbstractBuffer& lhs, AbstractBuffer& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._buffer, rhs._buffer);
		Swap(lhs._backing, rhs._backing);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
