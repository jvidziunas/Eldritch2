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
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	UniformBuffer::UniformBuffer(UniformBuffer&& buffer) :
		UniformBuffer() {
		Swap(*this, buffer);
	}

	// ---------------------------------------------------

	VkResult UniformBuffer::BindResources(Gpu& gpu, VkDeviceSize sizeInBytes) {
		enum : VkMemoryPropertyFlags { InferFromUsage = 0u };

		return AbstractBuffer::BindResources(
			gpu,
			VkBufferCreateInfo{
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u,
				sizeInBytes,
				VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				/*queueFamilyIndexCount =*/0u,    // Exclusive sharing.
				/*pQueueFamilyIndices =*/nullptr, // Exclusive sharing.
			},
			VmaAllocationCreateInfo{
				VMA_ALLOCATION_CREATE_MAPPED_BIT,
				VMA_MEMORY_USAGE_CPU_TO_GPU,
				/*requiredFlags =*/InferFromUsage,
				/*preferredFlags =*/InferFromUsage,
				/*memoryTypeBits =*/0u,
				/*pool =*/VK_NULL_HANDLE,
				/*pUserData =*/this });
	}

	// ---------------------------------------------------

	void Swap(UniformBuffer& lhs, UniformBuffer& rhs) {
		Detail::Swap(lhs, rhs);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
