/*==================================================================*\
  Matrices.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Vector.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
class Angle;
} // namespace Eldritch2

namespace Eldritch2 {

class Matrix {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Matrix instance.
	/*! @param[in] m00 element to be loaded into the matrix at the [0,0]th position.
		@param[in] m01 element to be loaded into the matrix at the [0,1]th position.
		@param[in] m02 element to be loaded into the matrix at the [0,2]th position.
		@param[in] m03 element to be loaded into the matrix at the [0,3]th position.
		@param[in] m10 element to be loaded into the matrix at the [1,0]th position.
		@param[in] m11 element to be loaded into the matrix at the [1,1]th position.
		@param[in] m12 element to be loaded into the matrix at the [1,2]th position.
		@param[in] m13 element to be loaded into the matrix at the [1,3]th position.
		@param[in] m20 element to be loaded into the matrix at the [2,0]th position.
		@param[in] m21 element to be loaded into the matrix at the [2,1]th position.
		@param[in] m22 element to be loaded into the matrix at the [2,2]th position.
		@param[in] m23 element to be loaded into the matrix at the [2,3]th position.
		@param[in] m30 element to be loaded into the matrix at the [3,0]th position.
		@param[in] m31 element to be loaded into the matrix at the [3,1]th position.
		@param[in] m32 element to be loaded into the matrix at the [3,2]th position.
		@param[in] m33 element to be loaded into the matrix at the [3,3]th position. */
	ETConstexpr Matrix(
		float32 m00, float32 m01, float32 m02, float32 m03,
		float32 m10, float32 m11, float32 m12, float32 m13,
		float32 m20, float32 m21, float32 m22, float32 m23,
		float32 m30, float32 m31, float32 m32, float32 m33) ETNoexceptHint;
	//! Constructs this @ref Matrix instance.
	/*! @param[in] row0 @ref Vector element to be loaded into the first row of the matrix.
		@param[in] row1 @ref Vector element to be loaded into the second row of the matrix.
		@param[in] row2 @ref Vector element to be loaded into the third row of the matrix.
		@param[in] row3 @ref Vector element to be loaded into the fourth row of the matrix. */
	ETConstexpr Matrix(Vector row0, Vector row1, Vector row2, Vector row3) ETNoexceptHint;
	//! Constructs this @ref Matrix instance.
	/*! @param[in] values 16-wide array of @ref float32 values that will be loaded into the matrix.
		@remarks This constructor uses unaligned loads and does not impose any alignment requirement on the input. */
	explicit ETConstexpr Matrix(const float32 values[]) ETNoexceptHint;
	//! Constructs this @ref Matrix instance.
	/*! @param[in] values 4-wide array (with four float32 elements per vector, total 16 elements) of @ref Vector values that will be loaded into the matrix.
		@remarks This constructor uses unaligned loads and does not impose any alignment requirement on the input. */
	explicit ETConstexpr Matrix(const Vector values[]) ETNoexceptHint;
	//! Constructs this @ref Matrix instance.
	ETConstexpr Matrix(const Matrix&) ETNoexceptHint = default;
	//! Constructs this @ref Matrix instance.
	/*!	@remarks Note that this this does not initialize any members. */
	Matrix() = default;

	// ---------------------------------------------------

public:
	//! Adds one @ref Matrix to another.
	friend ETPureFunctionHint Matrix ETSimdCall operator+(Matrix, Matrix) ETNoexceptHint;
	//! Adds a scalar to each element of a @ref Matrix.
	friend ETPureFunctionHint Matrix ETSimdCall operator+(Matrix, float32) ETNoexceptHint;
	//! Adds a scalar to each element of a @ref Matrix.
	friend ETPureFunctionHint Matrix ETSimdCall operator+(float32, Matrix) ETNoexceptHint;

	//! Subtracts one @ref Matrix from another.
	friend ETPureFunctionHint Matrix ETSimdCall operator-(Matrix, Matrix) ETNoexceptHint;
	//! Subtracts each element of a @ref Matrix
	friend ETPureFunctionHint Matrix ETSimdCall operator-(float32, Matrix) ETNoexceptHint;
	//! Subtracts a scalar from each element of a @ref Matrix.
	friend ETPureFunctionHint Matrix ETSimdCall operator-(Matrix, float32) ETNoexceptHint;

	//! Multiplies a @ref Matrix by another @ref Matrix.
	friend ETPureFunctionHint Matrix ETSimdCall operator*(Matrix, Matrix) ETNoexceptHint;
	//! Multiplies a @ref Matrix by a scalar.
	friend ETPureFunctionHint Matrix ETSimdCall operator*(Matrix, float32) ETNoexceptHint;
	//! Multiplies a @ref Matrix by a scalar.
	friend ETPureFunctionHint Matrix ETSimdCall operator*(float32, Matrix) ETNoexceptHint;
	
	friend ETPureFunctionHint Matrix ETSimdCall Transpose(Matrix matrix) ETNoexceptHint;

	Matrix& ETSimdCall operator+=(Matrix) ETNoexceptHint;
	Matrix& ETSimdCall operator-=(Matrix) ETNoexceptHint;
	Matrix& ETSimdCall operator*=(float32) ETNoexceptHint;
	Matrix& ETSimdCall operator*=(Matrix) ETNoexceptHint;

	ETConstexpr Matrix& ETSimdCall operator=(const Matrix&) ETNoexceptHint = default;

	// ---------------------------------------------------

public:
	static ETPureFunctionHint Matrix ETSimdCall AsPerspectiveProjection(Angle horizontalFov, Angle verticalFov, float32 nearDistance, float32 farDistance) ETNoexceptHint;
	static ETPureFunctionHint Matrix ETSimdCall AsPerspectiveProjection(Angle horizontalFov, Angle verticalFov, float32 nearDistance) ETNoexceptHint;

	static ETPureFunctionHint Matrix ETSimdCall AsRotation(Quaternion orientation) ETNoexceptHint;

	static ETPureFunctionHint Matrix ETSimdCall AsRigidTransformation(Vector translation, Quaternion orientation) ETNoexceptHint;
	static ETPureFunctionHint Matrix ETSimdCall AsRigidTransformation(Transformation transform) ETNoexceptHint;

	static ETConstexpr ETPureFunctionHint Matrix ETSimdCall MakeIdentity() ETNoexceptHint;

	static ETConstexpr ETPureFunctionHint Matrix ETSimdCall MakeZero() ETNoexceptHint;

	// ---------------------------------------------------

public:
	__m128 rows[4];
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Mpl/Matrix.inl>
//------------------------------------------------------------------//
