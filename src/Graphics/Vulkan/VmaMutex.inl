/*==================================================================*\
  VmaMutex.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint ETForceInlineHint void VmaMutex::Lock() ETNoexceptHint {
		while (!TryEnter()) {
			// Busy wait.
		}
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void VmaMutex::Unlock() ETNoexceptHint {
		Leave();
	}

}}} // namespace Eldritch2::Graphics::Vulkan
