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
#include <Graphics/Vulkan/GpuResources.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class GpuHeap;
	class Vulkan;
	class Gpu;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class ETPureAbstractHint DisplaySource {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		DisplaySource(const DisplaySource&) = delete;
		//!	Construct this @ref DisplaySource instance.
		DisplaySource(DisplaySource&&);
		//!	Construct this @ref DisplaySource instance.
		DisplaySource() = default;

		~DisplaySource() = default;

		// ---------------------------------------------------

	public:
		const Framebuffer& GetFramebuffer() const;

		VkExtent2D GetValidExtent() const;

		void SetValidExtent(VkExtent2D extent);

		// ---------------------------------------------------

	public:
		virtual VkExtent2D GetFramebufferExtent(VkExtent2D extent) const abstract;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkExtent2D extent);

		void FreeResources(Gpu& gpu);

		// - DATA MEMBERS ------------------------------------

	private:
		Framebuffer _framebuffer;
		VkExtent2D  _validExtent;

		// ---------------------------------------------------

		friend void Swap(DisplaySource&, DisplaySource&);
	};

	// ---

	class Display {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint32_t { MaxQueueDepth = 3u };

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
		bool ShouldDestroy() const;

		// ---------------------------------------------------

	public:
		bool TryAcquire(DisplaySource& source);

		void Release(DisplaySource& source);

		// ---------------------------------------------------

	public:
		VkResult AcquireImage(Vulkan& vulkan, Gpu& gpu);

		VkResult SubmitImage(Gpu& gpu);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Vulkan& vulkan, Gpu& gpu);

		void FreeResources(Vulkan& vulkan, Gpu& gpu);

		// ---------------------------------------------------

	private:
		VkResult ResizeSwapchain(Gpu& gpu);

		//!	Disable copy assignment.
		Display& operator=(const Display&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Window             _window;
		VkSurfaceKHR       _surface;
		VkSurfaceFormatKHR _format;
		VkPresentModeKHR   _presentMode;
		VkSwapchainKHR     _swapchain;
		uint32_t           _currentImage;
		VkCommandPool      _blitPool;
		VkCommandBuffer    _blitCommands[MaxQueueDepth];
		VkImage            _images[MaxQueueDepth];
		VkImageView        _views[MaxQueueDepth];
		VkFramebuffer      _framebuffers[MaxQueueDepth];
		VkSemaphore        _imageReady[MaxQueueDepth];
		DisplaySource*     _sources[4];

		// ---------------------------------------------------

		friend void Swap(Display&, Display&);
	};

	// ---

	template <class Allocator = MallocAllocator>
	using DisplayMap = CachingHashMap<String<>, Display, Hash<String<>>, EqualTo<String<>>, Allocator>;

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/Display.inl>
//------------------------------------------------------------------//
