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

	UniformBuffer::UniformBuffer( UniformBuffer&& buffer ) : _buffer( eastl::exchange( buffer._buffer, nullptr ) ), _backing( eastl::exchange( buffer._backing, nullptr ) ) {}
	
// ---------------------------------------------------

	UniformBuffer::UniformBuffer() : _buffer( nullptr ), _backing( nullptr ) {}

// ---------------------------------------------------

	UniformBuffer::~UniformBuffer() {
		ET_ASSERT( _buffer == nullptr,	"Leaking Vulkan buffer!" );
		ET_ASSERT( _backing == nullptr, "Leaking Vulkan allocation!" );
	}

// ---------------------------------------------------

	VkResult UniformBuffer::BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes ) {
		using ::Eldritch2::Swap;

		const VkBufferCreateInfo bufferInfo{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr,								// No extension data.
			0u,										// No create flags.
			sizeInBytes,
			VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0, nullptr								// No queue families.
		};

		const VmaAllocationCreateInfo allocationInfo{
			VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VMA_MEMORY_USAGE_CPU_TO_GPU,
			0u, 0u, VK_NULL_HANDLE, nullptr
		};

		VmaAllocation backing;
		VkBuffer buffer;

		ET_FAIL_UNLESS( vmaCreateBuffer( heap, &bufferInfo, &allocationInfo, &buffer, &backing, nullptr ) );
		ET_AT_SCOPE_EXIT( vmaDestroyBuffer( heap, buffer, backing ) );

		Swap( _buffer,	buffer );
		Swap( _backing, backing );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void UniformBuffer::FreeResources( GpuHeap& heap ) {
		if (VkBuffer buffer = eastl::exchange( _buffer, nullptr )) {
			heap.AddGarbage( { buffer, eastl::exchange( _backing, nullptr ) } );
		}
	}

// ---------------------------------------------------

	void Swap( UniformBuffer& buffer0, UniformBuffer& buffer1 ) {
		using ::Eldritch2::Swap;

		Swap( buffer0._buffer,	buffer1._buffer );
		Swap( buffer0._backing,	buffer1._backing );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2