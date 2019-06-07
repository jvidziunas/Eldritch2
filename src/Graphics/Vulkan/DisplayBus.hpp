/*==================================================================*\
  DisplayBus.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/Display.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class DisplayBus {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ResizeDelegate = Function<VkResult(Gpu&)>;
		using PresentList    = SoaList<VkResult /*presentResult*/, VkSemaphore /*ready*/, VkSwapchainKHR /*swapchain*/, uint32 /*index*/, ResizeDelegate /*onResize*/>;
		using DisplayList    = ArrayList<Display>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Disable copy construction.
		DisplayBus(const DisplayBus&) = delete;
		//! Constructs this @ref DisplayBus instance.
		DisplayBus() ETNoexceptHint;

		~DisplayBus() = default;

		// ---------------------------------------------------

	public:
		void ScheduleFlip(VkSemaphore ready, VkSwapchainKHR swapchain, uint32 index, ResizeDelegate onResize);

		VkResult FlipSwapchainImages(Gpu& gpu);

		// ---------------------------------------------------

	public:
		ETConstexpr Mutex& GetDisplaysMutex() ETNoexceptHint;

		DisplayList::SliceType GetDisplays() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu);

		void FreeResources(Gpu& gpu) ETNoexceptHint;

		// ---------------------------------------------------

	private:
		//! Disable copy assignment.
		DisplayBus& operator=(const DisplayBus&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		HashMap<VkFormat, GraphicsPipeline> _compositorByFormat;
		ETCacheLineAligned mutable Mutex    _displaysMutex;
		DisplayList                         _displays;
		ETCacheLineAligned mutable Mutex    _presentableImagesMutex;
		PresentList                         _presentableImages;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/DisplayBus.inl>
//------------------------------------------------------------------//
