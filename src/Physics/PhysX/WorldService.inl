/*==================================================================*\
  WorldService.inl
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
namespace Physics {
namespace PhysX {
	
	ETInlineHint Eldritch2::Allocator& WorldService::GetArmatureAnimationLayerRootAllocator() const {
		return _animationLayerAllocator;
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Allocator& WorldService::GetArmatureAllocator() const {
		return _armatureAllocator;
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

