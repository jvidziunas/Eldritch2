/*==================================================================*\
  PhysicsScene.inl
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

	ETInlineHint const physx::PxMaterial& PhysicsScene::GetTriggerMaterial() const {
		return *_triggerMaterial;
	}

// ---------------------------------------------------

	ETInlineHint bool PhysicsScene::IsSimulationComplete() const {
		return _scene->checkResults();
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2