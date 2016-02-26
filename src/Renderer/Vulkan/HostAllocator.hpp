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

	class HostAllocator : public ::Eldritch2::ChildAllocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		HostAllocator( ::Eldritch2::Allocator& backingAllocator, const ::Eldritch2::UTF8Char* const name );

		~HostAllocator() = default;

	// ---------------------------------------------------

		ETInlineHint operator const ::VkAllocationCallbacks*() const;
		ETInlineHint operator ::VkAllocationCallbacks*();

	// - DATA MEMBERS ------------------------------------

	private:
		::VkAllocationCallbacks	_allocationCallbacks;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/HostAllocator.inl>
//------------------------------------------------------------------//