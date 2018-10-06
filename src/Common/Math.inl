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
#	if ET_PLATFORM_64BIT
#		pragma intrinsic(_BitScanReverse64)
#	endif
#	pragma intrinsic(_BitScanReverse)
#	pragma intrinsic(pow)
#	pragma intrinsic(log)
#endif

namespace Eldritch2 {

namespace Detail {

	ETConstexpr int32 IeeeRcp_NewtonRaphson[]     = { 0x7EF311C2, 0x7EF311C2, 0x7EF312AC };
	ETConstexpr int32 IeeeSqrt_NewtonRaphson[]    = { 0x1FBD1DF5 };
	ETConstexpr int32 IeeeRcpSqrt_NewtonRaphson[] = { 0x5f3759df, 0x5F375A86, 0x5F375A86 };

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint float32 FastIeeeRcpSqrt(float32 in, int32 constant) {
		return AsFloatBits(constant - (AsIntBits(in) >> 1));
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint float32 FastIeeeRcpSqrtNewtonRaphson(float32 half, float32 rcp) {
		return rcp * (-half * (rcp * rcp) + 1.5f);
	}

} // namespace Detail

ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint32 Log2(uint16 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	unsigned long result;
	return _BitScanReverse(ETAddressOf(result), value) ? uint32(result - 16) : 0u;
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint32 Log2(uint32 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	unsigned long result;
	return _BitScanReverse(ETAddressOf(result), value) ? uint32(result) : 0u;
#else
#	error No Log2 implementation available!
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint32 Log2(uint64 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	unsigned long result;
	return _BitScanReverse64(ETAddressOf(result), value) ? uint32(result) : 0u;
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

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Sine(float32 value) {
	const float32 sine(FastSine(value));
	return 0.225f * (sine * AbsoluteValue(sine) - sine) + sine;
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Sine(float64 value) {
	const float64 sine(FastSine(value));
	return 0.225 * (sine * AbsoluteValue(sine) - sine) + sine;
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Cosine(float32 value) {
	const float32 temp(value + PiOverTwo);
	return Sine(temp - AsFloatBits(AsIntBits(TwoPi) & int32(temp > TwoPi)));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Cosine(float64 value) {
	const float64 temp(value + PiOverTwoD);
	return Sine(temp - AsFloatBits(AsIntBits(TwoPiD) & int64(temp > TwoPiD)));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Tangent(float32 value) {
	float32 sine, cosine;

	SineCosine(value, sine, cosine);
	return sine * Reciprocal(cosine);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Tangent(float64 value) {
	float64 sine, cosine;

	SineCosine(value, sine, cosine);
	return sine * Reciprocal(cosine);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 FastSine(float32 value) {
	return (FourOverPi * value + (FourOverPiSquared * value) * AbsoluteValue(value));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 FastSine(float64 value) {
	return (FourOverPiD * value + (FourOverPiSquaredD * value) * AbsoluteValue(value));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 FastCosine(float32 value) {
	const float32 temp(value + PiOverTwo);
	return FastSine(temp - AsFloatBits(AsIntBits(TwoPi) & int32(temp > TwoPi)));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 FastCosine(float64 value) {
	const float64 temp(value + PiOverTwoD);
	return FastSine(temp - AsFloatBits(AsIntBits(TwoPiD) & int64(temp > TwoPiD)));
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 FastTangent(float32 value) {
	float32 sine, cosine;
	FastSineCosine(value, sine, cosine);

	return sine * Reciprocal(cosine);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 FastTangent(float64 value) {
	float64 sine, cosine;
	FastSineCosine(value, sine, cosine);

	return sine * Reciprocal(cosine);
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

ETPureFunctionHint ETNeverThrowsHint ETInlineHint void FastSineCosine(float32 value, float32& sine, float32& cosine) {
	sine   = FastSine(value);
	cosine = FastCosine(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint void FastSineCosine(float64 value, float64& sine, float64& cosine) {
	sine   = FastSine(value);
	cosine = FastCosine(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Sqrt(float32 value) {
#if ET_PLATFORM_X86
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(value)));
#else
	return ::sqrtf(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 Sqrt(float64 value) {
#if ET_PLATFORM_X86
	return _mm_cvtsd_f64(_mm_sqrt_sd(_mm_setzero_pd(), _mm_set_sd(value)));
#else
	return ::sqrt(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 ReciprocalSqrt(float32 value) {
#if ET_PLATFORM_X86
	return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(value)));
#else
	return 1.0f / Sqrt(value);
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float64 ReciprocalSqrt(float64 value) {
	return 1.0 / Sqrt(value);
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint float32 Reciprocal(float32 value) {
#if ET_PLATFORM_X86
	return _mm_cvtss_f32(_mm_rcp_ss(_mm_set_ss(value)));
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
ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint T Min(T lhs) {
	return eastl::move(lhs);
}

// ---------------------------------------------------

template <typename T, typename... U>
ETCpp14Constexpr ETPureFunctionHint ETNeverThrowsHint T Min(T lhs, U... rhs) {
	const T temp(Min(eastl::move(rhs)...));
	return lhs < temp ? lhs : temp;
}

// ---------------------------------------------------

template <typename T>
ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint T Max(T lhs) {
	return eastl::move(lhs);
}

// ---------------------------------------------------

template <typename T, typename... U>
ETCpp14Constexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint T Max(T lhs, U... rhs) {
	const T temp(Max(eastl::move(rhs)...));
	return lhs < temp ? temp : lhs;
}

// ---------------------------------------------------

template <typename T>
ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint T Clamp(T value, T min, T max) {
	return Max(Min(value, max), min);
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
	uint32 eax  = AsUintBits(value) >> 31u;

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
	int32   ecx  = reinterpret_cast<uint32*>(ETAddressOf(value))[1] >> 31u;

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

ETConstexpr ETPureFunctionHint ETNeverThrowsHint int32 AsIntBits(float32 value) {
	return reinterpret_cast<int32&>(value);
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint int64 AsIntBits(float64 value) {
	return reinterpret_cast<int64&>(value);
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint32 AsUintBits(float32 value) {
	return reinterpret_cast<const uint32&>(value);
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint64 AsUintBits(float64 value) {
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

ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint ETNeverThrowsHint float32 AsFloatBits(int32 value) {
	return reinterpret_cast<float32&>(value);
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint ETNeverThrowsHint float32 AsFloatBits(uint32 value) {
	return reinterpret_cast<float32&>(value);
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint ETNeverThrowsHint float64 AsFloatBits(int64 value) {
	return reinterpret_cast<float64&>(value);
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint ETNeverThrowsHint float64 AsFloatBits(uint64 value) {
	return reinterpret_cast<float64&>(value);
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
	return _BitScanForward(ETAddressOf(index), value) != 0 ? index : 32u;
#else
#	error CountLeadingZeroes() needs implementation for your platform!
#endif
}

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint ETInlineHint uint32 CountLeadingZeroes(uint64 value) {
#if ET_COMPILER_IS_MSVC && ET_PLATFORM_X86
	unsigned long index;
	return _BitScanForward64(ETAddressOf(index), value) != 0 ? index : 64u;
#else
#	error CountLeadingZeroes() needs implementation for your platform!
#endif
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint float32 Interpolate(float32 lhs, float32 rhs, float32 alpha) {
	return lhs * (1.0f - alpha) + rhs * alpha;
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint float64 Interpolate(float64 lhs, float64 rhs, float64 alpha) {
	return lhs * (1.0 - alpha) + rhs * alpha;
}

} // namespace Eldritch2
