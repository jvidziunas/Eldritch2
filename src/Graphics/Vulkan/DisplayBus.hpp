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
#include <Graphics/Vulkan/CommandList.hpp>
#include <Graphics/Vulkan/Display.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class DisplayBus {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Disable copy construction.
		DisplayBus(const DisplayBus&) = delete;
		//! Constructs this @ref DisplayBus instance.
		DisplayBus();

		~DisplayBus() = default;

		// ---------------------------------------------------

	public:
		VkResult PresentSwapchainImages(Gpu& gpu);

		VkResult AcquireSwapchainImages(Gpu& gpu);

		// ---------------------------------------------------

	public:
		ETConstexpr DisplayList& GetDisplays() ETNoexceptHint;

		ETConstexpr Mutex& GetDisplaysMutex() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

	private:
		//! Disable copy assignment.
		DisplayBus& operator=(const DisplayBus&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		HashMap<VkFormat, GraphicsPipeline> _compositorByFormat;
		mutable Mutex                       _displayMutex;
		DisplayList                         _displays;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/DisplayBus.inl>
//------------------------------------------------------------------//
