/*==================================================================*\
  Vulkan.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETConstexpr ETInlineHint ETForceInlineHint Device& Vulkan::GetDevice() ETNoexceptHint {
		return _device;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint Vulkan::operator VkInstance() ETNoexceptHint {
		return _vulkan;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
