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
		const VkBufferCreateInfo bufferInfo{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr,	// No extension info
			0u,
		};

		const VmaAllocationCreateInfo allocationInfo{
			0u,
			VMA_MEMORY_USAGE_GPU_ONLY,

		};

		vmaCreateBuffer( heap, &bufferInfo,  );
	}

// ---------------------------------------------------

	void VertexBuffer::FreeResources( GpuHeap& heap );

// ---------------------------------------------------

	VertexBuffer::operator VkBuffer();

// ---------------------------------------------------

		//!	Disable copy assignment.
		VertexBuffer&	operator=( const VertexBuffer& ) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VmaAllocation	_backing;
		VkBuffer		_buffer;

		// ---------------------------------------------------

		friend void	Swap( VertexBuffer&, VertexBuffer& );
	};
	
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2