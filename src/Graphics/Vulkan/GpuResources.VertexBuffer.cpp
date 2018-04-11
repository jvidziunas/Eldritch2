/*==================================================================*\
  GpuResources.VertexBuffer.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	VkResult VertexBuffer::BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes ) {
		enum : VkMemoryPropertyFlags { InferMemoryPropertiesFromUsage = 0u };

		return AbstractBuffer::BindResources(
			heap,
			VkBufferCreateInfo{
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				nullptr,	// No extension info.
				0u,			// No flags.
				sizeInBytes,
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0u, nullptr	// No shared queue families.
			},
			VmaAllocationCreateInfo{
				0u,
				VMA_MEMORY_USAGE_GPU_ONLY,
				InferMemoryPropertiesFromUsage,
				InferMemoryPropertiesFromUsage,
				VK_NULL_HANDLE,	// Use default pool.
				nullptr
			}
		);
	}

// ---------------------------------------------------

	void Swap( VertexBuffer& lhs, VertexBuffer& rhs ) {
		Detail::Swap( lhs, rhs );
	}
	
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2