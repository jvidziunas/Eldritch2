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
#include <Common/Mpl/Vectors.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Angle;
}

namespace Eldritch2 {

	class Matrix4x4 {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Matrix4x4 instance.
	/*! @param[in] m00 @ref float32 element to be loaded into the matrix at the [0,0]th position.
		@param[in] m01 @ref float32 element to be loaded into the matrix at the [0,1]th position.
		@param[in] m02 @ref float32 element to be loaded into the matrix at the [0,2]th position.
		@param[in] m03 @ref float32 element to be loaded into the matrix at the [0,3]th position.
		@param[in] m10 @ref float32 element to be loaded into the matrix at the [1,0]th position.
		@param[in] m11 @ref float32 element to be loaded into the matrix at the [1,1]th position.
		@param[in] m12 @ref float32 element to be loaded into the matrix at the [1,2]th position.
		@param[in] m13 @ref float32 element to be loaded into the matrix at the [1,3]th position.
		@param[in] m20 @ref float32 element to be loaded into the matrix at the [2,0]th position.
		@param[in] m21 @ref float32 element to be loaded into the matrix at the [2,1]th position.
		@param[in] m22 @ref float32 element to be loaded into the matrix at the [2,2]th position.
		@param[in] m23 @ref float32 element to be loaded into the matrix at the [2,3]th position.
		@param[in] m30 @ref float32 element to be loaded into the matrix at the [3,0]th position.
		@param[in] m31 @ref float32 element to be loaded into the matrix at the [3,1]th position.
		@param[in] m32 @ref float32 element to be loaded into the matrix at the [3,2]th position.
		@param[in] m33 @ref float32 element to be loaded into the matrix at the [3,3]th position. */
		Matrix4x4(
			float32 m00, float32 m01, float32 m02, float32 m03,
			float32 m10, float32 m11, float32 m12, float32 m13,
			float32 m20, float32 m21, float32 m22, float32 m23,
			float32 m30, float32 m31, float32 m32, float32 m33
		);
	//! Constructs this @ref Matrix4x4 instance.
	/*! @param[in] row0 @ref Vector element to be loaded into the first row of the matrix.
		@param[in] row1 @ref Vector element to be loaded into the second row of the matrix.
		@param[in] row2 @ref Vector element to be loaded into the third row of the matrix.
		@param[in] row3 @ref Vector element to be loaded into the fourth row of the matrix. */
		Matrix4x4( Vector row0, Vector row1, Vector row2, Vector row3 );
	//! Constructs this @ref Matrix4x4 instance.
	/*! @param[in] values 16-wide array of @ref float32 values that will be loaded into the matrix.
		@remarks This constructor uses unaligned loads and does not impose any alignment requirement on the input. */
		explicit Matrix4x4( const float32* const values );
	//! Constructs this @ref Matrix4x4 instance.
	/*! @param[in] values 4-wide array (with four float32 elements per vector, total 16 elements) of @ref Vector values that will be loaded into the matrix.
		@remarks This constructor uses unaligned loads and does not impose any alignment requirement on the input. */
		explicit Matrix4x4( Vector values[4] );
	//! Constructs this @ref Matrix4x4 instance.
	/*!	@remarks Note that this this does not initialize any members. */
		Matrix4x4() = default;

	// ---------------------------------------------------

	public:
	//! Adds one @ref Matrix4x4 to another.
	/*! @param[in] operand0 Base matrix.
		@param[in] operand1 @ref Matrix4x4 to add to _operand0_.
		@returns The sum of the two matrices, according to common mathematical definition (component-wise addition). */
		friend ETPureFunctionHint Matrix4x4 ETSimdCall	operator+( Matrix4x4 operand0, Matrix4x4 operand1 );

	//! Subtracts one @ref Matrix4x4 from another.
	/*! @param[in] operand0 Base matrix.
		@param[in] operand1 @ref Matrix4x4 to subtract from _operand0_.
		@returns The difference of the two matrices, according to common mathematical definition (component-wise subtraction). */
		friend ETPureFunctionHint Matrix4x4 ETSimdCall	operator-( Matrix4x4 operand0, Matrix4x4 operand1 );

	//! Multiplies a @ref Matrix4x4 by a scalar.
	/*! @param[in] matrix @ref Matrix4x4 to multiply.
		@param[in] scalar Scalar to multiply the elements in _matrix_ by.
		@returns The scaled matrix. */
		friend ETPureFunctionHint Matrix4x4 ETSimdCall	operator*( Matrix4x4 matrix, float32 scalar );
	//! Multiplies a @ref Matrix4x4 by a scalar.
	/*!	@param[in] scalar Scalar to multiply the elements in _matrix_ by.
		@param[in] matrix @ref Matrix4x4 to multiply.
		@returns The scaled matrix. */
		friend ETPureFunctionHint Matrix4x4 ETSimdCall	operator*( float32 scalar, Matrix4x4 matrix );

		friend ETPureFunctionHint Matrix4x4 ETSimdCall	operator*( Matrix4x4 operand0, Matrix4x4 operand1 );

		Matrix4x4& ETSimdCall							operator+=( Matrix4x4 operand );
		Matrix4x4& ETSimdCall							operator-=( Matrix4x4 operand );
		Matrix4x4& ETSimdCall							operator*=( float32 scalar );
		Matrix4x4& ETSimdCall							operator*=( Matrix4x4 matrix );

		Matrix4x4& ETSimdCall							operator=( Matrix4x4 operand );

	// ---------------------------------------------------

	public:
		static ETPureFunctionHint Matrix4x4 ETSimdCall	AsPerspectiveProjection( Angle horizontalFov, Angle verticalFov, float32 nearDistance, float32 farDistance );
		static ETPureFunctionHint Matrix4x4 ETSimdCall	AsPerspectiveProjection( Angle verticalFov, float32 aspect, float32 nearDistance, float32 farDistance );

		static ETPureFunctionHint Matrix4x4 ETSimdCall	AsRotation( Quaternion orientation );

		static ETPureFunctionHint Matrix4x4 ETSimdCall	AsRigidTransformation( Vector translation, Quaternion orientation );
		static ETPureFunctionHint Matrix4x4 ETSimdCall	AsRigidTransformation( Transformation transform );

		static ETPureFunctionHint Matrix4x4 ETSimdCall	Identity();

		static ETPureFunctionHint Matrix4x4 ETSimdCall	Zero();

	// ---------------------------------------------------

	public:
		__m128	rows[4];
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Mpl/Matrices.inl>
//------------------------------------------------------------------//