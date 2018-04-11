/*==================================================================*\
  GpuResources.UniformBuffer.cpp
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

	VkResult UniformBuffer::BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes ) {
		enum : VkMemoryPropertyFlags { InferMemoryPropertiesFromUsage = 0u };

		return AbstractBuffer::BindResources(
			heap,
			VkBufferCreateInfo{
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				nullptr,								// No extension data.
				0u,										// No create flags.
				sizeInBytes,
				VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0, nullptr								// No queue families.
			},
			VmaAllocationCreateInfo{
				VMA_ALLOCATION_CREATE_MAPPED_BIT,
				VMA_MEMORY_USAGE_CPU_TO_GPU,
				InferMemoryPropertiesFromUsage,
				InferMemoryPropertiesFromUsage,
				0u,
				VK_NULL_HANDLE,	// Use default pool.
				nullptr
			}
		);
	}

// ---------------------------------------------------

	void Swap( UniformBuffer& lhs, UniformBuffer& rhs ) {
		Detail::Swap( lhs, rhs );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2