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

	ETInlineHint ETForceInlineHint Viewport* Display::TryAcquireViewport(const GraphicsPipeline& generator) ETNoexceptHint {
		for (Viewport& viewport : _viewports) {
			if (viewport.TryAcquire(generator)) {
				return ETAddressOf(viewport);
			}
		}

		return nullptr;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const Display::ViewportList& Display::GetViewports() const ETNoexceptHint {
		return _viewports;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
