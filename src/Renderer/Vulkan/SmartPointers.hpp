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
#include <Utility/UniquePointer.hpp>
//------------------------------------------------------------------//
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace Detail {

	template <typename>
	class InstanceDeleter;

}	// namespace Detail
	
	template <typename VulkanHandle>
	using UniquePointer = Eldritch2::UniquePointer<typename eastl::remove_pointer<VulkanHandle>::type, Detail::InstanceDeleter<VulkanHandle>>;

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/SmartPointers.inl>
//------------------------------------------------------------------//