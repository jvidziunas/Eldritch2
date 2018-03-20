/*==================================================================*\
  Vectors.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	ETInlineHint ETPureFunctionHint __m128 ETSimdCall _mm_xpd_ps( __m128 xmm0, __m128 xmm1 ) {
		register __m128 xmm2 = _mm_mul_ps( xmm1, _mm_shuffle_ps( xmm0, xmm0, _MM_SHUFFLE( 3, 0, 2, 1 ) ) );
		register __m128 xmm3 = _mm_mul_ps( xmm0, _mm_shuffle_ps( xmm1, xmm1, _MM_SHUFFLE( 3, 0, 2, 1 ) ) );
		xmm0 = _mm_sub_ps( xmm2, xmm3 );

		return _mm_shuffle_ps( xmm0, xmm0, _MM_SHUFFLE( 3, 0, 2, 1 ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint __m128 ETSimdCall _mm_vec2quat_ps( __m128 xmm0, __m128 xmm1 ) {
		register __m128	xmm2, xmm3;
		
		xmm2 = _mm_xpd_ps( xmm0, xmm1 );
		xmm0 = _mm_xor_ps( _mm_set_ps( +0.0f, +0.0f, -0.0f, -0.0f ), _mm_shuffle_ps( xmm0, xmm0, _MM_SHUFFLE( 3, 3, 3, 3 ) ) );
		xmm1 = _mm_xor_ps( _mm_set_ps( +0.0f, -0.0f, +0.0f, -0.0f ), _mm_shuffle_ps( xmm1, xmm1, _MM_SHUFFLE( 2, 2, 2, 2 ) ) );
		xmm2 = _mm_xor_ps( _mm_set_ps( +0.0f, -0.0f, -0.0f, +0.0f ), _mm_shuffle_ps( xmm2, xmm2, _MM_SHUFFLE( 1, 1, 1, 1 ) ) );

		xmm3 = _mm_add_ps( _mm_set_ps1( 1.0f ), xmm0 );
		xmm3 = _mm_add_ps( _mm_add_ps( xmm1, xmm2 ), xmm3 );

		return _mm_mul_ps( _mm_sqrt_ps( _mm_max_ps( _mm_setzero_ps(), xmm3 ) ), _mm_set_ps1( 0.5f ) );
	}

// ---------------------------------------------------

//	Uses ~7 XMM registers.
	ETInlineHint ETPureFunctionHint __m128 ETSimdCall _mm_mulquat_ps( __m128 xmm0, __m128 xmm1 ) {
	/*	i = w0i1 + i0w1 + j0k1 - k0j1
		j = w0j1 + j0w1 + k0i1 - i0k1
		k = w0k1 + k0w1 + i0j1 - j0i1
		w = w0w1 - i0i1 - j0j1 - k0k1
	*/
	//	XOR an XMM register with this value to flip the IEEE754 sign bit in the W position.
		register const __m128	flipSign = _mm_set_ps( +0.0f, +0.0f, +0.0f, -0.0f );
		register __m128			xmm2, xmm3, xmm4, xmm5;

		xmm2 = _mm_mul_ps( _mm_shuffle_ps( xmm0, xmm0, _MM_SHUFFLE( 3, 3, 3, 3 ) ), xmm1 /*Identity shuffle*/ );
		xmm3 = _mm_mul_ps( _mm_shuffle_ps( xmm0, xmm0, _MM_SHUFFLE( 3, 2, 1, 3 ) ), _mm_shuffle_ps( xmm1, xmm1, _MM_SHUFFLE( 0, 0, 0, 3 ) ) );
		xmm4 = _mm_mul_ps( _mm_shuffle_ps( xmm0, xmm0, _MM_SHUFFLE( 2, 1, 3, 2 ) ), _mm_shuffle_ps( xmm1, xmm1, _MM_SHUFFLE( 1, 3, 2, 2 ) ) );
		xmm5 = _mm_mul_ps( _mm_shuffle_ps( xmm0, xmm0, _MM_SHUFFLE( 1, 3, 2, 1 ) ), _mm_shuffle_ps( xmm1, xmm1, _MM_SHUFFLE( 2, 1, 3, 1 ) ) );
		xmm2 = _mm_add_ps( xmm2, _mm_xor_ps( xmm3, flipSign ) );
		xmm4 = _mm_sub_ps( _mm_xor_ps( xmm4, flipSign ), xmm5 );

		return _mm_add_ps( xmm2, xmm4 );
	}

}	// anonymous namespace

	ETInlineHint Vector::Vector( float32 x, float32 y, float32 z, float32 w ) : coefficients( _mm_set_ps( x, y, z, w ) ) {}

// ---------------------------------------------------
	
	ETInlineHint Vector::Vector( const float32* coefficients ) : coefficients( _mm_loadu_ps( coefficients ) ) {}

// ---------------------------------------------------

	ETInlineHint Vector::Vector( const __m128& coefficients ) : coefficients( coefficients ) {}

// ---------------------------------------------------

	template <Vector::Component component0, Vector::Component component1, Vector::Component component2, Vector::Component component3>
	ETInlineHint Vector& Vector::Swizzle() {
		coefficients = _mm_shuffle_ps( coefficients, coefficients, static_cast<unsigned int>(_MM_SHUFFLE( component3, component2, component1, component0 )) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall operator+( Vector operand0, Vector operand1 ) {
		return _mm_add_ps( operand0, operand1 );;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall operator-( Vector operand0, Vector operand1 ) {
		return _mm_sub_ps( operand0, operand1 );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall operator*( Vector operand0, Vector operand1 ) {
		return _mm_mul_ps( operand0, operand1 );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall operator*( Vector operand, float32 scalar ) {
		return _mm_mul_ps( operand, _mm_set_ps1( scalar ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall operator/( Vector operand0, Vector operand1 ) {
		return _mm_div_ps( operand0, operand1 );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall operator/( Vector operand, float32 scalar ) {
		return _mm_div_ps( operand, _mm_set_ps1( scalar ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall operator-( Vector vector ) {
		return _mm_sub_ps( _mm_setzero_ps(), vector );
	}

// ---------------------------------------------------

	ETInlineHint Vector& Vector::operator+=( Vector operand ) {
		coefficients = _mm_add_ps( coefficients, operand );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Vector& Vector::operator-=( Vector operand ) {
		coefficients = _mm_sub_ps( coefficients, operand.coefficients );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Vector& Vector::operator*=( Vector operand ) {
		coefficients = _mm_mul_ps( coefficients, operand );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Vector& Vector::operator*=( float32 scalar ) {
		coefficients = _mm_mul_ps( coefficients, _mm_set_ps1( scalar ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Vector& Vector::operator/=( Vector operand ) {
		coefficients = _mm_div_ps( coefficients, operand );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Vector& Vector::operator/=( float32 scalar ) {
		coefficients = _mm_div_ps( coefficients, _mm_set_ps1( scalar ) );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Vector& Vector::Normalize() {
		coefficients = _mm_mul_ps( _mm_rsqrt_ps( _mm_dp_ps( coefficients, coefficients, 0xFF ) ), coefficients );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint void Vector::ExtractCoefficients( float32 (&output)[4] ) const {
		_mm_storeu_ps( output, coefficients );
	}

// ---------------------------------------------------

	ETForceInlineHint Vector::operator const __m128&() const {
		return coefficients;
	}

// ---------------------------------------------------

	ETForceInlineHint Vector::operator __m128&() {
		return coefficients;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall AbsoluteValue( Vector vector ) {
		return _mm_andnot_ps( _mm_set_ps1( -0.0f ), vector );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall Ceil( Vector vector ) {
		return _mm_ceil_ps( vector );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall Floor( Vector vector ) {
		return _mm_floor_ps( vector );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall CrossProduct( Vector vector0, Vector vector1 ) {
		return _mm_xpd_ps( vector0, vector1 );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint float32 ETSimdCall DotProduct( Vector vector0, Vector vector1 ) {
		return _mm_cvtss_f32( _mm_dp_ps( vector0, vector1, 0xFF ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint float32 ETSimdCall SquaredLength( Vector vector ) {
		return _mm_cvtss_f32( _mm_dp_ps( vector, vector, 0xFF ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint float32 ETSimdCall Length( Vector vector ) {
		return _mm_cvtss_f32( _mm_sqrt_ps( _mm_dp_ps( vector, vector, 0xFF ) ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall LinearInterpolate( Vector vector0, Vector vector1, float32 alpha ) {
		return _mm_add_ps( vector0, _mm_mul_ps( _mm_sub_ps( vector1, vector0 ), _mm_set_ps1( alpha ) ) );
	}

// ---------------------------------------------------

	ETInlineHint Quaternion::Quaternion( float32 i, float32 j, float32 k, float32 w ) : coefficients( _mm_set_ps( i, j, k, w ) ) {}

// ---------------------------------------------------

	ETInlineHint Quaternion::Quaternion( const float32* coefficients ) : coefficients( _mm_loadu_ps( coefficients ) ) {}

// ---------------------------------------------------

	ETInlineHint Quaternion::Quaternion( const __m128& coefficients ) : coefficients( coefficients ) {}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Quaternion ETSimdCall operator-( Quaternion quaternion ) {
		return _mm_mul_ps( quaternion, _mm_set_ps( 1.0f, 1.0f, 1.0f, -1.0f ) );
	}

// ---------------------------------------------------

	ETInlineHint Quaternion& Quaternion::Normalize() {
		coefficients = _mm_mul_ps( _mm_rsqrt_ps( _mm_dp_ps( coefficients, coefficients, 0xFF ) ), coefficients );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Quaternion& ETSimdCall Quaternion::operator*=( Quaternion quaternion ) {
		coefficients = _mm_mulquat_ps( coefficients, quaternion );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Quaternion ETSimdCall Quaternion::GetReverse() const {
		return _mm_mul_ps( coefficients, _mm_set_ps( -1.0f, -1.0f, -1.0f, 1.0f ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall Quaternion::Rotate( Vector point ) const {
		register __m128 xmm( _mm_mul_ps( _mm_xpd_ps( coefficients, point ), _mm_set_ps1( 2.0f ) ) );
		register __m128	w( _mm_shuffle_ps( coefficients, coefficients, _MM_SHUFFLE( 3, 3, 3, 3 ) ) );

		return _mm_add_ps( _mm_add_ps( point, _mm_mul_ps( w, xmm ) ), _mm_xpd_ps( coefficients, xmm ) );
	}

// ---------------------------------------------------

	ETInlineHint Vector ETSimdCall Quaternion::GetForward() const {
		return Rotate( { 0.0f, 0.0f, 1.0f, 0.0f } );
	}

// ---------------------------------------------------

	ETInlineHint Vector ETSimdCall Quaternion::GetRight() const {
		return Rotate( { 1.0f, 0.0f, 0.0f, 0.0f } );
	}

// ---------------------------------------------------

	ETInlineHint Vector ETSimdCall Quaternion::GetUp() const {
		return Rotate( { 0.0f, 1.0f, 0.0f, 0.0f } );
	}

// ---------------------------------------------------

	ETInlineHint void Quaternion::ExtractCoefficients( float32 (&output)[4] ) const {
		_mm_storeu_ps( output, coefficients );
	}

// ---------------------------------------------------

	ETForceInlineHint Quaternion::operator const __m128&() const {
		return coefficients;
	}

// ---------------------------------------------------

	ETForceInlineHint Quaternion::operator __m128&() {
		return coefficients;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Quaternion ETSimdCall operator*( Quaternion quaternion0, Quaternion quaternion1 ) {
		return _mm_mulquat_ps( quaternion0, quaternion1 );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint float32 ETSimdCall DotProduct( Quaternion quaternion0, Quaternion quaternion1 ) {
		return _mm_cvtss_f32( _mm_dp_ps( quaternion0, quaternion1, 0xFF ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Quaternion ETSimdCall LinearInterpolate( Quaternion quaternion0, Quaternion quaternion1, float32 alpha ) {
	//	RMS minimized coefficients for the quaternion alpha spline adjustment, taken from http://number-none.com/product/Hacking%20Quaternions/index.html
		static constexpr float64	alphaAttenuation( 0.7878088 );
		static constexpr float64	alphaK( 0.5069269 );
	/*	Adapted from the original by Jonathan Blow, see http://number-none.com/product/Hacking%20Quaternions/index.html
	 *	In particular, the alpha parameter has been changed to take the cosine thereof to exploit the quaternion dot product
	 *	along with factoring out a few terms to be global constants (see above) instead of parameters. */
		auto adjustAlpha( [] ( float32 t, float32 cosAlpha ) {
			const float64	factor( 1.0 - (alphaAttenuation * cosAlpha) );
			const float32	k( static_cast<float32>(alphaK * factor * factor) );

			return t * ( (2.0f * k) * t + (-3.0f * k) ) + (1.0f + k);
		} );

	// ---

		return LinearInterpolateNonConstantVelocity( quaternion0, quaternion1, adjustAlpha( alpha, DotProduct( quaternion0, quaternion1 ) ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Quaternion ETSimdCall AsBasis( Vector forward, Vector up ) {
		return _mm_vec2quat_ps( forward, up );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Quaternion ETSimdCall LinearInterpolateNonConstantVelocity( Quaternion quaternion0, Quaternion quaternion1, float32 alpha ) {
	//	Determine whether or not one of the quaternions must be negated in order for shortest-path interpolation.
	//	Mask bits will be set to all ones if the dot product is less than 0.
		const __m128	mask( _mm_cmplt_ps( _mm_dp_ps( quaternion0, quaternion1, 0xFF ), _mm_setzero_ps() ) );

	//	Negate the initial quaternion if necessary. This is done in a branchless way with masking.
		quaternion0.coefficients = _mm_or_ps( _mm_and_ps( mask, _mm_mul_ps( quaternion0, _mm_set_ps1( -1.0f ) ) ), _mm_andnot_ps( mask, quaternion0 ) );

	//	Perform a conventional linear interpolation on the coefficients of the quaternion...
		const __m128	interpolatedQuaternion( _mm_add_ps( quaternion0, _mm_mul_ps( _mm_sub_ps( quaternion1, quaternion0 ), _mm_set_ps1( alpha ) ) ) );

	//	... then normalize the result to remove any undesired scaling.
		return _mm_mul_ps( _mm_rsqrt_ps( _mm_dp_ps( interpolatedQuaternion, interpolatedQuaternion, 0xFF ) ), interpolatedQuaternion );
	}

// ---------------------------------------------------

	ETInlineHint Transformation::Transformation( Vector translation, Quaternion rotation ) : translation( translation ), rotation( rotation ) {}

// ---------------------------------------------------

	ETInlineHint Transformation::Transformation( Vector translation ) : Transformation( translation, { 1.0f, 0.0f, 0.0f, 0.0f } ) {}

// ---------------------------------------------------

	ETInlineHint Transformation::Transformation( Quaternion orientation ) : Transformation( _mm_setzero_ps(), orientation ) {}

// ---------------------------------------------------

	ETInlineHint Transformation ETSimdCall Transformation::GetInverse() const {
		return { -translation, rotation.GetReverse() };
	}

// ---------------------------------------------------

	ETInlineHint Transformation& ETSimdCall Transformation::operator*=( Transformation operand ) {
		translation = operand.translation + operand.rotation.Rotate( translation );
		rotation	= operand.rotation * rotation;
	}

}	// namespace Eldritch2