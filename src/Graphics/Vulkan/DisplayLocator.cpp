/*==================================================================*\
  DisplayLocator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/DisplayLocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	DisplayLocator::DisplayLocator(DisplayList& displays, Mutex& mutex) :
		_displays(ETAddressOf(displays)),
		_displayMutex(ETAddressOf(mutex)) {
	}

	// ---------------------------------------------------

	UniquePointer<Viewport, ViewportDisposer> DisplayLocator::TryAcquireViewport(const GraphicsPipelineBuilder& pipeline) {
		Lock _(*_displayMutex);
		for (Display& display : Range<Display*>(_displays->Begin<Display>(), _displays->End<Display>())) {
			for (Viewport& viewport : display.GetViewports()) {
				if (viewport.TryAcquire(pipeline)) {
					return ETAddressOf(viewport);
				}
			}
		}

		return nullptr;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
