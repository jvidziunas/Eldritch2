/*==================================================================*\
  VectorTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  Rough equivalent to the IntTypes.hpp header, modified to instead
  provide a baseline for floating-point types instead of fixed.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/VectorTypes.hpp>
#include <Utility/Math/MathUtils.hpp>
#include <xmmintrin.h>
#include <emmintrin.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace
{

	static ET16ByteAligned uint32	_abs[4]      = { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF };
	static ET16ByteAligned uint32	_infinity[4] = { 0x47FFEFFF, 0x47FFEFFF, 0x47FFEFFF, 0x47FFEFFF };
	static ET16ByteAligned uint32	_denormal[4] = { 0x38800000, 0x38800000, 0x38800000, 0x38800000 };
	static ET16ByteAligned uint32	_fixup[4]    = { 0x48000000, 0x48000000, 0x48000000, 0x48000000 };
	static ET16ByteAligned uint32	_round1[4]   = { 0x00000001, 0x00000001, 0x00000001, 0x00000001 };
	static ET16ByteAligned uint32	_round2[4]   = { 0x00000FFF, 0x00000FFF, 0x00000FFF, 0x00000FFF };
	static ET16ByteAligned uint32	_sign[4]     = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
	static ET16ByteAligned uint32	_base[4]     = { 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF };
	static ET16ByteAligned uint32	_integer[4]  = { 0x52000000, 0x52000000, 0x52000000, 0x52000000 };

	// RMS minimized coefficients for the quaternion alpha spline adjustment.
	// Taken from http://number-none.com/product/Hacking%20Quaternions/index.html
	static const float64	alphaK = 0.5069269;
	static const float64	alphaAttenuation = 0.7878088;

// ---------------------------------------------------

	// Adapted from the original by Jonathan Blow, see http://number-none.com/product/Hacking%20Quaternions/index.html
	// In particular, the alpha parameter has been changed to take the cosine thereof to exploit the quaternion dot product
	// along with factoring out a few terms to be global constants (see above) instead of parameters.
	float32 QuaternionAlphaCorrection( float32 t, float32 cosAlpha )
	{
		const float64	factor( 1.0 - ( alphaAttenuation * cosAlpha ) );
		const float32	k( alphaK * factor * factor );
		const float32	b(  2.0f * k );
		const float32	c( -3.0f * k );
		const float32	d(  1.0f + k );

		return t * ( b * t + c ) + d;
	}

}	// anonymous namespace

namespace Eldritch2
{

	void Float4ToHalf4( half4& dst, const float4& src )
	{
		register union
		{
			__m128 xmm0;
			int16 halves[sizeof(__m128)/sizeof(int16)];
		};
		register __m128 xmm1;
		register __m128 xmm2;
		register __m128 xmm3;
		register __m128 xmm4;

		// Compute masks
		xmm1 = _mm_load_ps( src.data() );
		xmm0 = _mm_and_ps( xmm1, _mm_load_ps( reinterpret_cast<float*>( _abs ) ) );
		xmm2 = _mm_cmpnlt_ps( xmm1, _mm_load_ps( reinterpret_cast<float*>( _infinity ) ) );
		xmm3 = _mm_cmplt_ps( xmm1, _mm_load_ps( reinterpret_cast<float*>( _denormal ) ) );

		// Denormal case
		xmm4 = _mm_castsi128_ps( _mm_cvttps_epi32( _mm_mul_ps( xmm0, _mm_load_ps( reinterpret_cast<float*>( _integer ) ) ) ) );

		// Normal case and combine
		xmm0 = _mm_castsi128_ps( _mm_add_epi32( _mm_castps_si128( xmm0 ), _mm_castps_si128( _mm_load_ps( reinterpret_cast<float*>( _fixup ) ) ) ) );
		xmm4 = _mm_and_ps( xmm4, xmm3 );
		xmm3 = _mm_andnot_ps( xmm3, xmm0 );
		xmm4 = _mm_or_ps( xmm4, xmm3 );

		// Correct rounding
		xmm0 = _mm_castsi128_ps( _mm_srli_epi32( _mm_castps_si128( xmm4 ), 13 ) );
		xmm0 = _mm_and_ps( xmm0, _mm_load_ps( reinterpret_cast<float*>( _round1 ) ) );
		xmm0 = _mm_castsi128_ps( _mm_add_epi32( _mm_castps_si128( xmm0 ), _mm_castps_si128( _mm_load_ps( reinterpret_cast<float*>( _round2 ) ) ) ) );
		xmm0 = _mm_castsi128_ps( _mm_add_epi32( _mm_castps_si128( xmm0 ), _mm_castps_si128( xmm4 ) ) );

		// Combine with sign and infinity
		xmm0 = _mm_castsi128_ps( _mm_srli_epi32( _mm_castps_si128( xmm0 ), 13 ) );
		xmm1 = _mm_and_ps( xmm1, _mm_load_ps( reinterpret_cast<float*>( _sign ) ) );
		xmm1 = _mm_castsi128_ps( _mm_srli_epi32( _mm_castps_si128( xmm1 ), 16 ) );
		xmm0 = _mm_or_ps( xmm0, xmm2 );
		xmm0 = _mm_and_ps( xmm0, _mm_load_ps( reinterpret_cast<float*>( _base ) ) );
		xmm0 = _mm_or_ps( xmm0, xmm1 );

		dst.x() = halves[1];
		dst.y() = halves[3];
		dst.z() = halves[5];
		dst.w() = halves[7];
	}

// ---------------------------------------------------

	Quaternion QuaternionLerp( const Quaternion& src0, const Quaternion& src1, float32 alpha )
	{
		return Quaternion( src0.coeffs() + alpha * ( src1.coeffs() - src0.coeffs() ) ).normalized();
	}

// ---------------------------------------------------

	Quaternion SplinedQuaternionLerp( const Quaternion& src0, const Quaternion& src1, float32 alpha )
	{
		return QuaternionLerp( src0, src1, QuaternionAlphaCorrection( alpha, src0.dot( src1 ) ) );
	}

// ---------------------------------------------------

	void InPlaceQuaternionLerp( Quaternion& src0, const Quaternion& src1, float32 alpha )
	{
		src0 = src0.coeffs() + alpha * ( src1.coeffs() - src0.coeffs() );
		src0.normalize();
	}

// ---------------------------------------------------

	void InPlaceSplinedQuaternionLerp( Quaternion& src0, const Quaternion& src1, float32 alpha )
	{
		InPlaceQuaternionLerp( src0, src1, QuaternionAlphaCorrection( alpha, src0.dot( src1 ) ) );
	}

}	// namespace Eldritch2