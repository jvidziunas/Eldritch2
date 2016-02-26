/*==================================================================*\
  SmartPointers.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SmartPointers.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace Detail {

	InstanceDeleterBase::InstanceDeleterBase( const ::VkAllocationCallbacks* const callbacks ) : _allocationCallbacks( callbacks ) {}

}	// namespace Detail
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2



