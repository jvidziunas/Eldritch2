/*==================================================================*\
  TerrainResourceView.inl
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

	ETInlineHint ::physx::PxShape& TerrainResourceView::GetShape() const {
		return *_shape.get();
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2