/*==================================================================*\
  ObjectBuilders.SurfaceBuilder.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Win32ApplicationHelpers.hpp>
#include <Renderer/Vulkan/ObjectBuilders.hpp>
#include <Platform/PlatformWindow.hpp>
//------------------------------------------------------------------//

#if defined( ET_PLATFORM_WINDOWS )
namespace Eldritch2 {
namespace Renderer{
namespace Vulkan {

	void SurfaceBuilder::EnableExtensions( LogicalDeviceBuilder& /*builder*/ ) {}

// ---------------------------------------------------

	void SurfaceBuilder::EnableExtensions( InstanceBuilder& builder ) {
		builder.EnableExtension( VK_KHR_SURFACE_EXTENSION_NAME );
		builder.EnableExtension( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );
	}

// ---------------------------------------------------

	VulkanResult<VkSurfaceKHR> SurfaceBuilder::Create( VkInstance instance, const Platform::PlatformWindow& window, const VkAllocationCallbacks* callbacks ) {
		VkWin32SurfaceCreateInfoKHR	createInfo;
		VkSurfaceKHR				surface( VK_NULL_HANDLE );

		createInfo.sType		= VkStructureType::VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.pNext		= nullptr;
		createInfo.flags		= 0u;
		createInfo.hinstance	= Utility::GetInstance();
		createInfo.hwnd			= static_cast<HWND>(window);

		const auto	createSurfaceResult( vkCreateWin32SurfaceKHR( instance, &createInfo, callbacks, &surface ) );

		if( createSurfaceResult < VkResult::VK_SUCCESS ) {
			return { createSurfaceResult };
		}

		return Vulkan::UniquePointer<VkSurfaceKHR>( surface, { instance, callbacks } );
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2
#endif	// defined( ET_PLATFORM_WINDOWS )