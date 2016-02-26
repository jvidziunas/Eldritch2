/*==================================================================*\
  SmartPointers.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <memory>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace Detail {

	template <typename VulkanHandle>
	class InstanceDeleter;

}	// namespace Detail
	
template <typename VulkanHandle>
using UniquePointer = ::std::unique_ptr<typename ::std::remove_pointer<VulkanHandle>::type, Detail::InstanceDeleter<VulkanHandle>>;

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/SmartPointers.inl>
//------------------------------------------------------------------//