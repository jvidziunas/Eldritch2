/*==================================================================*\
  DisplayLocator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/Display.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class DisplayLocator {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref DisplayLocator instance.
		DisplayLocator(DisplayList& displays, Mutex& mutex);
		//!	Disable copy construction.
		DisplayLocator(const DisplayLocator&) = delete;

		~DisplayLocator() = default;

		// ---------------------------------------------------

	public:
		UniquePointer<Viewport, ViewportDisposer> TryAcquireViewport(const GraphicsPipeline& pipeline);

		// - DATA MEMBERS ------------------------------------

	private:
		DisplayList* _displays;
		Mutex*       _displayMutex;
	};

}}} // namespace Eldritch2::Graphics::Vulkan
