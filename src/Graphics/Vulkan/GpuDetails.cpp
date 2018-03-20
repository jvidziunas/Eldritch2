/*==================================================================*\
  GpuDetails.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuDetails.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "vulkan-1.lib" )
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	GpuDetails::GpuDetails( VkPhysicalDevice gpu ) {
		vkGetPhysicalDeviceProperties( gpu, &basic );
		vkGetPhysicalDeviceMemoryProperties( gpu, &memory );
	}

// ---------------------------------------------------

	uint32 GpuDetails::GetIndexOfLargestHeapSupportingFlags( VkMemoryPropertyFlags flags ) const {
		VkDeviceSize	heapSize( 0u );
		uint32_t		heapIndex( static_cast<uint32>(-1) );

		for (uint32_t index( 0u ); index < memory.memoryHeapCount; ++index) {
			const auto&	descriptor( memory.memoryHeaps[index] );

			if ((flags == (descriptor.flags & flags)) && (descriptor.size > heapSize) ) {
				heapIndex	= index;
				heapSize	= descriptor.size;
			}
		}

		return heapIndex;
	}

// ---------------------------------------------------

	bool GpuDetails::DescribesNamedGpu( const char* name ) const {
		return StringsEqualCaseInsensitive( basic.deviceName, name );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2