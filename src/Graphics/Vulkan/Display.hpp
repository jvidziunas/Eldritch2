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
		VkRect2D GetOwnedRegion() const;

		void SetOwnedRegion(VkRect2D region);

		// - DATA MEMBERS ------------------------------------

	private:
		VkRect2D _ownedRegion;

		// ---------------------------------------------------

		friend void Swap(DisplaySource&, DisplaySource&);
	};

	// ---

	class Display {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint32_t { MaxQueueDepth = 3u };

		// ---

		struct CompositeInfo {
			VkCommandPool   blitPool;
			VkCommandBuffer blitCommands;
			VkSemaphore     imageReady;
		};

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

		VkResult PresentImage(Gpu& gpu);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Vulkan& vulkan, Gpu& gpu);

		void FreeResources(Vulkan& vulkan, Gpu& gpu);

		// ---------------------------------------------------

	private:
		VkResult ResizeSwapchain(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Display& operator=(const Display&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Window             _window;
		VkSurfaceKHR       _surface;
		VkSurfaceFormatKHR _format;
		VkPresentModeKHR   _presentMode;
		VkSwapchainKHR     _swapchain;
		uint32_t           _swapchainIndex;
		uint32_t           _presentCount;
		VkImageView        _imageViews[MaxQueueDepth];
		VkFramebuffer      _framebuffers[MaxQueueDepth];
		CompositeInfo      _compositeInfos[MaxQueueDepth];
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
