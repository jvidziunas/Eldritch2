/*==================================================================*\
  GpuResourceApi.GpuBuffer.cpp
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

	GpuBuffer::GpuBuffer() ETNoexceptHint : _backing(NullVulkanHandle), _buffer(NullVulkanHandle) {}

	// ---------------------------------------------------

	GpuBuffer::~GpuBuffer() {
		ETAssert(_buffer == NullVulkanHandle, "Leaking Vulkan buffer {}!", fmt::ptr(_buffer));
		ETAssert(_backing == NullVulkanHandle, "Leaking Vulkan allocation {}!", fmt::ptr(_backing));
	}

	// ---------------------------------------------------

	VkResult GpuBuffer::BindResources(Gpu& gpu, const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocationInfo) {
		VkBuffer buffer;
		ET_ABORT_UNLESS(vkCreateBuffer(gpu, ETAddressOf(bufferInfo), gpu.GetAllocationCallbacks(), ETAddressOf(buffer)));
		ET_AT_SCOPE_EXIT(vkDestroyBuffer(gpu, buffer, gpu.GetAllocationCallbacks()));

		VmaAllocation backing;
		ET_ABORT_UNLESS(gpu.AllocateMemory(backing, allocationInfo, buffer));

		return GpuBuffer::BindResources(gpu, Exchange(buffer, NullVulkanHandle), backing);
	}

	// ---------------------------------------------------

	VkResult GpuBuffer::BindResources(Gpu& gpu, VkBuffer buffer, VmaAllocation backing) ETNoexceptHint {
		gpu.Finalize([oldBuffer = Exchange(_buffer, buffer), oldBacking = Exchange(_backing, backing)](Gpu& gpu) ETNoexceptHint {
			if (oldBacking == NullVulkanHandle) {
				return;
			}

			vkDestroyBuffer(gpu, oldBuffer, gpu.GetAllocationCallbacks());
			gpu.DeallocateMemory(oldBacking);
		});

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GpuBuffer::FreeResources(Gpu& gpu) ETNoexceptHint {
		GpuBuffer::BindResources(gpu, NullVulkanHandle, NullVulkanHandle);
	}

	// ---------------------------------------------------

	void Swap(GpuBuffer& lhs, GpuBuffer& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._buffer, rhs._buffer);
		Swap(lhs._backing, rhs._backing);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
