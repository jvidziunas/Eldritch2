/*==================================================================*\
  VulkanProperties.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/DeviceProperties.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "vulkan-1.lib" )
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	PhysicalDeviceProperties::PhysicalDeviceProperties( VkPhysicalDevice device ) {
		vkGetPhysicalDeviceProperties( device, &properties );
		vkGetPhysicalDeviceMemoryProperties( device, &memoryProperties );
	}

// ---------------------------------------------------

	uint32 PhysicalDeviceProperties::GetIndexOfLargestHeapSupportingFlags( VkMemoryPropertyFlags flags ) const {
		VkDeviceSize	selectedHeapSize( 0u );
		uint32_t		selectedHeapIndex( static_cast<uint32>(-1) );

		for( uint32_t heapIndex( 0u ); heapIndex < memoryProperties.memoryHeapCount; ++heapIndex ) {
			auto&	heapDescriptor( memoryProperties.memoryHeaps[heapIndex] );

			if( (flags == (heapDescriptor.flags & flags)) && (heapDescriptor.size > selectedHeapSize) ) {
				selectedHeapIndex	= heapIndex;
				selectedHeapSize	= heapDescriptor.size;
			}
		}

		return selectedHeapIndex;
	}

// ---------------------------------------------------

	bool PhysicalDeviceProperties::DescribesNamedDevice( const char* adapterName ) const {
		return EqualityCompareStringCaseInsensitive( properties.deviceName, adapterName );
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2