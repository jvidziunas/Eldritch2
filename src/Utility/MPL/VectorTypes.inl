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
#include <intrin.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ETForceInlineHint Float4::Float4( const ::Eldritch2::float32 x, const ::Eldritch2::float32 y, const ::Eldritch2::float32 z, const ::Eldritch2::float32 w ) {
		::_mm_storeu_ps( this->coefficients, ::_mm_set_ps( z, y, x, w ) );
	}

// ---------------------------------------------------
	
	ETForceInlineHint Float4::Float4( const ::Eldritch2::float32* const coefficients ) {
		::_mm_storeu_ps( this->coefficients, ::_mm_loadu_ps( coefficients ) );
	}

// ---------------------------------------------------

	template <::Eldritch2::Float4::VectorSwizzleComponent component0, ::Eldritch2::Float4::VectorSwizzleComponent component1, ::Eldritch2::Float4::VectorSwizzleComponent component2, ::Eldritch2::Float4::VectorSwizzleComponent component3>
	ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4& Float4::Swizzle() {
		register const __m128	coeffs( ::_mm_loadu_ps( this->coefficients ) );

		::_mm_storeu_ps( this->coefficients, ::_mm_shuffle_ps( coeffs, coeffs, static_cast<unsigned int>(_MM_SHUFFLE( component2, component1, component0, component3 )) ) );

		return *this;
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	Float4::operator +( const ::Eldritch2::Float4 operand ) const {
		return ::Eldritch2::Float4( ::_mm_add_ps( ::_mm_loadu_ps( this->coefficients ), ::_mm_loadu_ps( operand ) ).m128_f32 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	Float4::operator -( const ::Eldritch2::Float4 operand ) const {
		return ::Eldritch2::Float4( ::_mm_sub_ps( ::_mm_loadu_ps( this->coefficients ), ::_mm_loadu_ps( operand ) ).m128_f32 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	Float4::operator *( const ::Eldritch2::float32 scalar ) const {
		return ::Eldritch2::Float4( ::_mm_mul_ps( ::_mm_loadu_ps( this->coefficients ), ::_mm_set_ps1( scalar ) ).m128_f32 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint::Eldritch2::Float4	Float4::operator /( const ::Eldritch2::float32 scalar ) const {
		return ::Eldritch2::Float4( ::_mm_div_ps( ::_mm_loadu_ps( this->coefficients ), ::_mm_set_ps1( scalar ) ).m128_f32 );
	}

// ---------------------------------------------------

	ETForceInlineHint ::Eldritch2::Float4& Float4::operator +=( const ::Eldritch2::Float4 operand ) {
		::_mm_storeu_ps( this->coefficients, ::_mm_add_ps( ::_mm_loadu_ps( this->coefficients ), ::_mm_loadu_ps( operand ) ) );

		return *this;
	}

// ---------------------------------------------------

	ETForceInlineHint ::Eldritch2::Float4& Float4::operator -=( const ::Eldritch2::Float4 operand ) {
		::_mm_storeu_ps( this->coefficients, ::_mm_sub_ps( ::_mm_loadu_ps( this->coefficients ), ::_mm_loadu_ps( operand ) ) );

		return *this;
	}

// ---------------------------------------------------

	ETForceInlineHint ::Eldritch2::Float4& Float4::operator *=( const ::Eldritch2::float32 scalar ) {
		::_mm_storeu_ps( this->coefficients, ::_mm_mul_ps( ::_mm_loadu_ps( this->coefficients ), ::_mm_set_ps1( scalar ) ) );

		return *this;
	}

// ---------------------------------------------------

	ETForceInlineHint ::Eldritch2::Float4& Float4::operator /=( const ::Eldritch2::float32 scalar ) {
		::_mm_storeu_ps( this->coefficients, ::_mm_div_ps( ::_mm_loadu_ps( this->coefficients ), ::_mm_set_ps1( scalar ) ) );

		return *this;
	}

// ---------------------------------------------------

	ETForceInlineHint ::Eldritch2::Float4& Float4::operator =( const ::Eldritch2::Float4 operand ) {
		::_mm_storeu_ps( this->coefficients, ::_mm_loadu_ps( operand ) );

		return *this;
	}

// ---------------------------------------------------

	ETForceInlineHint ::Eldritch2::Float4& Float4::Normalize() {
		const register __m128	coeffs( ::_mm_loadu_ps( this->coefficients ) );

		::_mm_storeu_ps( this->coefficients, ::_mm_mul_ps( ::_mm_rsqrt_ps( ::_mm_dp_ps( coeffs, coeffs, 0xFF ) ), coeffs ) );

		return *this;
	}

// ---------------------------------------------------

	ETForceInlineHint Float4::operator const Float4::CoefficientSet&() const {
		return coefficients;
	}

// ---------------------------------------------------

	ETForceInlineHint Float4::operator Float4::CoefficientSet&() {
		return coefficients;
	}

// ---------------------------------------------------

	ETForceInlineHint Quaternion::Quaternion( const ::Eldritch2::float32 x, const ::Eldritch2::float32 y, const ::Eldritch2::float32 z, const ::Eldritch2::float32 w ) {
		::_mm_storeu_ps( this->coefficients, ::_mm_set_ps( z, y, x, w ) );
	}

// ---------------------------------------------------

	ETForceInlineHint Quaternion::Quaternion( const ::Eldritch2::float32* const coefficients ) {
		::_mm_storeu_ps( this->coefficients, ::_mm_loadu_ps( coefficients ) );
	}

// ---------------------------------------------------

	ETForceInlineHint ::Eldritch2::Quaternion& Quaternion::Normalize() {
		const register __m128	coeffs( ::_mm_loadu_ps( this->coefficients ) );

		::_mm_storeu_ps( this->coefficients, ::_mm_mul_ps( ::_mm_rsqrt_ps( ::_mm_dp_ps( coeffs, coeffs, 0xFF ) ), coeffs ) );

		return *this;
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint ::Eldritch2::Quaternion Quaternion::Reverse() const {
		return { -coefficients[0], -coefficients[1], -coefficients[2], coefficients[3] };
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4 Quaternion::RotateVector( const ::Eldritch2::Float4 point ) const {
		const ::Eldritch2::Float4	temp( ::Eldritch2::CrossProduct( ::Eldritch2::Float4( this->coefficients ), point ) * 2.0f );

		return ::Eldritch2::Float4( point + (temp * coefficients[3]) + ::Eldritch2::CrossProduct( ::Eldritch2::Float4( this->coefficients ), temp ) );
	}

// ---------------------------------------------------

	ETForceInlineHint Quaternion::operator const Quaternion::CoefficientSet&() const {
		return coefficients;
	}

// ---------------------------------------------------

	ETForceInlineHint Quaternion::operator Quaternion::CoefficientSet&() {
		return coefficients;
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::float32 DotProduct( const ::Eldritch2::Float4 vector0, const ::Eldritch2::Float4 vector1 ) {
		return ::_mm_cvtss_f32( ::_mm_dp_ps( ::_mm_loadu_ps( vector0 ), ::_mm_loadu_ps( vector1 ), 0xFF ) );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::Float4 CrossProduct( const ::Eldritch2::Float4 vector0, const ::Eldritch2::Float4 vector1 ) {
		register const __m128	shuffle0( ::_mm_shuffle_ps( ::_mm_loadu_ps( vector0 ), ::_mm_loadu_ps( vector0 ), _MM_SHUFFLE( 3, 0, 2, 1 ) ) );
		register const __m128	shuffle1( ::_mm_shuffle_ps( ::_mm_loadu_ps( vector1 ), ::_mm_loadu_ps( vector1 ), _MM_SHUFFLE( 3, 0, 2, 1 ) ) );
		register const __m128	result( ::_mm_sub_ps( ::_mm_mul_ps( ::_mm_loadu_ps( vector1 ), shuffle0 ), ::_mm_mul_ps( ::_mm_loadu_ps( vector0 ), shuffle1 ) ) );

		return Float4( ::_mm_shuffle_ps( result, result, _MM_SHUFFLE( 3, 0, 2, 1 ) ).m128_f32 );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::Float4 LinearInterpolate( const ::Eldritch2::Float4 vector0, const ::Eldritch2::Float4 vector1, const ::Eldritch2::float32 alpha ) {
		return Float4( ::_mm_add_ps( ::_mm_loadu_ps( vector0 ), ::_mm_mul_ps( ::_mm_sub_ps( ::_mm_loadu_ps( vector1 ), ::_mm_loadu_ps( vector0 ) ), ::_mm_set_ps1( alpha ) ) ).m128_f32 );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::float32 DotProduct( const ::Eldritch2::Quaternion quaternion0, const ::Eldritch2::Quaternion quaternion1 ) {
		return ::_mm_cvtss_f32( ::_mm_dp_ps( ::_mm_loadu_ps( quaternion0 ), ::_mm_loadu_ps( quaternion1 ), 0xFF ) );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::Quaternion LinearInterpolate( const ::Eldritch2::Quaternion quaternion0, const ::Eldritch2::Quaternion quaternion1, const ::Eldritch2::float32 alpha ) {
		struct AlphaCorrectionWrapper {
			// Adapted from the original by Jonathan Blow, see http://number-none.com/product/Hacking%20Quaternions/index.html
			// In particular, the alpha parameter has been changed to take the cosine thereof to exploit the quaternion dot product
			// along with factoring out a few terms to be global constants (see above) instead of parameters.
			static ETForceInlineHint ETNoAliasHint ::Eldritch2::float32 Adjust( const ::Eldritch2::float32 t, const ::Eldritch2::float32 cosAlpha ) {
				// RMS minimized coefficients for the quaternion alpha spline adjustment.
				// Taken from http://number-none.com/product/Hacking%20Quaternions/index.html
				static const ::Eldritch2::float64	alphaAttenuation( 0.7878088 );
				static const ::Eldritch2::float64	alphaK( 0.5069269 );

				const ::Eldritch2::float64	factor( 1.0 - (alphaAttenuation * cosAlpha) );
				const ::Eldritch2::float32	k( static_cast<::Eldritch2::float32>(alphaK * factor * factor) );
				const ::Eldritch2::float32	b(  2.0f * k );
				const ::Eldritch2::float32	c( -3.0f * k );
				const ::Eldritch2::float32	d(  1.0f + k );

				return t * (b * t + c) + d;
			}
		};

	// ---

		return ::Eldritch2::LinearInterpolateNonConstantVelocity( quaternion0, quaternion1, AlphaCorrectionWrapper::Adjust( alpha, ::Eldritch2::DotProduct( quaternion0, quaternion1 ) ) );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::Quaternion LinearInterpolateNonConstantVelocity( const ::Eldritch2::Quaternion quaternion0, const ::Eldritch2::Quaternion quaternion1, const ::Eldritch2::float32 alpha ) {
		const register __m128	coeffs1( ::_mm_loadu_ps( quaternion1 ) );
		register __m128			coeffs0( ::_mm_loadu_ps( quaternion0 ) );

		// Determine whether or not one of the quaternions must be negated in order for shortest-path interpolation.
		// Mask bits will be set to all ones if the dot product is less than 0.
		const register __m128	mask( ::_mm_cmplt_ps( ::_mm_dp_ps( coeffs0, coeffs1, 0xFF ), ::_mm_set1_ps( 0.0f ) ) );

		// Negate the initial quaternion if necessary. This is done in a branchless way with masking.
		coeffs0 = ::_mm_or_ps( ::_mm_and_ps( mask, ::_mm_mul_ps( coeffs0, ::_mm_set1_ps( -1.0f ) ) ), ::_mm_andnot_ps( mask, coeffs0 ) );

		// Perform a conventional linear interpolation on the coefficients of the quaternion...
		register const __m128	interpolatedQuaternion( ::_mm_add_ps( coeffs0, ::_mm_mul_ps( ::_mm_sub_ps( coeffs1, coeffs0 ), ::_mm_set_ps1( alpha ) ) ) );
		// ... then normalize the result to remove any undesired scaling.
		return ::Eldritch2::Quaternion( ::_mm_mul_ps( ::_mm_rsqrt_ps( ::_mm_dp_ps( interpolatedQuaternion, interpolatedQuaternion, 0xFF ) ), interpolatedQuaternion ).m128_f32 );
	}

}	// namespace Eldritch2