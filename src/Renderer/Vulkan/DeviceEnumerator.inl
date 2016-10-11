/*==================================================================*\
  DeviceEnumerator.inl
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

	ETInlineHint DeviceEnumerator& DeviceEnumerator::FilterUnless( Filter&& filter ) {
		_filters.PushBack( eastl::move( filter ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint DeviceEnumerator& DeviceEnumerator::OrderBy( OrderingPredicate&& predicate ) {
		_orderingPredicate = eastl::move( predicate );

		return *this;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

