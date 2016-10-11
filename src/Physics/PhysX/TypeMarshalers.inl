/*==================================================================*\
  TypeMarshalers.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Mpl/VectorTypes.hpp>
//------------------------------------------------------------------//
#include <characterkinematic/PxExtended.h>
#include <foundation/PxTransform.h>
#include <foundation/PxVec3.h>
#include <foundation/PxQuat.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	ETInlineHint ETPureFunctionHint physx::PxExtendedVec3 ETSimdCall PxExtendedVec3FromFloat4( Float4 value ) {
		return { value[0], value[1], value[2] };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint physx::PxVec3 ETSimdCall PxVec3FromFloat4( Eldritch2::Float4 value ) {
		return { value[0], value[1], value[2] };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 ETSimdCall Float4FromExtendedVec3( physx::PxExtendedVec3 value ) {
		return { static_cast<float32>(value.x), static_cast<float32>(value.y), static_cast<float32>(value.z), 0.0f };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 ETSimdCall Float4FromPxVec3( physx::PxVec3 value ) {
		return { value.x, value.y, value.z, 0.0f };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Quaternion ETSimdCall QuaternionFromPxQuat( physx::PxQuat value ) {
		return { value.x, value.y, value.z, value.w };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint physx::PxQuat ETSimdCall PxQuatFromQuaternion( Eldritch2::Quaternion value ) {
		return { value[0], value[1], value[2], value[3] };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::RigidTransform ETSimdCall RigidTransformFromPxTransform( physx::PxTransform transform ) {
		return { Float4FromPxVec3( transform.p ), QuaternionFromPxQuat( transform.q ) };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint physx::PxTransform ETSimdCall PxTransformFromRigidTransform( Eldritch2::RigidTransform value ) {
		return { PxVec3FromFloat4( value.translation ), PxQuatFromQuaternion( value.orientation ) };
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2