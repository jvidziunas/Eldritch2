/*==================================================================*\
  PresentCoordinator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/OutputWindow.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class PresentCoordinator {
	// - TYPE PUBLISHING ---------------------------------

	public:
		template <typename Value, class Allocator = MallocAllocator>
		using NameMap = CachingHashMap<String<>, Value, Hash<String<>>, EqualTo<String<>>, Allocator>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		PresentCoordinator( const PresentCoordinator& ) = delete;
	//!	Constructs this @ref PresentCoordinator instance.
		PresentCoordinator();

		~PresentCoordinator() = default;

	// ---------------------------------------------------

	public:
		VkResult	Present( Gpu& gpu );

	// ---------------------------------------------------

	public:
		CountedPointer<OutputWindow>	FindWindowByName( const Utf8Char* name );

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Vulkan& vulkan );

		void		FreeResources( Vulkan& vulkan );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		PresentCoordinator&	operator=( const PresentCoordinator& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan*						_vulkan;

		mutable Mutex				_outputsMutex;
		NameMap<OutputWindow>		_outputsByName;

		ArrayList<VkSwapchainKHR>	_presentableSwapchains;
		ArrayList<uint32_t>			_presentableImageIndices;
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2