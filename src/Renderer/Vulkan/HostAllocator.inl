/*==================================================================*\
  HostAllocator.inl
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

	ETInlineHint HostAllocator::operator const ::VkAllocationCallbacks*() const {
		return &_allocationCallbacks;
	}

// ---------------------------------------------------

	ETInlineHint HostAllocator::operator ::VkAllocationCallbacks*() {
		return &_allocationCallbacks;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

