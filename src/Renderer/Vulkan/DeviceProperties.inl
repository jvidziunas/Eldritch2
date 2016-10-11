/*==================================================================*\
  VulkanProperties.inl
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

	ETInlineHint bool PhysicalDeviceProperties::IsDiscreteGpu() const {
		return VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU == properties.deviceType;
	}

// ---------------------------------------------------

	ETInlineHint bool PhysicalDeviceProperties::IsIntegratedGpu() const {
		return VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU == properties.deviceType;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

