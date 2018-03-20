/*==================================================================*\
  OutputWindow.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/OutputWindow.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	VkResult CreateSurface( Vulkan& vulkan, Window& window, VkSurfaceKHR* surface ) {
		const VkWin32SurfaceCreateInfoKHR	createInfo{
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			nullptr,	// No (additional) extension data.
			0u,			// No flags.
			window.GetHinstance(),
			window.GetHwnd()
		};

		return vkCreateWin32SurfaceKHR( vulkan, &createInfo, nullptr, surface );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2