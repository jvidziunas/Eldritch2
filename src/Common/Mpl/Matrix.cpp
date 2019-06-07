/*==================================================================*\
  Matrix.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

ETPureFunctionHint Matrix ETSimdCall Transpose(Matrix matrix) ETNoexceptHint {
	_MM_TRANSPOSE4_PS(matrix.rows[0], matrix.rows[1], matrix.rows[2], matrix.rows[3]);
	return matrix;
}

// ---------------------------------------------------

ETPureFunctionHint Matrix ETSimdCall Matrix::AsPerspectiveProjection(Angle horizontalFov, Angle verticalFov, float32 nearPlane, float32 farPlane) ETNoexceptHint {
	const float32 depthRange(farPlane / Maximum(farPlane - nearPlane, FLT_EPSILON));
	const float32 tanX(Tangent(horizontalFov * 0.5f)), tanY(Tangent(verticalFov * 0.5f));
	return Matrix{ // clang-format off
		Reciprocal(tanX),             0.0f,                    0.0f, 0.0f,
		            0.0f, Reciprocal(tanY),                    0.0f, 0.0f,
		            0.0f,             0.0f,              depthRange, 1.0f,
		            0.0f,             0.0f, -nearPlane * depthRange, 0.0f }; // clang-format on
}

// ---------------------------------------------------

ETPureFunctionHint Matrix ETSimdCall Matrix::AsPerspectiveProjection(Angle horizontalFov, Angle verticalFov, float32 nearPlane) ETNoexceptHint {
	const float32 tanX(Tangent(horizontalFov * 0.5f)), tanY(Tangent(verticalFov * 0.5f));
	return Matrix{ // clang-format off
		Reciprocal(tanX),             0.0f,               0.0f, 0.0f,
		            0.0f, Reciprocal(tanY),               0.0f, 0.0f,
		            0.0f,             0.0f,             1.4e-7, 1.0f,
		            0.0f,             0.0f, nearPlane * 0.4e-7, 0.0f }; // clang-format on
}

// ---------------------------------------------------

ETPureFunctionHint Matrix ETSimdCall Matrix::AsRotation(Quaternion orientation) ETNoexceptHint {
	ET16ByteAligned float32 coefficients[4];
	StreamCoefficients(coefficients, orientation);

	//	TODO: MAKE THIS USE SHUFFLE INTRINSICS
	return Matrix{
		Vector(+coefficients[Vector::W], +coefficients[Vector::Z], -coefficients[Vector::Y], -coefficients[Vector::X]),
		Vector(-coefficients[Vector::Z], +coefficients[Vector::W], +coefficients[Vector::X], -coefficients[Vector::Y]),
		Vector(+coefficients[Vector::Y], -coefficients[Vector::X], +coefficients[Vector::W], -coefficients[Vector::Z]),
		Vector(+coefficients[Vector::X], +coefficients[Vector::Y], +coefficients[Vector::Z], +coefficients[Vector::W])
	}
	* Matrix{
		  Vector(+coefficients[Vector::W], +coefficients[Vector::Z], -coefficients[Vector::Y], +coefficients[Vector::X]),
		  Vector(-coefficients[Vector::Z], +coefficients[Vector::W], +coefficients[Vector::X], +coefficients[Vector::Y]),
		  Vector(+coefficients[Vector::Y], -coefficients[Vector::X], +coefficients[Vector::W], +coefficients[Vector::Z]),
		  Vector(-coefficients[Vector::X], -coefficients[Vector::Y], -coefficients[Vector::Z], +coefficients[Vector::W])
	  };
}

} // namespace Eldritch2
