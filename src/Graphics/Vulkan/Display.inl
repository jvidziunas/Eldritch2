/*==================================================================*\
  Display.inl
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

	ETConstexpr ETInlineHint ETForceInlineHint auto Display::GetViewports() const ETNoexceptHint -> const Viewport (&)[4] {
		return _viewports;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint auto Display::GetViewports() ETNoexceptHint -> Viewport (&)[4] {
		return _viewports;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
