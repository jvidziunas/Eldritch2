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
#include <Graphics/Vulkan/GpuResourceApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class DisplayBus;
	class Display;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	enum class ViewportId : uintptr {
		Invalid = ~uintptr(0u)
	};

	// ---

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
		VkResult PresentImages() const;

		// ---------------------------------------------------

	public:
		ViewportId TryAcquireViewport(const PipelineBuilder& pipeline);

		void ReleaseViewport(ViewportId id) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, DisplayBus& bus);

		void FreeResources(Gpu& gpu) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		DisplayBus*                       _bus;
		ArrayMap<ViewportId, Framebuffer> _framebufferById;
	};

}}} // namespace Eldritch2::Graphics::Vulkan
