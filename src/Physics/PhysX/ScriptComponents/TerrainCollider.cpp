/*==================================================================*\
  TerrainCollider.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ScriptComponents/TerrainCollider.hpp>
#include <Physics/PhysX/AssetViews/TerrainView.hpp>
//------------------------------------------------------------------//
#include <PxRigidStatic.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Physics::PhysX::AssetViews;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Assets;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {

	TerrainCollider::TerrainCollider( UniquePointer<PxRigidStatic> actor, const AssetViews::TerrainView& asset ) : _actor( eastl::move( actor ) ), _asset( &asset ) {
		_actor->userData = this;
	}

// ---------------------------------------------------

	void TerrainCollider::Enable() {
		_asset->AttachToActor( *_actor );
	}

// ---------------------------------------------------

	void TerrainCollider::Disable() {
		_asset->DetachFromActor( *_actor );
	}

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2
