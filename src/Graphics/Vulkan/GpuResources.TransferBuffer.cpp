/*==================================================================*\
  GpuResources.TransferBuffer.cpp
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
#include <Graphics/Vulkan/GpuHeap.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	VkResult TransferBuffer::BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes ) {
		enum : VkMemoryPropertyFlags { InferMemoryPropertiesFromUsage = 0u };

		return AbstractBuffer::BindResources(
			heap,
			VkBufferCreateInfo{
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				nullptr,	// No extension data.
				0u,			// No create flags.
				sizeInBytes,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0u, nullptr	// No queues.
			},
			VmaAllocationCreateInfo{
				VMA_ALLOCATION_CREATE_MAPPED_BIT,
				VMA_MEMORY_USAGE_CPU_ONLY,
				InferMemoryPropertiesFromUsage,
				InferMemoryPropertiesFromUsage,
				VK_NULL_HANDLE,
				nullptr
			}
		);
	}

// ---------------------------------------------------

	void Swap( TransferBuffer& lhs, TransferBuffer& rhs ) {
		Detail::Swap( lhs, rhs );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2