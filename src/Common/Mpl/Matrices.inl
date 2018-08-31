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

ETInlineHint Matrix4x4::Matrix4x4(const float32* const values) {
	rows[0] = _mm_loadu_ps(values);
	rows[1] = _mm_loadu_ps(values + 4);
	rows[2] = _mm_loadu_ps(values + 8);
	rows[3] = _mm_loadu_ps(values + 16);
}

// ---------------------------------------------------

ETInlineHint Matrix4x4::Matrix4x4(Vector values[4]) {
	rows[0] = values[0];
	rows[1] = values[1];
	rows[2] = values[2];
	rows[3] = values[3];
}

// ---------------------------------------------------

ETInlineHint Matrix4x4::Matrix4x4(
	Vector row0,
	Vector row1,
	Vector row2,
	Vector row3) {
	rows[0] = row0;
	rows[1] = row1;
	rows[2] = row2;
	rows[3] = row3;
}

// ---------------------------------------------------

ETInlineHint Matrix4x4::Matrix4x4(
	float32 m00, float32 m01, float32 m02, float32 m03,
	float32 m10, float32 m11, float32 m12, float32 m13,
	float32 m20, float32 m21, float32 m22, float32 m23,
	float32 m30, float32 m31, float32 m32, float32 m33) {
	rows[0] = _mm_set_ps(m02, m01, m00, m03);
	rows[1] = _mm_set_ps(m12, m11, m10, m13);
	rows[2] = _mm_set_ps(m22, m21, m20, m23);
	rows[3] = _mm_set_ps(m32, m31, m30, m33);
}

// ---------------------------------------------------

ETInlineHint ETPureFunctionHint Matrix4x4 ETSimdCall operator+(Matrix4x4 operand0, Matrix4x4 operand1) {
	operand0.rows[0] = _mm_add_ps(operand0.rows[0], operand1.rows[0]);
	operand0.rows[1] = _mm_add_ps(operand0.rows[1], operand1.rows[1]);
	operand0.rows[2] = _mm_add_ps(operand0.rows[2], operand1.rows[2]);
	operand0.rows[3] = _mm_add_ps(operand0.rows[3], operand1.rows[3]);

	return operand0;
}

// ---------------------------------------------------

ETInlineHint ETPureFunctionHint Matrix4x4 ETSimdCall operator-(Matrix4x4 operand0, Matrix4x4 operand1) {
	operand0.rows[0] = _mm_sub_ps(operand0.rows[0], operand1.rows[0]);
	operand0.rows[1] = _mm_sub_ps(operand0.rows[1], operand1.rows[1]);
	operand0.rows[2] = _mm_sub_ps(operand0.rows[2], operand1.rows[2]);
	operand0.rows[3] = _mm_sub_ps(operand0.rows[3], operand1.rows[3]);

	return operand0;
}

// ---------------------------------------------------

ETInlineHint ETPureFunctionHint Matrix4x4 ETSimdCall operator*(Matrix4x4 operand, float32 scalar) {
	const __m128 scaleVector(_mm_set_ps1(scalar));

	operand.rows[0] = _mm_mul_ps(operand.rows[0], scaleVector);
	operand.rows[1] = _mm_mul_ps(operand.rows[1], scaleVector);
	operand.rows[2] = _mm_mul_ps(operand.rows[2], scaleVector);
	operand.rows[3] = _mm_mul_ps(operand.rows[3], scaleVector);

	return operand;
}

// ---------------------------------------------------

ETInlineHint ETPureFunctionHint Matrix4x4 ETSimdCall operator*(float32 scalar, Matrix4x4 operand) {
	return operand * scalar;
}

// ---------------------------------------------------

ETInlineHint ETPureFunctionHint Matrix4x4 ETSimdCall operator*(Matrix4x4 operand0, Matrix4x4 operand1) {
	enum : int {
		//	Helper mask immediates used by transpose operations.
		ALL_SOURCE_COMPONENTS = 0xF0,
		X_MASK                = ALL_SOURCE_COMPONENTS | 0x01,
		Y_MASK                = ALL_SOURCE_COMPONENTS | 0x02,
		Z_MASK                = ALL_SOURCE_COMPONENTS | 0x04,
		W_MASK                = ALL_SOURCE_COMPONENTS | 0x08
	};

	// ---

	//	Prepare the operand by performing a matrix transpose.
	_MM_TRANSPOSE4_PS(operand1.rows[0], operand1.rows[1], operand1.rows[2], operand1.rows[3]);

	/*	Since _mm_dp_ps zeroes out all the elements not masked out, we can simply do a bitwise or and work on an individual component at a time instead of playing shuffling games.
	 *	This has the added beneficial side effect of allowing the processor to execute several dot products in parallel as there are fewer data dependencies over a naive implementation. */
	for (size_t row(0); row < ETCountOf(operand0.rows); ++row) {
		operand0.rows[row] = _mm_or_ps(
			_mm_or_ps(_mm_dp_ps(operand1.rows[0], operand0.rows[row], X_MASK), _mm_dp_ps(operand1.rows[1], operand0.rows[row], Y_MASK)),
			_mm_or_ps(_mm_dp_ps(operand1.rows[2], operand0.rows[row], Z_MASK), _mm_dp_ps(operand1.rows[3], operand0.rows[row], W_MASK)));
	}

	return operand0;
}

// ---------------------------------------------------

ETInlineHint Matrix4x4& ETSimdCall Matrix4x4::operator+=(Matrix4x4 operand) {
	rows[0] = _mm_add_ps(rows[0], operand.rows[0]);
	rows[1] = _mm_add_ps(rows[1], operand.rows[1]);
	rows[2] = _mm_add_ps(rows[2], operand.rows[2]);
	rows[3] = _mm_add_ps(rows[3], operand.rows[3]);

	return *this;
}

// ---------------------------------------------------

ETInlineHint Matrix4x4& ETSimdCall Matrix4x4::operator-=(Matrix4x4 operand) {
	rows[0] = _mm_sub_ps(rows[0], operand.rows[0]);
	rows[1] = _mm_sub_ps(rows[1], operand.rows[1]);
	rows[2] = _mm_sub_ps(rows[2], operand.rows[2]);
	rows[3] = _mm_sub_ps(rows[3], operand.rows[3]);

	return *this;
}

// ---------------------------------------------------

ETInlineHint Matrix4x4& ETSimdCall Matrix4x4::operator*=(float32 scalar) {
	const __m128 scaleVector(_mm_set_ps1(scalar));

	rows[0] = _mm_mul_ps(rows[0], scaleVector);
	rows[1] = _mm_mul_ps(rows[1], scaleVector);
	rows[2] = _mm_mul_ps(rows[2], scaleVector);
	rows[3] = _mm_mul_ps(rows[3], scaleVector);

	return *this;
}

// ---------------------------------------------------

ETInlineHint Matrix4x4& ETSimdCall Matrix4x4::operator*=(Matrix4x4 matrix) {
	enum : int {
		//	Helper mask immediates used by transpose operations.
		ALL_SOURCE_COMPONENTS = 0xF0,
		X_MASK                = ALL_SOURCE_COMPONENTS | 0x01,
		Y_MASK                = ALL_SOURCE_COMPONENTS | 0x02,
		Z_MASK                = ALL_SOURCE_COMPONENTS | 0x04,
		W_MASK                = ALL_SOURCE_COMPONENTS | 0x08
	};

	// ---

	//	Prepare the operand by performing a matrix transpose.
	_MM_TRANSPOSE4_PS(matrix.rows[0], matrix.rows[1], matrix.rows[2], matrix.rows[3]);

	/*	Since _mm_dp_ps zeroes out all the elements not masked out, we can simply do a bitwise or and work on an individual component at a time instead of playing shuffling games.
	 *	This has the added beneficial side effect of allowing the processor to execute several dot products in parallel as there are fewer data dependencies over a naive implementation. */
	for (size_t row(0); row < ETCountOf(rows); ++row) {
		rows[row] = _mm_or_ps(
			_mm_or_ps(_mm_dp_ps(matrix.rows[0], rows[row], X_MASK), _mm_dp_ps(matrix.rows[1], rows[row], Y_MASK)),
			_mm_or_ps(_mm_dp_ps(matrix.rows[2], rows[row], Z_MASK), _mm_dp_ps(matrix.rows[3], rows[row], W_MASK)));
	}

	return *this;
}

// ---------------------------------------------------

ETInlineHint Matrix4x4& ETSimdCall Matrix4x4::operator=(Matrix4x4 operand) {
	rows[0] = operand.rows[0];
	rows[1] = operand.rows[1];
	rows[2] = operand.rows[2];
	rows[3] = operand.rows[3];

	return *this;
}

} // namespace Eldritch2
