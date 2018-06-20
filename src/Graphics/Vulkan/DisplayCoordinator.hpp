/*==================================================================*\
  DisplayCoordinator.hpp
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

	class DisplayCoordinator {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Disable copy construction.
		DisplayCoordinator(const DisplayCoordinator&) = delete;
		//! Constructs this @ref DisplayCoordinator instance.
		DisplayCoordinator();

		~DisplayCoordinator() = default;

		// ---------------------------------------------------

	public:
		VkResult AcquireImages(Vulkan& vulkan, Gpu& gpu);

		// ---------------------------------------------------

	public:
		DisplayMap<>& GetDisplayCollection();

		Mutex& GetDisplayMutex();

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//! Disable copy assignment.
		DisplayCoordinator& operator=(const DisplayCoordinator&) = delete;

		// ---------------------------------------------------

	private:
		VkResult SubmitImages(Gpu& gpu);

		// - DATA MEMBERS ------------------------------------

	private:
		GraphicsPipeline _compositePipeline;
		mutable Mutex    _displayMutex;
		DisplayMap<>     _displayByName;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/DisplayCoordinator.inl>
//------------------------------------------------------------------//
