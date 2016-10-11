/*==================================================================*\
  MarshalingHelpers.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	RigidTransform;
	class	Quaternion;
	class	Float4;
}

namespace physx {
	struct	PxExtendedVec3;
	class	PxTransform;
	class	PxQuat;
	class	PxVec3;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	ETPureFunctionHint physx::PxExtendedVec3 ETSimdCall	PxExtendedVec3FromFloat4( Eldritch2::Float4 value );

	ETPureFunctionHint physx::PxVec3 ETSimdCall			PxVec3FromFloat4( Eldritch2::Float4 value );

	ETPureFunctionHint Eldritch2::Float4 ETSimdCall		Float4FromExtendedVec3( physx::PxExtendedVec3 value );

	ETPureFunctionHint Eldritch2::Float4 ETSimdCall		Float4FromPxVec3( physx::PxVec3 value );

// ---------------------------------------------------

	ETPureFunctionHint Eldritch2::Quaternion ETSimdCall	QuaternionFromPxQuat( physx::PxQuat value );

	ETPureFunctionHint physx::PxQuat ETSimdCall			PxQuatFromQuaternion( Eldritch2::Quaternion value );

// ---------------------------------------------------

	ETPureFunctionHint Eldritch2::RigidTransform	ETSimdCall	RigidTransformFromPxTransform( physx::PxTransform value );

	ETPureFunctionHint physx::PxTransform ETSimdCall			PxTransformFromRigidTransform( Eldritch2::RigidTransform value );

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/TypeMarshalers.inl>
//------------------------------------------------------------------//