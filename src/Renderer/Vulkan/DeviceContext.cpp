/*==================================================================*\
  DeviceContext.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/DeviceContext.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	DeviceContext::DeviceContext( UniquePointer<::VkDevice>&& device, Allocator& allocator ) : _device( ::std::move( device ) ), _allocator( allocator, UTF8L("Vulkan Device Context Allocator") ) {}

// ---------------------------------------------------

	VulkanResult<::VkDeviceMemory> DeviceContext::AllocateMemory( ::VkDeviceSize allocationSizeInBytes, uint32 memoryTypeIndex ) {
		VkMemoryAllocateInfo	allocateInfo;
		::VkDeviceMemory		deviceMemory( nullptr );

		allocateInfo.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.pNext				= nullptr;
		allocateInfo.allocationSize		= allocationSizeInBytes;
		allocateInfo.memoryTypeIndex	= memoryTypeIndex;
		
		const auto	allocateMemoryResult( ::vkAllocateMemory( _device.get(), &allocateInfo, _allocator, &deviceMemory ) );

		if( allocateMemoryResult < VK_SUCCESS ) {
			return { { deviceMemory, { _device.get(), _allocator } } };
		}

		return { allocateMemoryResult };
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2



