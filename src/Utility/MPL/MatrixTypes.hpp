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
		/*!	@remarks Note that this this does not initialize any members.
			*/
		ETForceInlineHint Float4x4() = default;
		// Constructs this @ref Float4x4 instance.
		/*! @param[in] values 16-wide array of @ref float32 values that will be loaded into the matrix.
			@remarks This constructor uses unaligned loads and does not impose any alignment requirement on the input.
			*/
		ETForceInlineHint explicit Float4x4( const ::Eldritch2::float32* const values );
		// Constructs this @ref Float4x4 instance.
		/*! @param[in] values 4-wide array (with four float32 elements per vector, total 16 elements) of Float4 values that will be loaded into the matrix.
			@remarks This constructor uses unaligned loads and does not impose any alignment requirement on the input.
			*/
		ETForceInlineHint explicit Float4x4( const ::Eldritch2::Float4* const values );
		// Constructs this @ref Float4x4 instance.
		/*! @param[in] row0 @ref Float4 element to be loaded into the first row of the matrix.
			@param[in] row1 @ref Float4 element to be loaded into the second row of the matrix.
			@param[in] row2 @ref Float4 element to be loaded into the third row of the matrix.
			@param[in] row3 @ref Float4 element to be loaded into the fourth row of the matrix.
			*/
		ETForceInlineHint Float4x4( const ::Eldritch2::Float4 row0, const ::Eldritch2::Float4 row1, const ::Eldritch2::Float4 row2, const ::Eldritch2::Float4 row3 );
		// Constructs this @ref Float4x4 instance.
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
			@param[in] m33 @ref float32 element to be loaded into the matrix at the [3,3]th position.
			*/
		ETForceInlineHint Float4x4( const ::Eldritch2::float32 m00, const ::Eldritch2::float32 m01, const ::Eldritch2::float32 m02, const ::Eldritch2::float32 m03,
									const ::Eldritch2::float32 m10, const ::Eldritch2::float32 m11, const ::Eldritch2::float32 m12, const ::Eldritch2::float32 m13,
									const ::Eldritch2::float32 m20, const ::Eldritch2::float32 m21, const ::Eldritch2::float32 m22, const ::Eldritch2::float32 m23,
									const ::Eldritch2::float32 m30, const ::Eldritch2::float32 m31, const ::Eldritch2::float32 m32, const ::Eldritch2::float32 m33 );

	// ---------------------------------------------------

		//! Adds one @ref Float4x4 to another.
		/*! @param[in] operand0 Base matrix.
			@param[in] operand1 @ref Float4x4 to add to _operand0_.
			@returns The sum of the two matrices, according to common mathematical definition (component-wise addition).
			*/
		friend ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4x4	operator +( const ::Eldritch2::Float4x4 operand0, const ::Eldritch2::Float4x4 operand1 );

		//! Subtracts one @ref Float4x4 from another.
		/*! @param[in] operand0 Base matrix.
			@param[in] operand1 @ref Float4x4 to subtract from _operand0_.
			@returns The difference of the two matrices, according to common mathematical definition (component-wise subtraction).
			*/
		friend ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4x4	operator -( const ::Eldritch2::Float4x4 operand0, const ::Eldritch2::Float4x4 operand1 );

		//! Multiplies a @ref Float4x4 by a scalar.
		/*! @param[in] matrix @ref Float4x4 to multiply.
			@param[in] scalar Scalar to multiply the elements in _matrix_ by.
			@returns The scaled matrix.
			*/
		friend ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4x4	operator *( const ::Eldritch2::Float4x4 matrix, const ::Eldritch2::float32 scalar );
		//! Multiplies a @ref Float4x4 by a scalar.
		/*!	@param[in] scalar Scalar to multiply the elements in _matrix_ by.
			@param[in] matrix @ref Float4x4 to multiply.
			@returns The scaled matrix.
			*/
		friend ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4x4	operator *( const ::Eldritch2::float32 scalar, const ::Eldritch2::Float4x4 matrix );

		friend ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4x4	operator *( const ::Eldritch2::Float4x4 operand0, const ::Eldritch2::Float4x4 operand1 );

		ETForceInlineHint ::Eldritch2::Float4x4&	operator +=( const ::Eldritch2::Float4x4 operand );
		ETForceInlineHint ::Eldritch2::Float4x4&	operator -=( const ::Eldritch2::Float4x4 operand );
		ETForceInlineHint ::Eldritch2::Float4x4&	operator *=( const ::Eldritch2::float32 scalar );
		ETForceInlineHint ::Eldritch2::Float4x4&	operator *=( const ::Eldritch2::Float4x4 matrix );

		ETForceInlineHint ::Eldritch2::Float4x4&	operator =( const ::Eldritch2::Float4x4 operand );

	// ---------------------------------------------------

		explicit operator	::Eldritch2::Quaternion() const;

	// ---------------------------------------------------

		static ETNoAliasHint ::Eldritch2::Float4x4	Perspective( const ::Eldritch2::RadianMeasure horizontalFieldOfView, const ::Eldritch2::float32 aspect, const ::Eldritch2::float32 near, const ::Eldritch2::float32 far );
		static ETNoAliasHint ::Eldritch2::Float4x4	Perspective( const ::Eldritch2::RadianMeasure horizontalFieldOfView, const ::Eldritch2::RadianMeasure verticalFieldOfView, const ::Eldritch2::float32 near, const ::Eldritch2::float32 far );

		static ETNoAliasHint ::Eldritch2::Float4x4	Rotation( const ::Eldritch2::Quaternion orientation );

		static ETNoAliasHint ::Eldritch2::Float4x4	RigidTransformation( const ::Eldritch2::Float4 translation, const ::Eldritch2::Quaternion orientation );

		static ETNoAliasHint ::Eldritch2::Float4x4	Identity();

		static ETNoAliasHint ::Eldritch2::Float4x4	Zero();

	// ---------------------------------------------------

		::Eldritch2::Float4	elements[4];
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/MPL/MatrixTypes.inl>
//------------------------------------------------------------------//