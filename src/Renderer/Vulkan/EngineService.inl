/*==================================================================*\
  EngineService.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	ETInlineHint Vulkan::Device& EngineService::GetDeviceForCurrentFrame() const {
		return *_device;
	}

// ---------------------------------------------------

	ETInlineHint Vulkan::Device& EngineService::GetDeviceForNextFrame() const {
		return *_device;
	}

// ---------------------------------------------------

	ETInlineHint Vulkan::HostAllocator& EngineService::GetVulkanAllocator() const {
		return _allocator;
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Allocator& EngineService::GetAllocator() const {
		return _allocator;
	}

// ---------------------------------------------------

	ETInlineHint VkInstance EngineService::GetVulkan() const {
		return _vulkan.Get();
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2