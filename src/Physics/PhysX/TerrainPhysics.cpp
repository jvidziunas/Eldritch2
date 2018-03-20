/*==================================================================*\
  TerrainPhysics.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/TerrainAsset.hpp>
#include <Physics/PhysX/TerrainPhysics.hpp>
//------------------------------------------------------------------//
#include <PxRigidStatic.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::Eldritch2::Assets;
	using namespace ::physx;

	TerrainPhysics::TerrainPhysics(
		PhysxPointer<PxRigidStatic> actor,
		const TerrainAsset& asset
	) : _actor( eastl::move( actor ) ),
		_asset( &asset ) {
		_actor->userData = this;
	}

// ---------------------------------------------------

	void TerrainPhysics::Enable() {
		_actor->attachShape( _asset->GetShape() );
	}

// ---------------------------------------------------

	void TerrainPhysics::Disable() {
		_actor->detachShape( _asset->GetShape() );
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2