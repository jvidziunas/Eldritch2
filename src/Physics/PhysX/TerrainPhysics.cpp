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
#include <Physics/PhysX/TerrainPhysics.hpp>
//------------------------------------------------------------------//
#include <PxRigidStatic.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Physics {
		namespace PhysX {

			using namespace ::physx;

			TerrainPhysics::TerrainPhysics(PhysxPointer<PxRigidStatic> actor) : _actor(eastl::move(actor)) {
				_actor->userData = this;
			}

		}	// namespace PhysX
	}	// namespace Physics
}	// namespace Eldritch2