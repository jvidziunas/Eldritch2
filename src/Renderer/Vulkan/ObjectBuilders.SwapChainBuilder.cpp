/*==================================================================*\
  ObjectBuilders.SwapChainBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ObjectBuilders.hpp>
#include <Renderer/Vulkan/Device.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "vulkan-1.lib" )
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

#if defined( VK_KHR_surface ) && defined( VK_KHR_swapchain )
	SwapChainBuilder::SwapChainBuilder() {
		_createInfo.sType				= VkStructureType::VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		_createInfo.pNext				= nullptr;
		_createInfo.flags				= 0u;
		_createInfo.oldSwapchain		= VK_NULL_HANDLE;
		_createInfo.imageSharingMode	= VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
		_createInfo.preTransform		= VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		_createInfo.compositeAlpha		= VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
		_createInfo.clipped				= VK_TRUE;
	}

// ---------------------------------------------------

	void SwapChainBuilder::EnableExtensions( LogicalDeviceBuilder& builder ) {
		builder.EnableExtension( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
	}

// ---------------------------------------------------

	void SwapChainBuilder::EnableExtensions( InstanceBuilder& builder ) {
		builder.EnableExtension( VK_KHR_SURFACE_EXTENSION_NAME );
	}

// ---------------------------------------------------

	SwapChainBuilder& SwapChainBuilder::SetDevicePreferredFormatAndColorSpace( VkPhysicalDevice device, VkSurfaceKHR surface ) {
		VkSurfaceFormatKHR	queriedFormats[4u];
		uint32_t			formatCount( _countof(queriedFormats) );
		const auto			result( vkGetPhysicalDeviceSurfaceFormatsKHR( device, surface, &formatCount, queriedFormats ) );

	//	Fall back to BGRA8888 if enumeration failed or the device indicates it doesn't care.
		if( result < VkResult::VK_SUCCESS || (1u == formatCount && (queriedFormats[0].format == VkFormat::VK_FORMAT_UNDEFINED)) ) {
			return SetFormatAndColorSpace( VkFormat::VK_FORMAT_B8G8R8A8_UNORM, VkColorSpaceKHR::VK_COLORSPACE_SRGB_NONLINEAR_KHR );
		}

		return SetFormatAndColorSpace( queriedFormats[0].format, queriedFormats[0].colorSpace );
	}

// ---------------------------------------------------

	bool SwapChainBuilder::IsUsableOnDevice( VkPhysicalDevice device, VkSurfaceKHR surface ) const {
		VkSurfaceCapabilitiesKHR	capabilities;

		const auto	result( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device, surface, &capabilities ) );
		if( VkResult::VK_SUCCESS != result ) {
			return false;
		}

		return ( _createInfo.minImageCount >= capabilities.minImageCount )
			&& ( _createInfo.minImageCount < capabilities.maxImageCount )
			&& ( _createInfo.imageExtent.width < capabilities.maxImageExtent.width )
			&& ( _createInfo.imageExtent.height < capabilities.maxImageExtent.height )
			&& ( _createInfo.compositeAlpha & capabilities.supportedCompositeAlpha )
			&& ( _createInfo.preTransform & capabilities.supportedTransforms );
	}

// ---------------------------------------------------

	VulkanResult<VkSwapchainKHR> SwapChainBuilder::Create( VkDevice device, VkSurfaceKHR surface, const VkAllocationCallbacks* callbacks ) {
		VkSwapchainKHR	swapChain( VK_NULL_HANDLE );
		const auto		createResult( vkCreateSwapchainKHR( device, &_createInfo, callbacks, &swapChain ) );

		if( createResult < VkResult::VK_SUCCESS ) {
			return { createResult };
		}

		return UniquePointer<VkSwapchainKHR>( swapChain, { device, callbacks } );
	}
#endif	// defined( VK_KHR_surface ) && defined( VK_KHR_swapchain )

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2