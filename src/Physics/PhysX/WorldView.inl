/*==================================================================*\
  WorldView.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	ETInlineHint const ::physx::PxControllerManager& WorldView::GetControllerManager() const {
		return *_controllerManager;
	}

// ---------------------------------------------------

	ETInlineHint ::physx::PxControllerManager& WorldView::GetControllerManager() {
		return *_controllerManager;
	}

// ---------------------------------------------------

	ETInlineHint const ::physx::PxScene& WorldView::GetScene() const {
		return *_scene;
	}

// ---------------------------------------------------

	ETInlineHint::physx::PxScene& WorldView::GetScene() {
		return *_scene;
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

