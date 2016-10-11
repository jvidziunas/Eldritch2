/*==================================================================*\
  Device.inl
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

	ETInlineHint Vulkan::HostAllocator& Device::GetAllocator() {
		return _allocator;
	}

// ---------------------------------------------------

	ETInlineHint Device::operator VkPhysicalDevice() {
		return _physicalDevice;
	}

// ---------------------------------------------------

	ETInlineHint Device::operator VkDevice() {
		return _device.Get();
	}

// ---------------------------------------------------

	ETInlineHint Device::operator VkPipelineCache() {
		return _pipelineCache.Get();
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2