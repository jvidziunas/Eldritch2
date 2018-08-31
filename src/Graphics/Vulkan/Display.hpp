/*==================================================================*\
  Display.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class CommandList;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class Display {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ViewportList = Viewport[4];

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Display(const Display&) = delete;
		//!	Constructs this @ref Display instance.
		Display(Display&&);
		//!	Constructs this @ref Display instance.
		Display();

		~Display();

		// ---------------------------------------------------

	public:
		const ViewportList& GetViewports() const ETNoexceptHint;

		Viewport* TryAcquireViewport(const GraphicsPipeline& generator) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult CreateSwapchain(Gpu& gpu, VkSwapchainKHR& outSwapchain) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, bool preferVerticalSync, uint32 formatCount, const VkFormat* preferredFormats);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Display& operator=(const Display&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Window             _window;
		VkSurfaceKHR       _surface;
		VkSurfaceFormatKHR _format;
		VkPresentModeKHR   _presentMode;
		ViewportList       _viewports;

		// ---------------------------------------------------

		friend void Swap(Display&, Display&);
	};

	// ---

	using ImageList   = SoArrayList<VkImageView /*view*/, Framebuffer /*backbuffer*/, CommandList /*commands*/>;
	using DisplayList = SoArrayList<Display /*display*/, VkResult /*presentResult*/, VkSwapchainKHR /*swapchain*/, VkSemaphore /*ready*/, uint32 /*index*/, ImageList /*images*/>;

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/Display.inl>
//------------------------------------------------------------------//
