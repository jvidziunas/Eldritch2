/*==================================================================*\
  TypeMarshals.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <characterkinematic/PxExtended.h>
#include <foundation/PxTransform.h>
#include <foundation/PxVec3.h>
#include <foundation/PxQuat.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	ETInlineHint ETPureFunctionHint physx::PxExtendedVec3 ETSimdCall AsPxExtendedVec3(Vector value) {
		float32 coefficients[4];

		value.ExtractCoefficients(coefficients);

		return { coefficients[0], coefficients[1], coefficients[2] };
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint physx::PxVec3 ETSimdCall AsPxVec3(Vector value) {
		float32 coefficients[4];

		value.ExtractCoefficients(coefficients);

		return { coefficients[0], coefficients[1], coefficients[2] };
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall AsVector(physx::PxExtendedVec3 value) {
		return { static_cast<float32>(value.x), static_cast<float32>(value.y), static_cast<float32>(value.z), 0.0f };
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall AsVector(physx::PxVec3 value) {
		return { value.x, value.y, value.z, 0.0f };
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Quaternion ETSimdCall AsQuaternion(physx::PxQuat value) {
		return { value.x, value.y, value.z, value.w };
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint physx::PxQuat ETSimdCall AsPxQuat(Quaternion value) {
		float32 coefficients[4];

		value.ExtractCoefficients(coefficients);

		return { coefficients[0], coefficients[1], coefficients[2], coefficients[3] };
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Transformation ETSimdCall AsTransformation(physx::PxTransform transform) {
		return { AsVector(transform.p), AsQuaternion(transform.q) };
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint physx::PxTransform ETSimdCall AsPxTransform(Transformation transform) {
		return { AsPxVec3(transform.translation), AsPxQuat(transform.rotation) };
	}

}}} // namespace Eldritch2::Physics::PhysX
