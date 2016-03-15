/*==================================================================*\
  VulkanBuilders.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/VulkanBuilders.hpp>
#include <Build.hpp>
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

	VulkanBuilder::VulkanBuilder( Allocator& allocator, uint32_t vulkanApiVersion ) : _enabledExtensions( { allocator, UTF8L("Vulkan Builder Enabled Extension Collection Allocator") } ),
																					  _enabledLayers( { allocator, UTF8L("Vulkan Builder Enabled Layer Collection Allocator") } ) {
		_applicationInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		_applicationInfo.pNext				= nullptr;
		_applicationInfo.pApplicationName	= PROJECT_NAME;
		_applicationInfo.applicationVersion	= 1;
		_applicationInfo.pEngineName		= "Eldritch2";
		_applicationInfo.engineVersion		= 1;
		_applicationInfo.apiVersion			= vulkanApiVersion;

		_createInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		_createInfo.pNext					= nullptr;
		_createInfo.flags					= 0;
		_createInfo.pApplicationInfo		= &_applicationInfo;
	}

// ---------------------------------------------------

	VulkanResult<::VkInstance> VulkanBuilder::Create( const ::VkAllocationCallbacks* callbacks ) {
		_createInfo.enabledLayerCount		= static_cast<uint32_t>(_enabledLayers.Size());
		_createInfo.ppEnabledLayerNames		= _enabledLayers.Data();
		_createInfo.enabledExtensionCount	= static_cast<uint32_t>(_enabledExtensions.Size());
		_createInfo.ppEnabledExtensionNames = _enabledExtensions.Data();

		::VkInstance	vulkan( nullptr );
		const auto		createInstanceResult( ::vkCreateInstance( &_createInfo, callbacks, &vulkan ) );

		if( VK_SUCCESS == createInstanceResult ) {
			return { { vulkan, { callbacks } } };
		}

		return { createInstanceResult };
	}

// ---------------------------------------------------

	LogicalDeviceBuilder::LogicalDeviceBuilder( Allocator& allocator ) : _enabledExtensions( { allocator, UTF8L("Vulkan Device Builder Enabled Extension Collection Allocator") } ),
																		 _enabledLayers( { allocator, UTF8L("Vulkan Device Builder Enabled Layer Collection Allocator") } ) {
		_createInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		_createInfo.pNext					= nullptr;
		_createInfo.flags					= 0;
		_createInfo.pEnabledFeatures		= nullptr;
		_createInfo.queueCreateInfoCount	= 0;
		_createInfo.pQueueCreateInfos		= nullptr;
	}

// ---------------------------------------------------

	VulkanResult<::VkDevice> LogicalDeviceBuilder::Create( ::VkPhysicalDevice physicalDevice, const ::VkAllocationCallbacks* callbacks ) {
		_createInfo.enabledLayerCount		= static_cast<uint32_t>(_enabledLayers.Size());
		_createInfo.ppEnabledLayerNames		= _enabledLayers.Data();
		_createInfo.enabledExtensionCount	= static_cast<uint32_t>(_enabledExtensions.Size());
		_createInfo.ppEnabledExtensionNames = _enabledExtensions.Data();

		::VkDevice	device( nullptr );
		const auto	createDeviceResult( ::vkCreateDevice( physicalDevice, &_createInfo, callbacks, &device ) );

		if( VK_SUCCESS == createDeviceResult ) {
			return { { device, { callbacks } } };
		}

		return { createDeviceResult };
	}

// ---------------------------------------------------

	PhysicalDeviceEnumerator::EnumeratedDevice::EnumeratedDevice( ::VkPhysicalDevice device ) : device( device ), properties( device ) {}

// ---------------------------------------------------

	PhysicalDeviceEnumerator::PhysicalDeviceEnumerator( VkInstance instance, Allocator& allocator ) : _enumeratedDevices( { allocator, UTF8L("Vulkan Device Enumerator Temporary Allocator") } ) {
		uint32	physicalDeviceCount( 0u );

		// First, get the number of installed physical devices in the system.
		if( VK_SUCCESS > ::vkEnumeratePhysicalDevices( instance, &physicalDeviceCount, nullptr ) || 0u == physicalDeviceCount ) {
			return;
		}

		// Allocate an array large enough to hold all the physical device handles.
		const auto	devices( static_cast<VkPhysicalDevice*>( _alloca( physicalDeviceCount * sizeof(VkPhysicalDevice) ) ) );

		// Fill the array with handles.
		if( VK_SUCCESS > ::vkEnumeratePhysicalDevices( instance, &physicalDeviceCount, devices ) ) {
			return;
		}

		// Retrieve data for each attached physical device.
		for( VkPhysicalDevice device : Range<VkPhysicalDevice*>( devices, devices + physicalDeviceCount ) ) {
			_enumeratedDevices.EmplaceBack( device );
		}
	}

// ---------------------------------------------------

	::VkPhysicalDevice PhysicalDeviceEnumerator::GetTopDevice() const {
		if( _enumeratedDevices.Empty() ) {
			return nullptr;
		}

		return _enumeratedDevices.Front().device;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

