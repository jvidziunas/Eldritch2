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

	ETConstexpr ETInlineHint ETForceInlineHint DisplayList& DisplayBus::GetDisplays() ETNoexceptHint {
		return _displays;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint Mutex& DisplayBus::GetDisplaysMutex() ETNoexceptHint {
		return _displayMutex;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
