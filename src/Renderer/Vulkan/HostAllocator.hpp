/*==================================================================*\
  VulkanAllocator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class HostAllocator : public Eldritch2::ChildAllocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref HostAllocator instance.
		HostAllocator( Eldritch2::Allocator& backingAllocator, const Eldritch2::Utf8Char* const name );
	//!	Disable copying.
		HostAllocator( const HostAllocator& ) = delete;
	//!	Constructs this @ref HostAllocator instance.
		HostAllocator( HostAllocator&& );

		~HostAllocator() = default;

	// ---------------------------------------------------

	public:
		const VkAllocationCallbacks*	AsAllocationCallbacks() const;

	// ---------------------------------------------------

	public:
		operator	const VkAllocationCallbacks*() const;

	// - DATA MEMBERS ------------------------------------

	private:
		VkAllocationCallbacks	_allocationCallbacks;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/HostAllocator.inl>
//------------------------------------------------------------------//