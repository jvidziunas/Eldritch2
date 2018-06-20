/*==================================================================*\
  DisplayCoordinator.inl
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

	ETInlineHint DisplayMap<>& DisplayCoordinator::GetDisplayCollection() {
		return _displayByName;
	}

	// ---------------------------------------------------

	ETInlineHint Mutex& DisplayCoordinator::GetDisplayMutex() {
		return _displayMutex;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
