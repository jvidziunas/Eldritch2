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

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	ETInlineHint ETForceInlineHint physx::PxShape& TerrainAsset::GetShape() const ETNoexceptHint {
		return *_shape;
	}

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
