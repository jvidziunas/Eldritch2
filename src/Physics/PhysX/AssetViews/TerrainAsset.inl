/*==================================================================*\
  TerrainAsset.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace AssetViews {

	ETInlineHint physx::PxShape& TerrainAsset::GetShape() const {
		return *_shape;
	}

}	// namespace AssetViews
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2