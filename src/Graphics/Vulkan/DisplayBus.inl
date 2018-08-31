/*==================================================================*\
  DisplayBus.inl
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

	ETInlineHint DisplayList& DisplayBus::GetDisplays() ETNoexceptHint {
		return _displays;
	}

	// ---------------------------------------------------

	ETInlineHint Mutex& DisplayBus::GetDisplaysMutex() ETNoexceptHint {
		return _displayMutex;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
