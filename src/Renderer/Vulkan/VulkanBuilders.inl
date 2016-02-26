/*==================================================================*\
  VulkanBuilders.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	template <typename DeviceFilter>
	ETInlineHint PhysicalDeviceEnumerator& PhysicalDeviceEnumerator::FilterDevices( DeviceFilter filter ) {
		for( auto current( _enumeratedDevices.Begin() ); current != _enumeratedDevices.End(); ) {
			if( filter( const_cast<const Vulkan::PhysicalDeviceProperties&>(current->properties) ) ) {
				++current;
			} else {
				current = _enumeratedDevices.Erase( current );
			}
		}

		return *this;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

