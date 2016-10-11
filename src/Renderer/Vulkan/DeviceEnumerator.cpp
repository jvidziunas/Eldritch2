/*==================================================================*\
  DeviceEnumerator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/DeviceEnumerator.hpp>
#include <Renderer/Vulkan/DeviceProperties.hpp>
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	DeviceEnumerator::DeviceEnumerator( VkInstance instance, Allocator& allocator ) : _filters( { allocator, "Vulkan Device Enumerator Filter Allocator" } ),
																					  _orderingPredicate( [] ( const PhysicalDeviceProperties&, const PhysicalDeviceProperties& ) { return false; } ),
																					  _instance( instance ) {}
	
// ---------------------------------------------------
	
	VkPhysicalDevice DeviceEnumerator::GetTopDevice() const {
		uint32	physicalDeviceCount( 0u );

	//	First, get the number of installed physical devices in the system.
		if( vkEnumeratePhysicalDevices( _instance, &physicalDeviceCount, nullptr ) < VkResult::VK_SUCCESS || 0u == physicalDeviceCount ) {
			return VK_NULL_HANDLE;
		}

	//	Allocate an array large enough to hold all the physical device handles.
		const auto	devices( static_cast<VkPhysicalDevice*>( _alloca( physicalDeviceCount * sizeof(VkPhysicalDevice) ) ) );

	//	Fill the array with handles.
		if( vkEnumeratePhysicalDevices( _instance, &physicalDeviceCount, devices ) < VkResult::VK_SUCCESS ) {
			return VK_NULL_HANDLE;
		}

		if( 1u == physicalDeviceCount ) {
			return devices[0];
		}

		PhysicalDeviceProperties	bestMatchDeviceProperties( devices[0] );
		VkPhysicalDevice			returnedDevice( devices[0] );

	//	Retrieve data for each attached physical device.
		for( auto device : Range<const VkPhysicalDevice*>( devices, devices + physicalDeviceCount ) ) {
			const PhysicalDeviceProperties	newDeviceProperties( device );
			bool							validCandidate( true );

			for( const auto& filter : _filters ) {
				if( !filter( newDeviceProperties ) ) {
					validCandidate = false;
					break;
				}
			}

			if( validCandidate && _orderingPredicate( bestMatchDeviceProperties, newDeviceProperties ) ) {
				returnedDevice				= device;
				bestMatchDeviceProperties	= newDeviceProperties;
			}
		}

		return returnedDevice;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

