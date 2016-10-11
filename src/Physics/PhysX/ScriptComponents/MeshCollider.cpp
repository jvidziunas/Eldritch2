/*==================================================================*\
  MeshCollider.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ScriptComponents/SkeletalArmature.hpp>
#include <Physics/PhysX/AssetViews/SkeletalColliderView.hpp>
#include <Physics/PhysX/ScriptComponents/MeshCollider.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Assets;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {

	MeshCollider::MeshCollider( SkeletalArmature& armature, const AssetViews::SkeletalColliderView& asset ) : _armature( armature ), _asset( &asset ) {
		Enable();
	}

// ---------------------------------------------------

	MeshCollider::MeshCollider( const MeshCollider& collider ) : _armature( collider._armature ), _asset( collider._asset ) {}

// ---------------------------------------------------

	void MeshCollider::Enable() {
		_asset->AttachTo( _armature.GetArticulation() );
	}

// ---------------------------------------------------

	void MeshCollider::Disable() {
		_asset->DetachFrom( _armature.GetArticulation() );
	}

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2