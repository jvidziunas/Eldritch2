/*==================================================================*\
 VectorTypes.inl
 ------------------------------------------------------------------
 Purpose:


 ------------------------------------------------------------------
 ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Mpl/MatrixTypes.hpp>
//------------------------------------------------------------------//
#include <intrin.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ETInlineHint Float4::Float4( const Eldritch2::float32 x, const Eldritch2::float32 y, const Eldritch2::float32 z, const Eldritch2::float32 w ) {
		_mm_storeu_ps( coefficients, _mm_set_ps( z, y, x, w ) );
	}

// ---------------------------------------------------
	
	ETInlineHint Float4::Float4( const Eldritch2::float32* const coefficients ) {
		_mm_storeu_ps( this->coefficients, _mm_loadu_ps( coefficients ) );
	}

// ---------------------------------------------------

	template <Eldritch2::Float4::VectorSwizzleComponent component0, Eldritch2::Float4::VectorSwizzleComponent component1, Eldritch2::Float4::VectorSwizzleComponent component2, Eldritch2::Float4::VectorSwizzleComponent component3>
	ETInlineHint Eldritch2::Float4& Float4::Swizzle() {
		register const __m128	coeffs( _mm_loadu_ps( coefficients ) );

		_mm_storeu_ps( coefficients, _mm_shuffle_ps( coeffs, coeffs, static_cast<unsigned int>(_MM_SHUFFLE( component2, component1, component0, component3 )) ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::float32 Float4::operator[]( size_t index ) const {
		return coefficients[index];
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 operator+( Eldritch2::Float4 operand0, Eldritch2::Float4 operand1 ) {
		return operand0 += operand1;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 operator-( Eldritch2::Float4 operand0, Eldritch2::Float4 operand1 ) {
		return operand0 -= operand1;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 operator*( Eldritch2::Float4 operand0, Eldritch2::Float4 operand1 ) {
		return operand0 *= operand1;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 operator*( Eldritch2::Float4 operand, Eldritch2::float32 scalar ) {
		return operand *= scalar;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 operator/( Eldritch2::Float4 operand0, Eldritch2::Float4 operand1 ) {
		return operand0 *= operand1;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 operator/( Eldritch2::Float4 operand, Eldritch2::float32 scalar ) {
		return operand /= scalar;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 operator-( Eldritch2::Float4 vector ) {
		return { -vector.coefficients[0], -vector.coefficients[1], -vector.coefficients[2], -vector.coefficients[3] };
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Float4& Float4::operator+=( Eldritch2::Float4 operand ) {
		_mm_storeu_ps( coefficients, _mm_add_ps( _mm_loadu_ps( coefficients ), _mm_loadu_ps( operand.coefficients ) ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Float4& Float4::operator-=( Eldritch2::Float4 operand ) {
		_mm_storeu_ps( coefficients, _mm_sub_ps( _mm_loadu_ps( coefficients ), _mm_loadu_ps( operand.coefficients ) ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Float4& Float4::operator*=( Eldritch2::Float4 operand ) {
		_mm_storeu_ps( coefficients, _mm_mul_ps( _mm_loadu_ps( coefficients ), _mm_loadu_ps( operand.coefficients ) ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Float4& Float4::operator*=( Eldritch2::float32 scalar ) {
		_mm_storeu_ps( coefficients, _mm_mul_ps( _mm_loadu_ps( coefficients ), _mm_set_ps1( scalar ) ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Float4& Float4::operator/=( Eldritch2::Float4 operand ) {
		_mm_storeu_ps( coefficients, _mm_div_ps( _mm_loadu_ps( coefficients ), _mm_loadu_ps( operand.coefficients ) ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Float4& Float4::operator/=( Eldritch2::float32 scalar ) {
		_mm_storeu_ps( coefficients, _mm_div_ps( _mm_loadu_ps( coefficients ), _mm_set_ps1( scalar ) ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Float4& Float4::operator=( Eldritch2::Float4 operand ) {
		_mm_storeu_ps( coefficients, _mm_loadu_ps( operand.coefficients ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Float4& Float4::Normalize() {
		const register __m128	coeffs( _mm_loadu_ps( this->coefficients ) );

		_mm_storeu_ps( this->coefficients, _mm_mul_ps( _mm_rsqrt_ps( _mm_dp_ps( coeffs, coeffs, 0xFF ) ), coeffs ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 ETSimdCall Ceil( Eldritch2::Float4 vector ) {
		return { _mm_ceil_ps( _mm_loadu_ps( vector.coefficients ) ).m128_f32 };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 ETSimdCall Floor( Eldritch2::Float4 vector ) {
		return { _mm_floor_ps( _mm_loadu_ps( vector.coefficients ) ).m128_f32 };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::float32 ETSimdCall DotProduct( Eldritch2::Float4 vector0, Eldritch2::Float4 vector1 ) {
		return _mm_cvtss_f32( _mm_dp_ps( _mm_loadu_ps( vector0.coefficients ), _mm_loadu_ps( vector1.coefficients ), 0xFF ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 ETSimdCall CrossProduct( Eldritch2::Float4 vector0, Eldritch2::Float4 vector1 ) {
		register const __m128	shuffle0( _mm_shuffle_ps( _mm_loadu_ps( vector0.coefficients ), _mm_loadu_ps( vector0.coefficients ), _MM_SHUFFLE( 3, 0, 2, 1 ) ) );
		register const __m128	shuffle1( _mm_shuffle_ps( _mm_loadu_ps( vector1.coefficients ), _mm_loadu_ps( vector1.coefficients ), _MM_SHUFFLE( 3, 0, 2, 1 ) ) );
		register const __m128	result( _mm_sub_ps( _mm_mul_ps( _mm_loadu_ps( vector1.coefficients ), shuffle0 ), _mm_mul_ps( _mm_loadu_ps( vector0.coefficients ), shuffle1 ) ) );

		return { _mm_shuffle_ps( result, result, _MM_SHUFFLE( 3, 0, 2, 1 ) ).m128_f32 };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 ETSimdCall LinearInterpolate( Eldritch2::Float4 vector0, Eldritch2::Float4 vector1, Eldritch2::float32 alpha ) {
		return { _mm_add_ps( _mm_loadu_ps( vector0.coefficients ), _mm_mul_ps( _mm_sub_ps( _mm_loadu_ps( vector1.coefficients ), _mm_loadu_ps( vector0.coefficients ) ), _mm_set_ps1( alpha ) ) ).m128_f32 };
	}

// ---------------------------------------------------

	ETInlineHint Quaternion::Quaternion( Eldritch2::float32 x, Eldritch2::float32 y, Eldritch2::float32 z, Eldritch2::float32 w ) {
		_mm_storeu_ps( this->coefficients, _mm_set_ps( z, y, x, w ) );
	}

// ---------------------------------------------------

	ETInlineHint Quaternion::Quaternion( const Eldritch2::float32* const coefficients ) {
		_mm_storeu_ps( this->coefficients, _mm_loadu_ps( coefficients ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Quaternion operator-( Eldritch2::Quaternion quaternion ) {
		return { quaternion.coefficients[0], quaternion.coefficients[1], quaternion.coefficients[2], -quaternion.coefficients[3] };
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::float32 Quaternion::operator[]( size_t index ) const {
		return coefficients[index];
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Quaternion& Quaternion::Normalize() {
		const register __m128	coeffs( _mm_loadu_ps( this->coefficients ) );

		_mm_storeu_ps( this->coefficients, _mm_mul_ps( _mm_rsqrt_ps( _mm_dp_ps( coeffs, coeffs, 0xFF ) ), coeffs ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Quaternion Quaternion::GetReverse() const {
		return { -coefficients[0], -coefficients[1], -coefficients[2], coefficients[3] };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Float4 Quaternion::RotateVector( Eldritch2::Float4 point ) const {
		const register Eldritch2::Float4	temp( Eldritch2::CrossProduct( Eldritch2::Float4( this->coefficients ), point ) * 2.0f );

		return { point + (temp * coefficients[3]) + Eldritch2::CrossProduct( Eldritch2::Float4( this->coefficients ), temp ) };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::float32 ETSimdCall DotProduct( Eldritch2::Quaternion quaternion0, Eldritch2::Quaternion quaternion1 ) {
		return _mm_cvtss_f32( _mm_dp_ps( _mm_loadu_ps( quaternion0.coefficients ), _mm_loadu_ps( quaternion1.coefficients ), 0xFF ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Quaternion ETSimdCall LinearInterpolate( Eldritch2::Quaternion quaternion0, Eldritch2::Quaternion quaternion1, Eldritch2::float32 alpha ) {
	//	Adapted from the original by Jonathan Blow, see http://number-none.com/product/Hacking%20Quaternions/index.html
	//	In particular, the alpha parameter has been changed to take the cosine thereof to exploit the quaternion dot product
	//	along with factoring out a few terms to be global constants (see above) instead of parameters.
		auto adjustAlpha( [] ( const Eldritch2::float32 t, const Eldritch2::float32 cosAlpha ) {
		//	RMS minimized coefficients for the quaternion alpha spline adjustment.
		//	Taken from http://number-none.com/product/Hacking%20Quaternions/index.html
			static const Eldritch2::float64	alphaAttenuation( 0.7878088 );
			static const Eldritch2::float64	alphaK( 0.5069269 );

			const Eldritch2::float64		factor( 1.0 - (alphaAttenuation * cosAlpha) );
			const Eldritch2::float32		k( static_cast<Eldritch2::float32>(alphaK * factor * factor) );
			const Eldritch2::float32		b( 2.0f * k );
			const Eldritch2::float32		c( -3.0f * k );
			const Eldritch2::float32		d( 1.0f + k );

			return t * (b * t + c) + d;
		} );

	// ---

		return Eldritch2::LinearInterpolateNonConstantVelocity( quaternion0, quaternion1, adjustAlpha( alpha, Eldritch2::DotProduct( quaternion0, quaternion1 ) ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Eldritch2::Quaternion ETSimdCall LinearInterpolateNonConstantVelocity( Eldritch2::Quaternion quaternion0, Eldritch2::Quaternion quaternion1, Eldritch2::float32 alpha ) {
		const register __m128	coeffs1( _mm_loadu_ps( quaternion1.coefficients ) );
		register __m128			coeffs0( _mm_loadu_ps( quaternion0.coefficients ) );

	//	Determine whether or not one of the quaternions must be negated in order for shortest-path interpolation.
	//	Mask bits will be set to all ones if the dot product is less than 0.
		const register __m128	mask( _mm_cmplt_ps( _mm_dp_ps( coeffs0, coeffs1, 0xFF ), _mm_set1_ps( 0.0f ) ) );

	//	Negate the initial quaternion if necessary. This is done in a branchless way with masking.
		coeffs0 = _mm_or_ps( _mm_and_ps( mask, _mm_mul_ps( coeffs0, _mm_set1_ps( -1.0f ) ) ), _mm_andnot_ps( mask, coeffs0 ) );

	//	Perform a conventional linear interpolation on the coefficients of the quaternion...
		register const __m128	interpolatedQuaternion( _mm_add_ps( coeffs0, _mm_mul_ps( _mm_sub_ps( coeffs1, coeffs0 ), _mm_set_ps1( alpha ) ) ) );

	//	... then normalize the result to remove any undesired scaling.
		return { _mm_mul_ps( _mm_rsqrt_ps( _mm_dp_ps( interpolatedQuaternion, interpolatedQuaternion, 0xFF ) ), interpolatedQuaternion ).m128_f32 };
	}

// ---------------------------------------------------

	ETInlineHint RigidTransform::RigidTransform( Eldritch2::Float4 translation, Eldritch2::Quaternion orientation ) : translation( translation ), orientation( orientation ) {}

// ---------------------------------------------------

	ETInlineHint RigidTransform::RigidTransform( Eldritch2::Float4 translation ) : RigidTransform( translation, { 1.0f, 0.0f, 0.0f, 0.0f } ) {}

// ---------------------------------------------------

	ETInlineHint RigidTransform::RigidTransform( Eldritch2::Quaternion orientation ) : RigidTransform( { 0.0f, 0.0f, 0.0f, 0.0f }, orientation ) {}

}	// namespace Eldritch2