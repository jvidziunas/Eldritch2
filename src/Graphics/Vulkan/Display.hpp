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
#include <Graphics/Vulkan/GpuResourceApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class CommandList;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class Display {
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
		ETConstexpr auto GetViewports() const ETNoexceptHint -> const Viewport (&)[4];
		ETConstexpr auto GetViewports() ETNoexceptHint -> Viewport (&)[4];

		// ---------------------------------------------------

	public:
		VkResult BindSwapchain(Gpu& gpu, VkSwapchainKHR& outSwapchain, VkSwapchainKHR old) ETNoexceptHint;

		void FreeSwapchain(Gpu& gpu, VkSwapchainKHR swapchain) ETNoexceptHint;

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
		Viewport           _viewports[4];

		// ---------------------------------------------------

		friend void Swap(Display&, Display&);
	};

	// ---

	using ImageList   = SoArrayList<VkImageView /*view*/, Framebuffer /*backbuffer*/, CommandList /*commands*/, VkSemaphore /*ready*/>;
	using DisplayList = SoArrayList<Display /*display*/, VkResult /*presentResult*/, VkSwapchainKHR /*swapchain*/, VkSemaphore /*ready*/, uint32 /*index*/, ImageList /*images*/>;

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/Display.inl>
//------------------------------------------------------------------//
