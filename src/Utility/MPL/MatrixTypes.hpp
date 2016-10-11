/*==================================================================*\
  MatrixTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/VectorTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	RadianMeasure;
}

namespace Eldritch2 {

	class Float4x4 {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Default constructs this @ref Float4x4 instance.
	/*!	@remarks Note that this this does not initialize any members. */
		Float4x4() = default;
	//! Constructs this @ref Float4x4 instance.
	/*! @param[in] values 16-wide array of @ref float32 values that will be loaded into the matrix.
		@remarks This constructor uses unaligned loads and does not impose any alignment requirement on the input. */
		explicit Float4x4( const Eldritch2::float32* const values );
	//! Constructs this @ref Float4x4 instance.
	/*! @param[in] values 4-wide array (with four float32 elements per vector, total 16 elements) of @ref Float4 values that will be loaded into the matrix.
		@remarks This constructor uses unaligned loads and does not impose any alignment requirement on the input. */
		explicit Float4x4( Eldritch2::Float4 values[4] );
	// Constructs this @ref Float4x4 instance.
	/*! @param[in] row0 @ref Float4 element to be loaded into the first row of the matrix.
		@param[in] row1 @ref Float4 element to be loaded into the second row of the matrix.
		@param[in] row2 @ref Float4 element to be loaded into the third row of the matrix.
		@param[in] row3 @ref Float4 element to be loaded into the fourth row of the matrix. */
		Float4x4( Eldritch2::Float4 row0, Eldritch2::Float4 row1, Eldritch2::Float4 row2, Eldritch2::Float4 row3 );
	//!	Constructs this @ref Float4x4 instance.
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
		Float4x4( Eldritch2::float32 m00, Eldritch2::float32 m01, Eldritch2::float32 m02, Eldritch2::float32 m03,
				  Eldritch2::float32 m10, Eldritch2::float32 m11, Eldritch2::float32 m12, Eldritch2::float32 m13,
				  Eldritch2::float32 m20, Eldritch2::float32 m21, Eldritch2::float32 m22, Eldritch2::float32 m23,
				  Eldritch2::float32 m30, Eldritch2::float32 m31, Eldritch2::float32 m32, Eldritch2::float32 m33 );

	// ---------------------------------------------------

	public:
	//! Adds one @ref Float4x4 to another.
	/*! @param[in] operand0 Base matrix.
		@param[in] operand1 @ref Float4x4 to add to _operand0_.
		@returns The sum of the two matrices, according to common mathematical definition (component-wise addition). */
		friend ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall	operator+( Eldritch2::Float4x4 operand0, Eldritch2::Float4x4 operand1 );

	//! Subtracts one @ref Float4x4 from another.
	/*! @param[in] operand0 Base matrix.
		@param[in] operand1 @ref Float4x4 to subtract from _operand0_.
		@returns The difference of the two matrices, according to common mathematical definition (component-wise subtraction). */
		friend ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall	operator-( Eldritch2::Float4x4 operand0, Eldritch2::Float4x4 operand1 );

	//! Multiplies a @ref Float4x4 by a scalar.
	/*! @param[in] matrix @ref Float4x4 to multiply.
		@param[in] scalar Scalar to multiply the elements in _matrix_ by.
		@returns The scaled matrix. */
		friend ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall	operator*( Eldritch2::Float4x4 matrix, Eldritch2::float32 scalar );
	//! Multiplies a @ref Float4x4 by a scalar.
	/*!	@param[in] scalar Scalar to multiply the elements in _matrix_ by.
		@param[in] matrix @ref Float4x4 to multiply.
		@returns The scaled matrix. */
		friend ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall	operator*( Eldritch2::float32 scalar, Eldritch2::Float4x4 matrix );

		friend ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall	operator*( Eldritch2::Float4x4 operand0, Eldritch2::Float4x4 operand1 );

		Eldritch2::Float4x4& ETSimdCall								operator+=( Eldritch2::Float4x4 operand );
		Eldritch2::Float4x4& ETSimdCall								operator-=( Eldritch2::Float4x4 operand );
		Eldritch2::Float4x4& ETSimdCall								operator*=( Eldritch2::float32 scalar );
		Eldritch2::Float4x4& ETSimdCall								operator*=( Eldritch2::Float4x4 matrix );

		Eldritch2::Float4x4& ETSimdCall								operator=( Eldritch2::Float4x4 operand );

	// ---------------------------------------------------

	public:
		static ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall	Perspective( Eldritch2::RadianMeasure horizontalFieldOfView, Eldritch2::RadianMeasure verticalFieldOfView, Eldritch2::float32 nearDistance, Eldritch2::float32 farDistance );
		static ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall	Perspective( Eldritch2::RadianMeasure horizontalFieldOfView, Eldritch2::float32 aspect, Eldritch2::float32 nearDistance, Eldritch2::float32 farDistance );

		static ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall	Rotation( Eldritch2::Quaternion orientation );

		static ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall	RigidTransformation( Eldritch2::Float4 translation, Eldritch2::Quaternion orientation );
		static ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall	RigidTransformation( RigidTransform transform );

		static ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall	Identity();

		static ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall	Zero();

	// ---------------------------------------------------

	public:
		Eldritch2::Float4	elements[4];
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/MPL/MatrixTypes.inl>
//------------------------------------------------------------------//