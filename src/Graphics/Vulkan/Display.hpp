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

	class Display {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ImageList = SoaList<VkImageView /*view*/, VkSemaphore /*ready*/>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Display(const Display&) = delete;
		//!	Constructs this @ref Display instance.
		Display(Display&&) ETNoexceptHint;
		//!	Constructs this @ref Display instance.
		Display() ETNoexceptHint;

		~Display();

		// ---------------------------------------------------

	public:
		VkResult AcquireImage(Gpu& gpu, VkSwapchainKHR& outSwapchain, VkSemaphore& outWaitSemaphore, uint32& outIndex, uint32 deviceMask = 0u);

		// ---------------------------------------------------

	public:
		VkResult BindSwapchain(Gpu& gpu);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, bool preferVerticalSync, uint32 formatCount, const VkFormat preferredFormats[]);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Display& operator=(const Display&) = delete;

		// - DATA MEMBERS ------------------------------------

	public:
		VkRect2D displayRects[4];

		// - DATA MEMBERS ------------------------------------

	private:
		Window             _window;
		VkSurfaceKHR       _surface;
		VkSurfaceFormatKHR _format;
		VkPresentModeKHR   _presentMode;
		VkSwapchainKHR     _swapchain;
		ImageList          _images;

		// ---------------------------------------------------

		friend void Swap(Display&, Display&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan
