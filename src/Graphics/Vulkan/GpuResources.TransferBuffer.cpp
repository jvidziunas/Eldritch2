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

	TransferBuffer::TransferBuffer( TransferBuffer&& buffer ) : _backing( eastl::exchange( buffer._backing, nullptr ) ), _buffer( eastl::exchange( buffer._buffer, nullptr ) ) {}

// ---------------------------------------------------

	TransferBuffer::TransferBuffer() : _backing( nullptr ), _buffer( nullptr ) {}

// ---------------------------------------------------

	TransferBuffer::~TransferBuffer() {
		ET_ASSERT( _backing == nullptr,	"Leaking Vulkan allocation!" );
		ET_ASSERT( _buffer == nullptr,	"Leaking Vulkan buffer object!" );
	}

// ---------------------------------------------------

	VkResult TransferBuffer::BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes ) {
		using ::Eldritch2::Swap;

		const VmaAllocationCreateInfo allocationInfo{
			VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VMA_MEMORY_USAGE_CPU_ONLY,
			0u, 0u, VK_NULL_HANDLE, nullptr
		};

		const VkBufferCreateInfo bufferInfo{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr,	// No extension data.
			0u,			// No create flags.
			sizeInBytes,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0u, nullptr	// No queues.
		};

		VmaAllocation	backing;
		VkBuffer		buffer;
		ET_FAIL_UNLESS( vmaCreateBuffer( heap, &bufferInfo, &allocationInfo, &buffer, &backing, nullptr ) );
		ET_AT_SCOPE_EXIT( vmaDestroyBuffer( heap, buffer, backing ) );

		Swap( _backing,	backing );
		Swap( _buffer,	buffer );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void TransferBuffer::FreeResources( GpuHeap& heap ) {
		if (VkBuffer buffer = eastl::exchange( _buffer, nullptr )) {
			heap.AddGarbage( { buffer, eastl::exchange( _backing, nullptr ) } );
		}
	}

// ---------------------------------------------------

	void Swap( TransferBuffer& buffer0, TransferBuffer& buffer1 ) {
		using ::Eldritch2::Swap;

		Swap( buffer0._backing, buffer1._backing );
		Swap( buffer0._buffer,	buffer1._buffer );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2