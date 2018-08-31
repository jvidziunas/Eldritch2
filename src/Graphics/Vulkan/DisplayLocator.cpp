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

	UniquePointer<Viewport, ViewportDisposer> DisplayLocator::TryAcquireViewport(const GraphicsPipeline& pipeline) {
		Lock _(*_displayMutex);
		for (auto display(_displays->Get<Display>()), end(display + _displays->GetSize()); display != end; ++display) {
			if (const auto viewport = display->TryAcquireViewport(pipeline)) {
				return viewport;
			}
		}

		return nullptr;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
