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
#include <Renderer/Vulkan/VulkanProperties.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "vulkan-1.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	PhysicalDeviceProperties::PhysicalDeviceProperties( ::VkPhysicalDevice device ) {
		::vkGetPhysicalDeviceProperties( device, &_properties );
		::vkGetPhysicalDeviceMemoryProperties( device, &_memoryProperties );
	}

// ---------------------------------------------------

	uint32 PhysicalDeviceProperties::GetIndexOfLargestHeapSupportingFlags( ::VkMemoryPropertyFlags flags ) const {
		VkDeviceSize	selectedHeapSize( 0u );
		uint32			selectedHeapIndex( static_cast<uint32>(-1) );

		for( uint32 heapIndex( 0u ); heapIndex < _memoryProperties.memoryHeapCount; ++heapIndex ) {
			auto&	heapDescriptor( _memoryProperties.memoryHeaps[heapIndex] );

			if( (flags == (heapDescriptor.flags & flags)) && (heapDescriptor.size > selectedHeapSize) ) {
				selectedHeapIndex	= heapIndex;
				selectedHeapSize	= heapDescriptor.size;
			}
		}

		return selectedHeapIndex;
	}

// ---------------------------------------------------

	bool PhysicalDeviceProperties::DescribesNamedDevice( const char* adapterName ) const {
		return EqualityCompareStringCaseInsensitive( _properties.deviceName, adapterName );
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2