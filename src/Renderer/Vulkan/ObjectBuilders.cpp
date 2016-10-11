/*==================================================================*\
  ObjectBuilders.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ObjectBuilders.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "vulkan-1.lib" )
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	const ArrayRegion	AllArraySlices	= { 0u, VK_REMAINING_ARRAY_LAYERS };
	const CubeRegion	AllCubes		= { 0u, VK_REMAINING_ARRAY_LAYERS };
	const MipRegion		AllMips			= { 0u, VK_REMAINING_MIP_LEVELS };

// ---------------------------------------------------

	InstanceBuilder::InstanceBuilder( Allocator& allocator, uint32_t vulkanApiVersion ) : _enabledExtensions( { allocator, "Vulkan Builder Enabled Extension Collection Allocator" } ),
																						  _enabledLayers( { allocator, "Vulkan Builder Enabled Layer Collection Allocator" } ) {
		_applicationInfo.sType				= VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO;
		_applicationInfo.pNext				= nullptr;
		_applicationInfo.pApplicationName	= PROJECT_NAME;
		_applicationInfo.applicationVersion	= 1;
		_applicationInfo.pEngineName		= "Eldritch2";
		_applicationInfo.engineVersion		= 1;
		_applicationInfo.apiVersion			= vulkanApiVersion;

		_createInfo.sType					= VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		_createInfo.pNext					= nullptr;
		_createInfo.flags					= 0;
		_createInfo.pApplicationInfo		= &_applicationInfo;
	}

// ---------------------------------------------------

	VulkanResult<VkInstance> InstanceBuilder::Create( const VkAllocationCallbacks* callbacks ) {
		_createInfo.enabledLayerCount		= static_cast<uint32_t>(_enabledLayers.GetSize());
		_createInfo.ppEnabledLayerNames		= _enabledLayers.Begin();
		_createInfo.enabledExtensionCount	= static_cast<uint32_t>(_enabledExtensions.GetSize());
		_createInfo.ppEnabledExtensionNames = _enabledExtensions.Begin();

		VkInstance	vulkan( VK_NULL_HANDLE );
		const auto	createInstanceResult( vkCreateInstance( &_createInfo, callbacks, &vulkan ) );

		if( createInstanceResult < VkResult::VK_SUCCESS ) {
			return { createInstanceResult };
		}

		return { { vulkan, { callbacks } } };
	}

// ---------------------------------------------------

	LogicalDeviceBuilder::LogicalDeviceBuilder( Allocator& allocator ) : _enabledExtensions( { allocator, "Vulkan Device Builder Enabled Extension Collection Allocator" } ),
																		 _enabledLayers( { allocator, "Vulkan Device Builder Enabled Layer Collection Allocator" } ) {
		_createInfo.sType					= VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		_createInfo.pNext					= nullptr;
		_createInfo.flags					= 0;
		_createInfo.pEnabledFeatures		= nullptr;
		_createInfo.queueCreateInfoCount	= 0;
		_createInfo.pQueueCreateInfos		= nullptr;
	}

// ---------------------------------------------------

	VulkanResult<VkDevice> LogicalDeviceBuilder::Create( VkPhysicalDevice physicalDevice, const VkAllocationCallbacks* callbacks ) {
		_createInfo.enabledLayerCount		= static_cast<uint32_t>(_enabledLayers.GetSize());
		_createInfo.ppEnabledLayerNames		= _enabledLayers.Begin();
		_createInfo.enabledExtensionCount	= static_cast<uint32_t>(_enabledExtensions.GetSize());
		_createInfo.ppEnabledExtensionNames = _enabledExtensions.Begin();

		VkDevice	device( VK_NULL_HANDLE );
		const auto	createDeviceResult( vkCreateDevice( physicalDevice, &_createInfo, callbacks, &device ) );

		if( createDeviceResult < VkResult::VK_SUCCESS ) {
			return { createDeviceResult };
		}

		return { { device, { callbacks } } };
	}

// ---------------------------------------------------

	ImageBuilder::ImageBuilder() {
		_createInfo.sType	= VkStructureType::VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		_createInfo.pNext	= nullptr;
		_createInfo.flags	= 0u;
	}

// ---------------------------------------------------

	ImageBuilder& ImageBuilder::SetExtent( uint32_t extent ) {
		_createInfo.imageType		= VkImageType::VK_IMAGE_TYPE_1D;
		_createInfo.extent.width	= extent;
		_createInfo.extent.height	= 1u;
		_createInfo.extent.depth	= 1u;
		_createInfo.arrayLayers		= 1u;

		return *this;
	}

// ---------------------------------------------------

	ImageBuilder& ImageBuilder::SetExtent( uint32_t extent, uint32_t arraySliceCount ) {
		_createInfo.imageType		= VkImageType::VK_IMAGE_TYPE_1D;
		_createInfo.extent.width	= extent;
		_createInfo.extent.height	= 1u;
		_createInfo.extent.depth	= 1u;
		_createInfo.arrayLayers		= arraySliceCount;

		return *this;
	}

// ---------------------------------------------------

	ImageBuilder& ImageBuilder::SetExtent( VkExtent2D extent ) {
		_createInfo.imageType		= VkImageType::VK_IMAGE_TYPE_2D;
		_createInfo.extent.width	= extent.width;
		_createInfo.extent.height	= extent.height;
		_createInfo.extent.depth	= 1u;
		_createInfo.arrayLayers		= 1u;

		return *this;
	}

// ---------------------------------------------------

	ImageBuilder& ImageBuilder::SetExtent( VkExtent2D extent, uint32_t arraySliceCount ) {
		_createInfo.imageType		= VkImageType::VK_IMAGE_TYPE_2D;
		_createInfo.extent.width	= extent.width;
		_createInfo.extent.height	= extent.height;
		_createInfo.extent.depth	= 1u;
		_createInfo.arrayLayers		= arraySliceCount;

		return *this;
	}

// ---------------------------------------------------

	ImageBuilder& ImageBuilder::SetExtent( VkExtent3D extent ) {
		_createInfo.imageType		= VkImageType::VK_IMAGE_TYPE_3D;
		_createInfo.extent			= extent;
		_createInfo.arrayLayers		= 1u;

		return *this;
	}

// ---------------------------------------------------

	ImageBuilder& ImageBuilder::SetCubeExtent( VkExtent2D extent ) {
		_createInfo.imageType		= VkImageType::VK_IMAGE_TYPE_2D;
		_createInfo.extent.width	= extent.width;
		_createInfo.extent.height	= extent.height;
		_createInfo.extent.depth	= 1u;
		_createInfo.arrayLayers		= 6u;

		return *this;
	}

// ---------------------------------------------------

	ImageBuilder& ImageBuilder::SetCubeExtent( VkExtent2D extent, uint32_t cubeCount ) {
		_createInfo.imageType		= VkImageType::VK_IMAGE_TYPE_2D;
		_createInfo.extent.width	= extent.width;
		_createInfo.extent.height	= extent.height;
		_createInfo.extent.depth	= 1u;
		_createInfo.arrayLayers		= cubeCount * 6u;

		return *this;
	}

// ---------------------------------------------------

	VulkanResult<VkImage> ImageBuilder::Create( VkDevice device, const VkAllocationCallbacks* callbacks ) {
		VkImage	image( VK_NULL_HANDLE );
		auto	createImageResult( vkCreateImage( device, &_createInfo, callbacks, &image ) );

		if( createImageResult < VkResult::VK_SUCCESS ) {
			return { createImageResult };
		}

		return { { image, { device, callbacks } } };
	}

// ---------------------------------------------------

	ShaderModuleBuilder::ShaderModuleBuilder() {
		_createInfo.sType	= VkStructureType::VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		_createInfo.pNext	= nullptr;
		_createInfo.flags	= 0u;
	}

// ---------------------------------------------------

	VulkanResult<VkShaderModule> ShaderModuleBuilder::Create( VkDevice device, const VkAllocationCallbacks* callbacks ) {
		VkShaderModule	module( VK_NULL_HANDLE );
		const auto		createModuleResult( vkCreateShaderModule( device, &_createInfo, callbacks, &module ) );

		if( createModuleResult < VkResult::VK_SUCCESS ) {
			return { createModuleResult };
		}

		return { { module, { device, callbacks } } };
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

