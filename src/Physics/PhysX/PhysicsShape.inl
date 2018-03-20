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
#include <new>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	ETInlineHint PhysicsShape::PhysicsShape( const physx::PxCapsuleGeometry& capsule ) {
		new (&this->asCapsule) physx::PxCapsuleGeometry( capsule );
	}

// ---------------------------------------------------

	ETInlineHint PhysicsShape::PhysicsShape( const physx::PxSphereGeometry& sphere ) {
		new (&this->asSphere) physx::PxSphereGeometry( sphere );
	}

// ---------------------------------------------------

	ETInlineHint PhysicsShape::PhysicsShape( const physx::PxBoxGeometry& box ) {
		new (&this->asBox) physx::PxBoxGeometry( box );
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2