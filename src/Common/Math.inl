/*==================================================================*\
  Math.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Platform.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
#	include <intrin.h>
#endif
#include <cmath>
//------------------------------------------------------------------//

#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
#	pragma intrinsic(_BitScanReverse)
#	pragma intrinsic(pow)
#	pragma intrinsic(log)
#	if ET_PLATFORM_64BIT
#		pragma intrinsic(_BitScanReverse64)
#	endif
#endif

namespace Eldritch2 {

ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint32 Log2(uint16 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	const uint32 temp(Log2(static_cast<uint32>(value)));
	return temp != 0u ? (temp - 16u) : temp;
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint32 Log2(uint32 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	unsigned long result;

	return _BitScanReverse(&result, value) ? static_cast<uint32>(result) : 0u;
#else
#	error No Log2 implementation available!
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint32 Log2(uint64 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	unsigned long result;

	return _BitScanReverse64(&result, value) ? static_cast<uint32>(result) : 0u;
#else
#	error No Log2 implementation available!
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Log2(float32 value) {
	return ::log2(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Log2(float64 value) {
	return ::log2(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Log10(float32 value) {
	return ::log10(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Log10(float64 value) {
	return ::log10(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Sine(Angle value) {
	return Sine(static_cast<float32>(value));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Sine(float32 value) {
	const float32 temp(ApproxSine(value));

	return 0.225f * (temp * AbsoluteValue(temp) - temp) + temp;
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Sine(float64 value) {
	const float64 temp(ApproxSine(value));

	return 0.225 * (temp * AbsoluteValue(temp) - temp) + temp;
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Cosine(Angle value) {
	return Cosine(static_cast<float32>(value));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Cosine(float32 value) {
	static const float32 piOverTwo(1.5707963267948966192313216916398f);
	static const float32 twoPi(6.283185307179586476925286766559f);
	const float32        temp(value + piOverTwo);
	const int32          temp2(reinterpret_cast<const int32&>(twoPi) & static_cast<int32>(temp > twoPi));

	return Sine(temp - reinterpret_cast<const float32&>(temp2));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Cosine(float64 value) {
	static const float64 piOverTwo(1.5707963267948966192313216916398f);
	static const float64 twoPi(6.283185307179586476925286766559f);
	const float64        temp(value + piOverTwo);
	const int64          temp2(reinterpret_cast<const int64&>(twoPi) & static_cast<int64>(temp > twoPi));

	return Sine(temp - reinterpret_cast<const float64&>(temp2));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Tangent(Angle value) {
	return Tangent(float32(value));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Tangent(float32 value) {
	float32 sine, cosine;

	SineCosine(value, sine, cosine);
	return (sine * Reciprocal(cosine));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Tangent(float64 value) {
	float64 sine, cosine;

	SineCosine(value, sine, cosine);
	return (sine * Reciprocal(cosine));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 ApproxSine(Angle value) {
	return ApproxSine(float32(value));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 ApproxSine(float32 value) {
	static const float32 fourOverPiSquared(-0.40528473456935108577551785283891f);
	static const float32 fourOverPi(1.2732395447351626861510701069801f);

	return (fourOverPi * value + (fourOverPiSquared * value) * AbsoluteValue(value));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 ApproxSine(float64 value) {
	static const float64 fourOverPiSquared(-0.40528473456935108577551785283891);
	static const float64 fourOverPi(1.2732395447351626861510701069801);

	return (fourOverPi * value + (fourOverPiSquared * value) * AbsoluteValue(value));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 ApproxCosine(Angle value) {
	return ApproxCosine(float32(value));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 ApproxCosine(float32 value) {
	static const float32 piOverTwo(1.5707963267948966192313216916398f);
	static const float32 twoPi(6.283185307179586476925286766559f);
	const float32        temp(value + piOverTwo);
	const int32          temp2(reinterpret_cast<const int32&>(twoPi) & int32(temp > twoPi));

	return ApproxSine(temp - reinterpret_cast<const float32&>(temp2));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 ApproxCosine(float64 value) {
	static const float64 piOverTwo(1.5707963267948966192313216916398f);
	static const float64 twoPi(6.283185307179586476925286766559f);
	const float64        temp(value + piOverTwo);
	const int64          temp2(reinterpret_cast<const int64&>(twoPi) & int64(temp > twoPi));

	return ApproxSine(temp - reinterpret_cast<const float64&>(temp2));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 ApproxTangent(Angle value) {
	return ApproxTangent(float32(value));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 ApproxTangent(float32 value) {
	float32 sine, cosine;

	ApproxSineCosine(value, sine, cosine);
	return (sine * Reciprocal(cosine));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 ApproxTangent(float64 value) {
	float64 sine, cosine;

	ApproxSineCosine(value, sine, cosine);
	return (sine * Reciprocal(cosine));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint void SineCosine(Angle value, float32& sine, float32& cosine) {
	return SineCosine(static_cast<float32>(value), sine, cosine);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint void SineCosine(float32 value, float32& sine, float32& cosine) {
	sine   = Sine(value);
	cosine = Cosine(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint void SineCosine(float64 value, float64& sine, float64& cosine) {
	sine   = Sine(value);
	cosine = Cosine(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint void ApproxSineCosine(Angle value, float32& sine, float32& cosine) {
	return ApproxSineCosine(static_cast<float32>(value), sine, cosine);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint void ApproxSineCosine(float32 value, float32& sine, float32& cosine) {
	sine   = ApproxSine(value);
	cosine = ApproxCosine(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint void ApproxSineCosine(float64 value, float64& sine, float64& cosine) {
	sine   = ApproxSine(value);
	cosine = ApproxCosine(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Sqrt(float32 value) {
#if ET_PLATFORM_X86
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_load_ss(&value)));
#else
	return ::sqrtf(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Sqrt(float64 value) {
#if ET_PLATFORM_X86
	const __m128d temp = _mm_load_sd(&value);
	return _mm_cvtsd_f64(_mm_sqrt_sd(temp, temp));
#else
	return ::sqrt(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 ReciprocalSqrt(float32 value) {
#if ET_PLATFORM_X86
#	if 0
		const float32 temp(AsFloatBits((0xBE6EB50C - AsIntBits(value)) >> 1));
		return 0.5f * temp * (3.0f - value * temp * temp);
#	else
	return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_load_ss(&value)));
#	endif
#else
	return 1.0 / Sqrt(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 ReciprocalSqrt(float64 value) {
#if ET_PLATFORM_X86
	const float64 temp(AsFloatBits((0xBFCDD6A18F6A6F54 - AsIntBits(value)) >> 1));
	return 0.5 * temp * (3.0 - value * temp * temp);
#else
	return (1.0 / Sqrt(value));
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Reciprocal(float32 value) {
#if ET_PLATFORM_X86
	return _mm_cvtss_f32(_mm_rcp_ss(_mm_load_ss(&value)));
#else
	return 1.0f / value;
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Reciprocal(float64 value) {
	return 1.0 / value;
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Pow(float32 base, float32 exponent) {
	return ::powf(base, exponent);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Pow(float64 base, float64 exponent) {
	return ::pow(base, exponent);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Exp2(float32 value) {
	return Pow(2.0f, value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Exp2(float64 value) {
	return Pow(2.0, value);
}

// ---------------------------------------------------

template <typename T>
ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint T Min(T lhs, T rhs) {
	return lhs < rhs ? lhs : rhs;
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Min(float32 lhs, float32 rhs) {
#if (ET_PLATFORM_X86)
	return _mm_cvtss_f32(_mm_min_ss(_mm_set_ss(lhs), _mm_set_ss(rhs)));
#else
	return lhs > rhs ? rhs : lhs;
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Min(float64 lhs, float64 rhs) {
#if ET_PLATFORM_X86
	return _mm_cvtsd_f64(_mm_min_sd(_mm_set_sd(lhs), _mm_set_sd(rhs)));
#else
	return lhs > rhs ? rhs : lhs;
#endif
}

// ---------------------------------------------------

template <typename T>
ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint T Max(T lhs, T rhs) {
	return lhs < rhs ? rhs : lhs;
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Max(float32 lhs, float32 rhs) {
#if ET_PLATFORM_X86
	return _mm_cvtss_f32(_mm_max_ss(_mm_set_ss(lhs), _mm_set_ss(rhs)));
#else
	return lhs > rhs ? lhs : rhs;
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Max(float64 lhs, float64 rhs) {
#if ET_PLATFORM_X86
	return _mm_cvtsd_f64(_mm_max_sd(_mm_set_sd(lhs), _mm_set_sd(rhs)));
#else
	return lhs > rhs ? lhs : rhs;
#endif
}

// ---------------------------------------------------

template <typename T>
ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint T Clamp(T value, T min, T max) {
	return Max(Min(max, value), min);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Clamp(float32 value, float32 min, float32 max) {
#if ET_PLATFORM_X86
	return _mm_cvtss_f32(_mm_min_ss(_mm_max_ss(_mm_set_ss(value), _mm_set_ss(min)), _mm_set_ss(max)));
#else
	return Min(Max(value, min), max);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Clamp(float64 value, float64 min, float64 max) {
#if ET_PLATFORM_X86
	return _mm_cvtsd_f64(_mm_min_sd(_mm_max_sd(_mm_set_sd(value), _mm_set_sd(min)), _mm_set_sd(max)));
#else
	return Min(Max(value, min), max);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Saturate(float32 value) {
	return Clamp(value, 0.0f, 1.0f);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Saturate(float64 value) {
	return Clamp(value, 0.0, 1.0);
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Sign(float32 value) {
	return value != 0.0f ? value < 0.0f ? -1.0f : 1.0f : 0.0f;
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Sign(float64 value) {
	return value != 0.0 ? value < 0.0 ? -1.0 : 1.0 : 0.0;
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint int8 Sign(int8 value) {
	return int8(value != 0 ? value < 0 ? -1 : 1 : 0);
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint int16 Sign(int16 value) {
	return int16(value != 0 ? value < 0 ? -1 : 1 : 0);
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint int32 Sign(int32 value) {
	return value != 0 ? value < 0 ? -1 : 1 : 0;
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint int64 Sign(int64 value) {
	return value != 0 ? value < 0 ? -1 : 1 : 0;
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 AbsoluteValue(float32 value) {
#if ET_PLATFORM_X86
	return _mm_cvtss_f32(_mm_andnot_ps(_mm_set_ss(-0.0f), _mm_set_ss(value)));
#else
	return ::fabsf(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 AbsoluteValue(float64 value) {
#if ET_PLATFORM_X86
	return _mm_cvtsd_f64(_mm_andnot_pd(_mm_set_sd(-0.0), _mm_set_sd(value)));
#else
	return ::fabs(value);
#endif
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint int8 AbsoluteValue(int8 value) {
	return value < 0 ? -value : value;
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint int16 AbsoluteValue(int16 value) {
	return value < 0 ? -value : value;
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint int32 AbsoluteValue(int32 value) {
	return value < 0 ? -value : value;
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint int64 AbsoluteValue(int64 value) {
	return value < 0 ? -value : value;
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Floor(float32 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	/*	cvttss2si    ebx,    [float_value]
		mov          eax,    [float_value]
		shr          eax,    31
		sub          ebx     eax
		cvtsi2ss     xmm0,   ebx */

	__m128 xmm0 = _mm_set_ss(value);
	int32  ebx  = _mm_cvttss_si32(xmm0);
	uint32 eax  = reinterpret_cast<uint32&>(value) >> 31u;

	return _mm_cvtss_f32(_mm_cvtsi32_ss(xmm0, ebx - reinterpret_cast<int32&>(eax)));
#else
	return ::floorf(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Floor(float64 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	/*	cvttsd2si eax, float_value
		mov ecx, dword ptr float_value+4
		shr ecx, 31
		sub eax, ecx
		cvtsi2ss xmm0, ecx */

	__m128d xmm0 = _mm_set_sd(value);
	int32   eax  = _mm_cvttsd_si32(xmm0);
	int32   ecx  = reinterpret_cast<uint32*>(&value)[1] >> 31u;

	return _mm_cvtsd_f64(_mm_cvtsi32_sd(xmm0, eax - reinterpret_cast<uint32&>(ecx)));
#else
	return ::floor(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Ceil(float32 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	/*	movss xmm0, [value]
		addss xmm0, xmm0
		movss xmm1, sngMinusOneHalf
		subss xmm1, xmm0
		cvtss2si eax, xmm1
		sar eax, 1
		neg eax
		cvtsi2ss xmm0, eax */

	int32  eax;
	__m128 xmm0 = _mm_set_ss(value);
	__m128 xmm1 = _mm_set_ss(-0.5f);

	xmm0 = _mm_add_ss(xmm0, xmm0);
	xmm1 = _mm_sub_ss(xmm1, xmm0);
	eax  = _mm_cvttss_si32(xmm1) >> 1u;

	return _mm_cvtss_f32(_mm_cvtsi32_ss(xmm0, -eax));
#else
	return ::ceilf(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Ceil(float64 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	/*	movsd xmm0, [value]
		addsd xmm0, xmm0
		movsd xmm1, dblMinusOneHalf
		subsd xmm1, xmm0
		cvtsd2si eax, xmm1
		sar eax, 1
		neg eax
		cvtsi2sd xmm0, eax
		movsd [return], xmm0 */

	int32   eax;
	__m128d xmm0 = _mm_set_sd(value);
	__m128d xmm1 = _mm_set_sd(-0.5);

	xmm0 = _mm_add_sd(xmm0, xmm0);
	xmm1 = _mm_sub_sd(xmm1, xmm0);
	eax  = _mm_cvttsd_si32(xmm1) >> 1u;

	return _mm_cvtsd_f64(_mm_cvtsi32_sd(xmm0, -eax));
#else
	return ::ceil(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Frac(float32 value) {
	return value - Floor(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Frac(float64 value) {
	return value - Floor(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Fmod(float32 dividend, float32 divisor) {
	return ::fmodf(dividend, divisor);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Fmod(float64 dividend, float64 divisor) {
	return ::fmod(dividend, divisor);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint int32 AsInt(float32 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	return _mm_cvttss_si32(_mm_set_ss(value));
#else
	return int32(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint int32 AsInt(float64 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	return _mm_cvttsd_si32(_mm_set_sd(value));
#else
	return int32(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint int64 AsInt64(float32 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT
	return _mm_cvttsd_si64(_mm_cvtps_pd(_mm_set_ss(value)));
#else
	return int64(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint int64 AsInt64(float64 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT
	return _mm_cvttsd_si64(_mm_set_sd(value));
#else
	return int64(value);
#endif
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint32 AsIntBits(float32 value) {
	return reinterpret_cast<const uint32&>(value);
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint64 AsIntBits(float64 value) {
	return reinterpret_cast<const uint64&>(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 AsFloat(int32 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	return _mm_cvtss_f32(_mm_cvtsi32_ss(_mm_setzero_ps(), value));
#else
	return float32(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 AsFloat(uint32 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	return _mm_cvtss_f32(_mm_cvtsi32_ss(_mm_setzero_ps(), int32(value)));
#else
	return float32(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 AsFloat(int64 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT
	return _mm_cvtss_f32(_mm_cvtpd_ps(_mm_cvtsi64_sd(_mm_setzero_pd(), value)));
#else
	return float32(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 AsFloat(uint64 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT
	return _mm_cvtss_f32(_mm_cvtpd_ps(_mm_cvtsi64_sd(_mm_setzero_pd(), int64(value))));
#else
	return float32(value);
#endif
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 AsFloatBits(uint32 value) {
	return reinterpret_cast<const float32&>(value);
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 AsFloatBits(uint64 value) {
	return reinterpret_cast<const float64&>(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 AsFloat64(int32 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT
	return _mm_cvtsd_f64(_mm_cvtsi64_sd(_mm_setzero_pd(), int64(value)));
#else
	return float64(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 AsFloat64(uint32 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT
	return _mm_cvtsd_f64(_mm_cvtsi64_sd(_mm_setzero_pd(), int64(value)));
#else
	return float64(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 AsFloat64(int64 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT
	return _mm_cvtsd_f64(_mm_cvtsi64_sd(_mm_setzero_pd(), value));
#else
	return float64(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 AsFloat64(uint64 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT
	return _mm_cvtsd_f64(_mm_cvtsi64_sd(_mm_setzero_pd(), int64(value)));
#else
	return float64(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint32 CountLeadingZeroes(uint32 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	unsigned long index;
	return _BitScanForward(&index, value) != 0 ? index : 32u;
#else
#	error CountLeadingZeroes() needs implementation for your platform!
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint32 CountLeadingZeroes(uint64 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	unsigned long index;
	return _BitScanForward64(&index, value) != 0 ? index : 64u;
#else
#	error CountLeadingZeroes() needs implementation for your platform!
#endif
}

} // namespace Eldritch2
