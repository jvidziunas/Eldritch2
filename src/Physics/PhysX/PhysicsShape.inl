/*==================================================================*\
  PhysicsShape.inl
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

namespace Eldritch2 { namespace Physics { namespace PhysX {

	ETConstexpr ETForceInlineHint PhysicsShape::PhysicsShape(const physx::PxCapsuleGeometry& shape) ETNoexceptHint : _as{ shape } {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint PhysicsShape::PhysicsShape(const physx::PxSphereGeometry& shape) ETNoexceptHint : _as{ shape } {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint PhysicsShape::PhysicsShape(const physx::PxBoxGeometry& shape) ETNoexceptHint : _as{ shape } {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint PhysicsShape::operator const PxGeometry&() const ETNoexceptHint {
		return _as.geometry;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint PhysicsShape& PhysicsShape::operator=(const physx::PxCapsuleGeometry& shape) ETNoexceptHint {
		_as.capsule = shape;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint PhysicsShape& PhysicsShape::operator=(const physx::PxSphereGeometry& shape) ETNoexceptHint {
		_as.sphere = shape;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint PhysicsShape& PhysicsShape::operator=(const physx::PxBoxGeometry& shape) ETNoexceptHint {
		_as.box = shape;
	}

}}} // namespace Eldritch2::Physics::PhysX
