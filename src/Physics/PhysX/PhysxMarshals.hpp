/*==================================================================*\
  TypeMarshals.hpp
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

namespace physx {
	struct	PxExtendedVec3;
	class	PxTransform;
	class	PxQuat;
	class	PxVec3;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	ETPureFunctionHint physx::PxExtendedVec3 ETSimdCall	AsPxExtendedVec3( Vector value );

	ETPureFunctionHint physx::PxVec3 ETSimdCall			AsPxVec3( Vector value );

	ETPureFunctionHint Vector ETSimdCall				AsVector( physx::PxExtendedVec3 value );

	ETPureFunctionHint Vector ETSimdCall				AsVector( physx::PxVec3 value );

// ---------------------------------------------------

	ETPureFunctionHint physx::PxQuat ETSimdCall	AsPxQuat( Quaternion value );

	ETPureFunctionHint Quaternion ETSimdCall	AsQuaternion( physx::PxQuat value );

// ---------------------------------------------------

	ETPureFunctionHint physx::PxTransform ETSimdCall	AsPxTransform( Transformation value );

	ETPureFunctionHint Transformation ETSimdCall		AsTransformation( physx::PxTransform value );

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/PhysxMarshals.inl>
//------------------------------------------------------------------//