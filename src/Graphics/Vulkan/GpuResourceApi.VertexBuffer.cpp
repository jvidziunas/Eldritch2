/*==================================================================*\
  GpuResourceApi.VertexBuffer.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	VertexBuffer::VertexBuffer(VertexBuffer&& buffer) :
		VertexBuffer() {
		Swap(*this, buffer);
	}

	// ---------------------------------------------------

	VkResult VertexBuffer::BindResources(Gpu& gpu, VkDeviceSize sizeInBytes) {
		enum : VkMemoryPropertyFlags { InferFromUsage = 0u };

		return GpuBuffer::BindResources(
			gpu,
			VkBufferCreateInfo {
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u,
				sizeInBytes,
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				/*queueFamilyIndexCount =*/0u,   // Exclusive sharing.
				/*pQueueFamilyIndices =*/nullptr // Exclusive sharing.
			},
			VmaAllocationCreateInfo {
				/*flags =*/0u,
				VMA_MEMORY_USAGE_GPU_ONLY,
				/*requiredFlags =*/InferFromUsage,
				/*preferredFlags =*/InferFromUsage,
				/*memoryTypeBits =*/0u,
				/*pool =*/VK_NULL_HANDLE,
				/*pUserData =*/this });
	}

	// ---------------------------------------------------

	void Swap(VertexBuffer& lhs, VertexBuffer& rhs) {
		Swap(static_cast<GpuBuffer&>(lhs), rhs);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
