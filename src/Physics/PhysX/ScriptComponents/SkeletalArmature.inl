/*==================================================================*\
  SkeletalArmature.inl
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
namespace ScriptComponents {

	ETInlineHint const physx::PxArticulation& SkeletalArmature::GetArticulation() const {
		return *_physicsAnimationLayer._articulation;
	}

// ---------------------------------------------------

	ETInlineHint physx::PxArticulation& SkeletalArmature::GetArticulation() {
		return *_physicsAnimationLayer._articulation;
	}

// ---------------------------------------------------

	ETInlineHint const SkeletalArmature::PhysicsAnimationLayer& SkeletalArmature::GetPhysicsAnimationLayer() const {
		return _physicsAnimationLayer;
	}

// ---------------------------------------------------

	ETInlineHint SkeletalArmature::PhysicsAnimationLayer& SkeletalArmature::GetPhysicsAnimationLayer() {
		return _physicsAnimationLayer;
	}

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2