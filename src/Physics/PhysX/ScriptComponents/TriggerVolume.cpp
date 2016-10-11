/*==================================================================*\
  TriggerVolume.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ScriptComponents/TriggerVolume.hpp>
#include <Utility/Mpl/VectorTypes.hpp>
#include <Scripting/ApiRegistrar.hpp>
//------------------------------------------------------------------//
#include <PxRigidStatic.h>
#include <PxPhysics.h>
#include <PxScene.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {

	TriggerVolume::TriggerVolume( UniquePointer<PxRigidStatic> actor, UniquePointer<PxShape> shape ) : _actor( eastl::move( actor ) ), _collisionShape( eastl::move( shape ) ) {}

// ---------------------------------------------------

	void TriggerVolume::Enable() {
		_actor->attachShape( *_collisionShape );
	}

// ---------------------------------------------------

	void TriggerVolume::Disable() {
		_actor->detachShape( *_collisionShape );
	}

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2



