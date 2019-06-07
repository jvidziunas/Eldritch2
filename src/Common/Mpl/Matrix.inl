/*==================================================================*\
  Matrices.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <intrin.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETConstexpr ETForceInlineHint Matrix::Matrix(
	float32 m00, float32 m01, float32 m02, float32 m03,
	float32 m10, float32 m11, float32 m12, float32 m13,
	float32 m20, float32 m21, float32 m22, float32 m23,
	float32 m30, float32 m31, float32 m32, float32 m33) ETNoexceptHint : rows{ { m33, m32, m31, m30 }, { m23, m22, m21, m20 }, { m13, m12, m11, m10 }, { m03, m02, m01, m00 } } {}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint Matrix::Matrix(Vector row0, Vector row1, Vector row2, Vector row3) ETNoexceptHint : rows{ row3, row2, row1, row0 } {}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint Matrix::Matrix(const float32 values[]) ETNoexceptHint : Matrix(values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8], values[9], values[10], values[11], values[12], values[13], values[14], values[15]) {}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint Matrix::Matrix(const Vector values[]) ETNoexceptHint : Matrix(values[0], values[1], values[2], values[3]) {}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall operator+(Matrix lhs, Matrix rhs) ETNoexceptHint {
	lhs.rows[0] = _mm_add_ps(lhs.rows[0], rhs.rows[0]);
	lhs.rows[1] = _mm_add_ps(lhs.rows[1], rhs.rows[1]);
	lhs.rows[2] = _mm_add_ps(lhs.rows[2], rhs.rows[2]);
	lhs.rows[3] = _mm_add_ps(lhs.rows[3], rhs.rows[3]);

	return lhs;
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall operator+(float32 lhs, Matrix rhs) ETNoexceptHint {
	const __m128 scalar(_mm_set_ps1(lhs));

	rhs.rows[0] = _mm_add_ps(scalar, rhs.rows[0]);
	rhs.rows[1] = _mm_add_ps(scalar, rhs.rows[1]);
	rhs.rows[2] = _mm_add_ps(scalar, rhs.rows[2]);
	rhs.rows[3] = _mm_add_ps(scalar, rhs.rows[3]);

	return rhs;
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall operator+(Matrix lhs, float32 rhs) ETNoexceptHint {
	const __m128 scalar(_mm_set_ps1(rhs));

	lhs.rows[0] = _mm_add_ps(scalar, lhs.rows[0]);
	lhs.rows[1] = _mm_add_ps(scalar, lhs.rows[1]);
	lhs.rows[2] = _mm_add_ps(scalar, lhs.rows[2]);
	lhs.rows[3] = _mm_add_ps(scalar, lhs.rows[3]);

	return lhs;
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall operator-(Matrix lhs, Matrix rhs) ETNoexceptHint {
	lhs.rows[0] = _mm_sub_ps(lhs.rows[0], rhs.rows[0]);
	lhs.rows[1] = _mm_sub_ps(lhs.rows[1], rhs.rows[1]);
	lhs.rows[2] = _mm_sub_ps(lhs.rows[2], rhs.rows[2]);
	lhs.rows[3] = _mm_sub_ps(lhs.rows[3], rhs.rows[3]);

	return lhs;
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall operator-(float32 lhs, Matrix rhs) ETNoexceptHint {
	const __m128 scalar(_mm_set_ps1(lhs));

	rhs.rows[0] = _mm_sub_ps(scalar, rhs.rows[0]);
	rhs.rows[1] = _mm_sub_ps(scalar, rhs.rows[1]);
	rhs.rows[2] = _mm_sub_ps(scalar, rhs.rows[2]);
	rhs.rows[3] = _mm_sub_ps(scalar, rhs.rows[3]);

	return rhs;
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall operator-(Matrix lhs, float32 rhs) ETNoexceptHint {
	const __m128 scalar(_mm_set_ps1(rhs));

	lhs.rows[0] = _mm_sub_ps(scalar, lhs.rows[0]);
	lhs.rows[1] = _mm_sub_ps(scalar, lhs.rows[1]);
	lhs.rows[2] = _mm_sub_ps(scalar, lhs.rows[2]);
	lhs.rows[3] = _mm_sub_ps(scalar, lhs.rows[3]);

	return lhs;
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall operator*(Matrix lhs, float32 rhs)ETNoexceptHint {
	const __m128 scaleVector(_mm_set_ps1(rhs));

	lhs.rows[0] = _mm_mul_ps(lhs.rows[0], scaleVector);
	lhs.rows[1] = _mm_mul_ps(lhs.rows[1], scaleVector);
	lhs.rows[2] = _mm_mul_ps(lhs.rows[2], scaleVector);
	lhs.rows[3] = _mm_mul_ps(lhs.rows[3], scaleVector);

	return lhs;
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall operator*(float32 lhs, Matrix rhs)ETNoexceptHint {
	return rhs * lhs;
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall operator*(Matrix lhs, Matrix rhs)ETNoexceptHint {
	enum : int {
		//	Helper mask immediates used by transpose operations.
		AllSourceComponents = 0xF0,
		MaskX               = AllSourceComponents | 0x01,
		MaskY               = AllSourceComponents | 0x02,
		MaskZ               = AllSourceComponents | 0x04,
		MaskW               = AllSourceComponents | 0x08
	};

	// ---

	//	Prepare the operand by performing a matrix transpose.
	_MM_TRANSPOSE4_PS(rhs.rows[0], rhs.rows[1], rhs.rows[2], rhs.rows[3]);

	/*	Since _mm_dp_ps zeroes out all the elements not masked out, we can simply do a bitwise or and work on an individual component at a time instead of playing shuffling games.
	 *	This has the added beneficial side effect of allowing the processor to execute several dot products in parallel as there are fewer data dependencies over a naive implementation. */
	for (size_t row(0); row < ETCountOf(lhs.rows); ++row) {
		lhs.rows[row] = _mm_or_ps(
			_mm_or_ps(_mm_dp_ps(rhs.rows[0], lhs.rows[row], MaskX), _mm_dp_ps(rhs.rows[1], lhs.rows[row], MaskY)),
			_mm_or_ps(_mm_dp_ps(rhs.rows[2], lhs.rows[row], MaskZ), _mm_dp_ps(rhs.rows[3], lhs.rows[row], MaskW)));
	}

	return lhs;
}

// ---------------------------------------------------

ETForceInlineHint Matrix& ETSimdCall Matrix::operator+=(Matrix rhs) ETNoexceptHint {
	rows[0] = _mm_add_ps(rows[0], rhs.rows[0]);
	rows[1] = _mm_add_ps(rows[1], rhs.rows[1]);
	rows[2] = _mm_add_ps(rows[2], rhs.rows[2]);
	rows[3] = _mm_add_ps(rows[3], rhs.rows[3]);

	return *this;
}

// ---------------------------------------------------

ETForceInlineHint Matrix& ETSimdCall Matrix::operator-=(Matrix rhs) ETNoexceptHint {
	rows[0] = _mm_sub_ps(rows[0], rhs.rows[0]);
	rows[1] = _mm_sub_ps(rows[1], rhs.rows[1]);
	rows[2] = _mm_sub_ps(rows[2], rhs.rows[2]);
	rows[3] = _mm_sub_ps(rows[3], rhs.rows[3]);

	return *this;
}

// ---------------------------------------------------

ETForceInlineHint Matrix& ETSimdCall Matrix::operator*=(float32 rhs) ETNoexceptHint {
	const __m128 scale(_mm_set_ps1(rhs));

	rows[0] = _mm_mul_ps(rows[0], scale);
	rows[1] = _mm_mul_ps(rows[1], scale);
	rows[2] = _mm_mul_ps(rows[2], scale);
	rows[3] = _mm_mul_ps(rows[3], scale);

	return *this;
}

// ---------------------------------------------------

ETForceInlineHint Matrix& ETSimdCall Matrix::operator*=(Matrix rhs) ETNoexceptHint {
	enum : int {
		//	Helper mask immediates used by transpose operations.
		AllSourceComponents = 0xF0,
		MaskX               = AllSourceComponents | 0x01,
		MaskY               = AllSourceComponents | 0x02,
		MaskZ               = AllSourceComponents | 0x04,
		MaskW               = AllSourceComponents | 0x08
	};

	// ---

	//	Prepare the operand by performing a matrix transpose.
	_MM_TRANSPOSE4_PS(rhs.rows[0], rhs.rows[1], rhs.rows[2], rhs.rows[3]);

	/*	Since _mm_dp_ps zeroes out all the elements not masked out, we can simply do a bitwise or and work on an individual component at a time instead of playing shuffling games.
	 *	This has the added beneficial side effect of allowing the processor to execute several dot products in parallel as there are fewer data dependencies over a naive implementation. */
	rows[0] = _mm_or_ps(
		_mm_or_ps(_mm_dp_ps(rhs.rows[0], rows[0], MaskX), _mm_dp_ps(rhs.rows[1], rows[0], MaskY)),
		_mm_or_ps(_mm_dp_ps(rhs.rows[2], rows[0], MaskZ), _mm_dp_ps(rhs.rows[3], rows[0], MaskW)));
	rows[1] = _mm_or_ps(
		_mm_or_ps(_mm_dp_ps(rhs.rows[0], rows[1], MaskX), _mm_dp_ps(rhs.rows[1], rows[1], MaskY)),
		_mm_or_ps(_mm_dp_ps(rhs.rows[2], rows[1], MaskZ), _mm_dp_ps(rhs.rows[3], rows[1], MaskW)));
	rows[2] = _mm_or_ps(
		_mm_or_ps(_mm_dp_ps(rhs.rows[0], rows[2], MaskX), _mm_dp_ps(rhs.rows[1], rows[2], MaskY)),
		_mm_or_ps(_mm_dp_ps(rhs.rows[2], rows[2], MaskZ), _mm_dp_ps(rhs.rows[3], rows[2], MaskW)));
	rows[3] = _mm_or_ps(
		_mm_or_ps(_mm_dp_ps(rhs.rows[0], rows[3], MaskX), _mm_dp_ps(rhs.rows[1], rows[3], MaskY)),
		_mm_or_ps(_mm_dp_ps(rhs.rows[2], rows[3], MaskZ), _mm_dp_ps(rhs.rows[3], rows[3], MaskW)));

	return *this;
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall Matrix::AsRigidTransformation(Vector translation, Quaternion orientation) ETNoexceptHint {
	Matrix temp(AsRotation(orientation));
	temp.rows[3] = translation;
	return temp;
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall Matrix::AsRigidTransformation(Transformation transform) ETNoexceptHint {
	return AsRigidTransformation(transform.translation, transform.rotation);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall Matrix::MakeIdentity() ETNoexceptHint {
	return Matrix(Vector::MakeForward(), Vector::MakeUp(), Vector::MakeRight(), Vector::MakeZero_HomogenousPoint());
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint Matrix ETSimdCall Matrix::MakeZero() ETNoexceptHint {
	return Matrix(Vector::MakeZero(), Vector::MakeZero(), Vector::MakeZero(), Vector::MakeZero());
}

} // namespace Eldritch2
