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

	ETInlineHint DisplayMap<>& DisplayBus::GetDisplays() {
		return _displayByName;
	}

	// ---------------------------------------------------

	ETInlineHint Mutex& DisplayBus::GetDisplaysMutex() {
		return _displayMutex;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
