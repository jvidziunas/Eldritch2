/*==================================================================*\
  GpuResources.AbstractBuffer.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
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
namespace Detail {

	AbstractBuffer::AbstractBuffer( AbstractBuffer&& buffer ) : _backing( eastl::exchange( buffer._backing, nullptr ) ), _buffer( eastl::exchange( buffer._buffer, nullptr ) ) {}

// ---------------------------------------------------

	AbstractBuffer::AbstractBuffer() : _backing( nullptr ), _buffer( nullptr ) {}

// ---------------------------------------------------

	AbstractBuffer::~AbstractBuffer() {
		ET_ASSERT( _buffer == nullptr,  "Leaking Vulkan buffer!" );
		ET_ASSERT( _backing == nullptr, "Leaking Vulkan allocation!" );
	}

// ---------------------------------------------------

	void AbstractBuffer::FreeResources( GpuHeap& heap ) {
		if (VkBuffer buffer = eastl::exchange( _buffer, nullptr )) {
			heap.AddGarbage( buffer, eastl::exchange( _backing, nullptr ) );
		}
	}

// ---------------------------------------------------

	VkResult AbstractBuffer::BindResources( GpuHeap& heap, const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocationInfo ) {
		using ::Eldritch2::Swap;

		VmaAllocation backing;
		VkBuffer buffer;

		ET_FAIL_UNLESS( vmaCreateBuffer( heap, &bufferInfo, &allocationInfo, &buffer, &backing, nullptr ) );
		ET_AT_SCOPE_EXIT( vmaDestroyBuffer( heap, buffer, backing ) );

		Swap( _buffer,	buffer );
		Swap( _backing, backing );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void AbstractBuffer::GetAllocationInfo( GpuHeap& heap, VmaAllocationInfo& info ) {
		vmaGetAllocationInfo( heap, _backing, &info );
	}

// ---------------------------------------------------

	void Swap( AbstractBuffer& buffer0, AbstractBuffer& buffer1 ) {
		using ::Eldritch2::Swap;

		Swap( buffer0._buffer,  buffer1._buffer );
		Swap( buffer0._backing, buffer1._backing );
	}

}	// namespace Detail
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2