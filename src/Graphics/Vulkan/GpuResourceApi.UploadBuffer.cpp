/*==================================================================*\
  GpuResourceApi.UploadBuffer.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
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
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	UploadBuffer::UploadBuffer(UploadBuffer&& buffer) ETNoexceptHint : UploadBuffer() {
		Swap(*this, buffer);
	}

	// ---------------------------------------------------

	VkResult UploadBuffer::BindResources(Gpu& gpu, VkDeviceSize sizeInBytes) {
		enum : VkMemoryPropertyFlags { InferFromUsage = 0u };

		return GpuBuffer::BindResources(
			gpu,
			VkBufferCreateInfo{
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u,
				sizeInBytes,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				/*queueFamilyIndexCount =*/0u,    // Exclusive sharing.
				/*pQueueFamilyIndices =*/nullptr, // Exclusive sharing.
			},
			VmaAllocationCreateInfo{
				VMA_ALLOCATION_CREATE_MAPPED_BIT,
				VMA_MEMORY_USAGE_CPU_ONLY,
				/*requiredFlags =*/InferFromUsage,
				/*preferredFlags =*/InferFromUsage,
				/*memoryTypeBits =*/0u,
				/*pool =*/VK_NULL_HANDLE,
				/*pUserData =*/this });
	}

	// ---------------------------------------------------

	void Swap(UploadBuffer& lhs, UploadBuffer& rhs) ETNoexceptHint {
		Swap(static_cast<GpuBuffer&>(lhs), static_cast<GpuBuffer&>(rhs));
	}

}}} // namespace Eldritch2::Graphics::Vulkan
