/*==================================================================*\
  PhysicsShape.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#include <geometry/PxCapsuleGeometry.h>
#include <geometry/PxSphereGeometry.h>
#include <geometry/PxBoxGeometry.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	union PhysicsShape {
		PhysicsShape( const physx::PxCapsuleGeometry& capsule );
		PhysicsShape( const physx::PxSphereGeometry& sphere );
		PhysicsShape( const physx::PxBoxGeometry& box );

		physx::PxGeometry			asGeometry;
		physx::PxCapsuleGeometry	asCapsule;
		physx::PxSphereGeometry		asSphere;
		physx::PxBoxGeometry		asBox;
	};

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/PhysicsShape.inl>
//------------------------------------------------------------------//