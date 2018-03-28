/*==================================================================*\
  GpuResources.IndexBuffer.cpp
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

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {


	VkResult IndexBuffer::BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes ) {
		enum : VkMemoryPropertyFlags { InferMemoryPropertiesFromUsage = 0u };

		return AbstractBuffer::BindResources(
			heap,
			VkBufferCreateInfo{
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				nullptr,		// No extension data.
				0u,				// Default flags.
				sizeInBytes,
				VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0u, nullptr,	// Exclusive sharing.
			},
			VmaAllocationCreateInfo{
				0u,
				VMA_MEMORY_USAGE_GPU_ONLY,
				InferMemoryPropertiesFromUsage,
				InferMemoryPropertiesFromUsage,
				VK_NULL_HANDLE,	// Use default pool scheme.
				nullptr			// No user data.
			}
		);
	}

// ---------------------------------------------------

	void Swap( IndexBuffer& buffer0, IndexBuffer& buffer1 ) {
		Detail::Swap( buffer0, buffer1 );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2