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
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Disable copy construction.
		DisplayBus(const DisplayBus&) = delete;
		//! Constructs this @ref DisplayCoordinator instance.
		DisplayBus();

		~DisplayBus() = default;

		// ---------------------------------------------------

	public:
		VkResult AcquireImages(Vulkan& vulkan, Gpu& gpu);

		// ---------------------------------------------------

	public:
		DisplayMap<>& GetDisplays();

		Mutex& GetDisplaysMutex();

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//! Disable copy assignment.
		DisplayBus& operator=(const DisplayBus&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		GraphicsPipeline _compositor;
		mutable Mutex    _displayMutex;
		DisplayMap<>     _displayByName;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/DisplayBus.inl>
//------------------------------------------------------------------//
