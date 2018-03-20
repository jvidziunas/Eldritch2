/*==================================================================*\
  GpuDetails.inl
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
namespace Graphics {
namespace Vulkan {

	ETInlineHint bool GpuDetails::IsDiscrete() const {
		return basic.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
	}

// ---------------------------------------------------

	ETInlineHint bool GpuDetails::IsIntegrated() const {
		return basic.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2