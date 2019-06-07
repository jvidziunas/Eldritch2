/*==================================================================*\
  Display.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/Vulkan.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_win32.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	VkResult CreateSurface(Gpu& gpu, const Window& window, const VkAllocationCallbacks* allocator, VkSurfaceKHR* outSurface) {
		const VkWin32SurfaceCreateInfoKHR surfaceInfo {
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			/*pNext =*/nullptr, // No (additional) extension data.
			/*flags =*/0u,      // No flags.
			window.GetHinstance(),
			window.GetHwnd()
		};

		return vkCreateWin32SurfaceKHR(gpu, ETAddressOf(surfaceInfo), allocator, outSurface);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
