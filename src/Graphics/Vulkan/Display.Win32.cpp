/*==================================================================*\
  Display.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/Vulkan.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_win32.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	VkResult CreateSurface(Vulkan& vulkan, Window& window, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) {
		const VkWin32SurfaceCreateInfoKHR surfaceInfo{
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			nullptr, // No (additional) extension data.
			0u,      // No flags.
			window.GetHinstance(),
			window.GetHwnd()
		};

		return vkCreateWin32SurfaceKHR(vulkan, &surfaceInfo, allocator, surface);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
