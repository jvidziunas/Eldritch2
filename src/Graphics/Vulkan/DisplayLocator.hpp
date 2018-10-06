/*==================================================================*\
  DisplayLocator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
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
		//!	Disable copy construction.
		DisplayLocator(const DisplayLocator&) = delete;
		//!	Constructs this @ref DisplayLocator instance.
		DisplayLocator() ETNoexceptHint;

		~DisplayLocator() = default;

		// ---------------------------------------------------

	public:
		UniquePointer<Viewport, ViewportDisposer> TryAcquireViewport(const GraphicsPipelineBuilder& pipeline);

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(DisplayList& displays, Mutex& mutex);

		void FreeResources();

		// - DATA MEMBERS ------------------------------------

	private:
		DisplayList* _displays;
		Mutex*       _displayMutex;
	};

}}} // namespace Eldritch2::Graphics::Vulkan
