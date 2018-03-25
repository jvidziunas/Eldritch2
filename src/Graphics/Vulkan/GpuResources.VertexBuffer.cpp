/*==================================================================*\
  GpuResources.VertexBuffer.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

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

	VertexBuffer::VertexBuffer( VertexBuffer&& ) {}

// ---------------------------------------------------

	VertexBuffer::VertexBuffer() {}

// ---------------------------------------------------

	VertexBuffer::~VertexBuffer() {
		ET_ASSERT( _backing == nullptr, "Leaking Vulkan allocation!" );
		ET_ASSERT( _buffer == nullptr,  "Leaking Vulkan buffer!" );
	}

// ---------------------------------------------------

	VkResult VertexBuffer::BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes ) {
		using ::Eldritch2::Swap;

		const VkBufferCreateInfo bufferInfo{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr,	// No extension info
			0u,			// No flags.
			sizeInBytes,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0u, nullptr	// No shared queue families.
		};

		const VmaAllocationCreateInfo allocationInfo{
			0u,
			VMA_MEMORY_USAGE_GPU_ONLY
		};

		VmaAllocation backing;
		VkBuffer buffer;

		ET_FAIL_UNLESS( vmaCreateBuffer( heap, &bufferInfo, &allocationInfo, &buffer, &backing, nullptr ) );
		ET_AT_SCOPE_EXIT( vmaDestroyBuffer( heap, buffer, backing ) );

		Swap( _backing, backing );
		Swap( _buffer,  buffer );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void VertexBuffer::FreeResources( GpuHeap& heap ) {
		if (VmaAllocation backing = eastl::exchange( _backing, nullptr )) {
			heap.AddGarbage( eastl::exchange( _buffer, nullptr ), backing );
		}
	}

// ---------------------------------------------------

	void Swap( VertexBuffer& buffer0, VertexBuffer& buffer1 ) {
		using ::Eldritch2::Swap;

		Swap( buffer0._backing, buffer1._backing );
		Swap( buffer0._buffer,  buffer1._buffer );
	}
	
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2