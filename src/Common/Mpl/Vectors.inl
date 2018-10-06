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
/*	(6297) MSVC doesn't like how 32-bit values are shifted before being cast to a 64-bit value. For Morton code generation this is completely benign;
 *	the warning is built around the presumption that the shift is great enough where bits would spill out of the smaller intermediate value. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6297);
#include <morton.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {

namespace {

	ETInlineHint ETForceInlineHint ETPureFunctionHint __m128 ETSimdCall _mm_xpd_ps(__m128 xmm0, __m128 xmm1) ETNoexceptHint {
		register __m128 xmm2 = _mm_mul_ps(xmm1, _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 0, 2, 1)));
		register __m128 xmm3 = _mm_mul_ps(xmm0, _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(3, 0, 2, 1)));
		xmm0                 = _mm_sub_ps(xmm2, xmm3);

		return _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 0, 2, 1));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint __m128 ETSimdCall _mm_vec2quat_ps(__m128 xmm0, __m128 xmm1) ETNoexceptHint {
		register __m128 xmm2, xmm3;

		xmm2 = _mm_xpd_ps(xmm0, xmm1);
		xmm0 = _mm_xor_ps(_mm_set_ps(+0.0f, +0.0f, -0.0f, -0.0f), _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 3, 3, 3)));
		xmm1 = _mm_xor_ps(_mm_set_ps(+0.0f, -0.0f, +0.0f, -0.0f), _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(2, 2, 2, 2)));
		xmm2 = _mm_xor_ps(_mm_set_ps(+0.0f, -0.0f, -0.0f, +0.0f), _mm_shuffle_ps(xmm2, xmm2, _MM_SHUFFLE(1, 1, 1, 1)));

		xmm3 = _mm_add_ps(xmm0, _mm_set_ps1(1.0f));
		xmm3 = _mm_add_ps(_mm_add_ps(xmm1, xmm2), xmm3);

		return _mm_mul_ps(_mm_sqrt_ps(_mm_max_ps(xmm3, _mm_setzero_ps())), _mm_set_ps1(0.5f));
	}

	// ---------------------------------------------------

	//	Uses ~7 XMM registers.
	ETInlineHint ETForceInlineHint ETPureFunctionHint __m128 ETSimdCall _mm_mulquat_ps(__m128 xmm0, __m128 xmm1) ETNoexceptHint {
		/*	i = w0i1 + i0w1 + j0k1 - k0j1
		 *	j = w0j1 + j0w1 + k0i1 - i0k1
		 *	k = w0k1 + k0w1 + i0j1 - j0i1
		 *	w = w0w1 - i0i1 - j0j1 - k0k1 */
		//	XOR an XMM register with this value to flip the IEEE754 sign bit in the W position.
		static const __m128 FlipWSignMask(_mm_set_ss(-0.0f));
		register __m128     xmm2, xmm3, xmm4, xmm5;

		xmm2 = _mm_mul_ps(_mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 3, 3, 3)), xmm1 /*Identity shuffle*/);
		xmm3 = _mm_mul_ps(_mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3, 2, 1, 3)), _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(0, 0, 0, 3)));
		xmm4 = _mm_mul_ps(_mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(2, 1, 3, 2)), _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(1, 3, 2, 2)));
		xmm5 = _mm_mul_ps(_mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(1, 3, 2, 1)), _mm_shuffle_ps(xmm1, xmm1, _MM_SHUFFLE(2, 1, 3, 1)));
		xmm2 = _mm_add_ps(_mm_xor_ps(xmm3, FlipWSignMask), xmm2);
		xmm4 = _mm_sub_ps(_mm_xor_ps(xmm4, FlipWSignMask), xmm5);

		return _mm_add_ps(xmm2, xmm4);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint __m128 ETSimdCall _mm_unpackquat_ps(uint32 bits) ETNoexceptHint {
		static ETConstexpr float32 InverseDomain(1.0f / 1024.0f);
		static ETConstexpr __m128 ShrinkDomain { InverseDomain, InverseDomain, InverseDomain, InverseDomain };
		const auto                ChannelBits([](uint32 bits) ETNoexceptHint -> int { const uint32 masked(bits & 0x0A); reinterpret_cast<const int&>(masked); });

		register __m128 xmm = _mm_mul_ps(_mm_cvtepi32_ps(_mm_set_epi32(ChannelBits(bits >> 2u), ChannelBits(bits >> 12u), ChannelBits(bits >> 22u), /*_I0 =*/0)), ShrinkDomain);
		// w = 1.0f - sqrt(x*x + y*y + z*z), set via bitwise OR with known 0 channel.
		xmm = _mm_or_ps(_mm_sub_ss(_mm_set_ss(1.0f), _mm_sqrt_ss(_mm_dp_ps(xmm, xmm, 0x1F))), xmm);
		switch (bits & 0x03) {
			ET_NO_DEFAULT_CASE;
		case 0: return _mm_shuffle_ps(xmm, xmm, _MM_SHUFFLE(3, 2, 1, 0));
		case 1: return _mm_shuffle_ps(xmm, xmm, _MM_SHUFFLE(3, 2, 0, 1));
		case 2: return _mm_shuffle_ps(xmm, xmm, _MM_SHUFFLE(3, 0, 2, 1));
		case 3: return _mm_shuffle_ps(xmm, xmm, _MM_SHUFFLE(0, 3, 2, 1));
		};
	}

} // anonymous namespace

ETConstexpr ETInlineHint ETForceInlineHint Vector::Vector(float32 x, float32 y, float32 z, float32 w) ETNoexceptHint : coefficients { w, z, y, x } {}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Vector::Vector(__m128 coefficients) ETNoexceptHint : coefficients(coefficients) {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector::Vector(const float32* coefficients) ETNoexceptHint : coefficients(_mm_loadu_ps(coefficients)) {}

// ---------------------------------------------------

template <Vector::Component component0, Vector::Component component1, Vector::Component component2, Vector::Component component3>
ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall Vector::Swizzle() const ETNoexceptHint {
	return _mm_shuffle_ps(coefficients, coefficients, unsigned int(_MM_SHUFFLE(component3, component2, component1, component0)));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector& Vector::operator+=(Vector rhs) ETNoexceptHint {
	coefficients = _mm_add_ps(coefficients, rhs);
	return *this;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector& Vector::operator-=(Vector rhs) ETNoexceptHint {
	coefficients = _mm_sub_ps(coefficients, rhs.coefficients);
	return *this;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector& Vector::operator*=(Vector rhs) ETNoexceptHint {
	coefficients = _mm_mul_ps(coefficients, rhs);
	return *this;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector& Vector::operator*=(float32 rhs) ETNoexceptHint {
	coefficients = _mm_mul_ps(coefficients, _mm_set_ps1(rhs));
	return *this;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector& Vector::operator/=(Vector rhs) ETNoexceptHint {
	coefficients = _mm_div_ps(coefficients, rhs);
	return *this;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector& Vector::operator/=(float32 rhs) ETNoexceptHint {
	coefficients = _mm_div_ps(coefficients, _mm_set_ps1(rhs));
	return *this;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector& Vector::operator^=(Vector rhs) ETNoexceptHint {
	coefficients = _mm_xpd_ps(coefficients, rhs);
	return *this;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall operator+(Vector lhs, Vector rhs) ETNoexceptHint {
	return _mm_add_ps(lhs, rhs);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall operator-(Vector lhs, Vector rhs) ETNoexceptHint {
	return _mm_sub_ps(lhs, rhs);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall operator*(Vector lhs, Vector rhs)ETNoexceptHint {
	return _mm_mul_ps(lhs, rhs);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall operator*(Vector lhs, float32 rhs)ETNoexceptHint {
	return _mm_mul_ps(lhs, _mm_set_ps1(rhs));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall operator/(Vector lhs, Vector rhs) ETNoexceptHint {
	return _mm_div_ps(lhs, rhs);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall operator/(Vector lhs, float32 rhs) ETNoexceptHint {
	return _mm_div_ps(lhs, _mm_set_ps1(rhs));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall operator^(Vector lhs, Vector rhs) ETNoexceptHint {
	return _mm_xpd_ps(lhs, rhs);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector ETSimdCall operator~(Vector vector) ETNoexceptHint {
	return _mm_rcp_ps(vector);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector ETSimdCall operator-(Vector vector) ETNoexceptHint {
	return _mm_sub_ps(_mm_setzero_ps(), vector);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector& Vector::Normalize() ETNoexceptHint {
	coefficients = _mm_mul_ps(_mm_rsqrt_ps(_mm_dp_ps(coefficients, coefficients, 0xFF)), coefficients);
	return *this;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint void ETSimdCall StreamCoefficients(float32 (&coefficients)[4], Vector vector) ETNoexceptHint {
	// _mm_store_ps(coefficients, vector);
	_mm_stream_ps(coefficients, vector);
}

// ---------------------------------------------------

template <typename Code>
ETInlineHint ETForceInlineHint ETPureFunctionHint Code ETSimdCall GetMortonCode(Vector vector) ETNoexceptHint {
	ET16ByteAligned int32 coefficients[4];

	_mm_store_si128(reinterpret_cast<__m128i*>(coefficients), _mm_castsi128_ps(_mm_cvt_ps2pi(_mm_floor_ps(vector))));
	return m3D_e_sLUT<Code>(coefficients[Vector::X], coefficients[Vector::Y], coefficients[Vector::Z]);
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint ETSimdCall Vector::operator const __m128&() const ETNoexceptHint {
	return coefficients;
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint ETSimdCall Vector::operator __m128&() ETNoexceptHint {
	return coefficients;
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Vector ETSimdCall Vector::MakeIdentity() ETNoexceptHint {
	return _mm_setzero_ps();
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Vector ETSimdCall Vector::MakeForward() ETNoexceptHint {
	return { 0.0f, 0.0f, 1.0f, 0.0f };
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Vector ETSimdCall Vector::MakeUp() ETNoexceptHint {
	return { 0.0f, 1.0f, 0.0f, 0.0f };
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Vector ETSimdCall Vector::MakeRight() ETNoexceptHint {
	return { 1.0f, 0.0f, 0.0f, 0.0f };
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall Min(Vector lhs, Vector rhs) ETNoexceptHint {
	return _mm_min_ps(lhs, rhs);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall Max(Vector lhs, Vector rhs) ETNoexceptHint {
	return _mm_max_ps(lhs, rhs);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall Clamp(Vector value, Vector minima, Vector maxima) ETNoexceptHint {
	return Min(Max(value, minima), maxima);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall AbsoluteValue(Vector vector) ETNoexceptHint {
	return _mm_andnot_ps(vector, _mm_set_ps1(-0.0f));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall Ceil(Vector vector) ETNoexceptHint {
	return _mm_ceil_ps(vector);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall Floor(Vector vector) ETNoexceptHint {
	return _mm_floor_ps(vector);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint float32 ETSimdCall DotProduct(Vector lhs, Vector rhs) ETNoexceptHint {
	return _mm_cvtss_f32(_mm_dp_ps(lhs, rhs, 0xFF));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint float32 ETSimdCall SquaredLength(Vector vector) ETNoexceptHint {
	return _mm_cvtss_f32(_mm_dp_ps(vector, vector, 0xFF));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint float32 ETSimdCall Length(Vector vector) ETNoexceptHint {
	return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(vector, vector, 0xFF)));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall Interpolate(Vector lhs, Vector rhs, float32 alpha) ETNoexceptHint {
	return _mm_add_ps(lhs, _mm_mul_ps(_mm_sub_ps(rhs, lhs), _mm_set_ps1(alpha)));
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Quaternion::Quaternion(float32 i, float32 j, float32 k, float32 w) ETNoexceptHint : coefficients { w, k, j, i } {}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Quaternion::Quaternion(__m128 coefficients) ETNoexceptHint : coefficients(coefficients) {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Quaternion::Quaternion(const float32* coefficients) ETNoexceptHint : coefficients(_mm_loadu_ps(coefficients)) {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Quaternion::Quaternion(uint32 packedBits) ETNoexceptHint : coefficients(_mm_unpackquat_ps(packedBits)) {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Quaternion& Quaternion::Normalize() ETNoexceptHint {
	coefficients = _mm_mul_ps(_mm_rsqrt_ps(_mm_dp_ps(coefficients, coefficients, 0xFF)), coefficients);
	return *this;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Quaternion& ETSimdCall Quaternion::operator*=(Quaternion quaternion) ETNoexceptHint {
	coefficients = _mm_mulquat_ps(coefficients, quaternion.coefficients);
	return *this;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector ETSimdCall Quaternion::GetForward() const ETNoexceptHint {
	return Rotate(Vector::MakeForward());
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector ETSimdCall Quaternion::GetRight() const ETNoexceptHint {
	return Rotate(Vector::MakeRight());
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Vector ETSimdCall Quaternion::GetUp() const ETNoexceptHint {
	return Rotate(Vector::MakeUp());
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint ETSimdCall Quaternion::operator const __m128&() const ETNoexceptHint {
	return coefficients;
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint ETSimdCall Quaternion::operator __m128&() ETNoexceptHint {
	return coefficients;
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Quaternion ETSimdCall Quaternion::MakeIdentity() ETNoexceptHint {
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Quaternion ETSimdCall operator*(Quaternion lhs, Quaternion rhs)ETNoexceptHint {
	return _mm_mulquat_ps(lhs, rhs);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Vector ETSimdCall operator*(Quaternion lhs, Vector rhs)ETNoexceptHint {
	register __m128 xmm(_mm_mul_ps(_mm_xpd_ps(lhs, rhs), _mm_set_ps1(2.0f)));
	register __m128 w(_mm_shuffle_ps(lhs, lhs, _MM_SHUFFLE(0, 0, 0, 0)));
	return _mm_add_ps(_mm_add_ps(rhs, _mm_mul_ps(w, xmm)), _mm_xpd_ps(lhs, xmm));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Quaternion ETSimdCall operator-(Quaternion quaternion) ETNoexceptHint {
	return _mm_mul_ps(quaternion, _mm_set_ps(1.0f, 1.0f, 1.0f, -1.0f));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Quaternion ETSimdCall operator~(Quaternion quaternion) ETNoexceptHint {
	return _mm_mul_ps(quaternion, _mm_set_ps(-1.0f, -1.0f, -1.0f, 1.0f));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint float32 ETSimdCall DotProduct(Quaternion lhs, Quaternion rhs) ETNoexceptHint {
	return _mm_cvtss_f32(_mm_dp_ps(lhs, rhs, 0xFF));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint void ETSimdCall StreamCoefficients(float32 (&coefficients)[4], Quaternion quaternion) ETNoexceptHint {
	_mm_stream_ps(coefficients, quaternion);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Quaternion ETSimdCall AsBasis(Vector forward, Vector up) ETNoexceptHint {
	return _mm_vec2quat_ps(forward, up);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Quaternion ETSimdCall InterpolateNonuniformVelocity(Quaternion lhs, Quaternion rhs, float32 alpha) ETNoexceptHint {
	/*	Determine whether or not one of the quaternions must be negated in order for shortest-path interpolation.
	 *	Mask bits will be set to all ones if the dot product is less than 0. */
	const __m128 mask(_mm_cmplt_ps(_mm_dp_ps(lhs, rhs, 0xFF), _mm_setzero_ps()));
	//	Negate the initial quaternion if necessary. This is done in a branchless way with masking.
	lhs.coefficients = _mm_or_ps(_mm_and_ps(mask, _mm_mul_ps(lhs, _mm_set_ps1(-1.0f))), _mm_andnot_ps(mask, lhs));
	//	Perform a conventional linear interpolation on the coefficients of the quaternion...
	const __m128 interpolated(_mm_add_ps(lhs, _mm_mul_ps(_mm_sub_ps(rhs, lhs), _mm_set_ps1(alpha))));
	//	... then normalize the result to remove any undesired scaling.
	return _mm_mul_ps(_mm_rsqrt_ps(_mm_dp_ps(interpolated, interpolated, 0xFF)), interpolated);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Quaternion ETSimdCall Interpolate(Quaternion lhs, Quaternion rhs, float32 alpha) ETNoexceptHint {
	/*	Adapted from the original by Jonathan Blow, see http://number-none.com/product/Hacking%20Quaternions/index.html
	 *	In particular, the alpha parameter has been changed to take the cosine thereof to exploit the quaternion dot product
	 *	along with factoring out a few terms to be global constants (see above) instead of parameters. */
	const auto AdjustAlpha([](float32 alpha, float32 cosTheta) -> float32 {
		//	RMS minimized coefficients for the quaternion alpha spline adjustment, taken from http://number-none.com/product/Hacking%20Quaternions/index.html
		static ETConstexpr float64 alphaAttenuation(0.7878088);
		static ETConstexpr float64 alphaK(0.5069269);
		const float64              factor(1.0 - (alphaAttenuation * cosTheta));
		const float32              k(float32(alphaK * factor * factor));

		return alpha * ((2.0f * k) * alpha + (-3.0f * k)) + (1.0f + k);
	});

	// ---

	return InterpolateNonuniformVelocity(lhs, rhs, AdjustAlpha(alpha, DotProduct(lhs, rhs)));
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Transformation::Transformation(Vector translation, Quaternion rotation) ETNoexceptHint : translation(translation), rotation(rotation) {}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Transformation::Transformation(Vector translation) ETNoexceptHint : Transformation(translation, Quaternion::MakeIdentity()) {}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Transformation::Transformation(Quaternion orientation) ETNoexceptHint : Transformation(Vector::MakeIdentity(), orientation) {}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Transformation ETSimdCall Transformation::MakeIdentity() ETNoexceptHint {
	return { Vector::MakeIdentity(), Quaternion::MakeIdentity() };
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Transformation& ETSimdCall Transformation::operator*=(Transformation rhs) ETNoexceptHint {
	translation = rhs.rotation * translation + rhs.translation;
	rotation    = rhs.rotation * rotation;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Transformation ETSimdCall operator*(Transformation lhs, Transformation rhs)ETNoexceptHint {
	return lhs *= rhs;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Transformation ETSimdCall operator~(Transformation transformation) ETNoexceptHint {
	return { -transformation.translation, ~transformation.rotation };
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Plane::Plane(float32 x, float32 y, float32 z, float32 w) ETNoexceptHint : coefficients { w, z, y, x } {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Plane::Plane(const float32* values) ETNoexceptHint : coefficients(_mm_loadu_ps(values)) {}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Plane::Plane(__m128 coefficients) ETNoexceptHint : coefficients(coefficients) {}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint Box::Box(Vector minima, Vector maxima) ETNoexceptHint : minima(minima), maxima(maxima) {}

} // namespace Eldritch2
