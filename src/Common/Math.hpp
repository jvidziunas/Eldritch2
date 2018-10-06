/*==================================================================*\
  Math.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/FloatTypes.hpp>
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETConstexpr float64 PiD(3.14159265358979323846);
ETConstexpr float32 Pi(3.14159265358979323846f);

ETConstexpr float64 TwoPiD(2.0 * PiD);
ETConstexpr float32 TwoPi(2.0f * Pi);

ETConstexpr float64 PiOverTwoD(PiD / 2.0);
ETConstexpr float32 PiOverTwo(Pi / 2.0f);

ETConstexpr float64 FourPiD(4.0 * PiD);
ETConstexpr float32 FourPi(4.0f * Pi);

ETConstexpr float64 FourOverPiD(4.0 / PiD);
ETConstexpr float32 FourOverPi(4.0f / Pi);

ETConstexpr float64 FourOverPiSquaredD(4.0 / (PiD * PiD));
ETConstexpr float32 FourOverPiSquared(4.0f / (Pi * Pi));

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint uint32 Log2(uint16 value);
ETPureFunctionHint ETNeverThrowsHint uint32 Log2(uint32 value);
ETPureFunctionHint ETNeverThrowsHint uint32 Log2(uint64 value);
ETPureFunctionHint ETNeverThrowsHint float32 Log2(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 Log2(float64 value);

ETPureFunctionHint ETNeverThrowsHint float32 Log10(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 Log10(float64 value);

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint float32 Sine(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 Sine(float64 value);

ETPureFunctionHint ETNeverThrowsHint float32 Cosine(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 Cosine(float64 value);

ETPureFunctionHint ETNeverThrowsHint float32 Tangent(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 Tangent(float64 value);

ETPureFunctionHint ETNeverThrowsHint void SineCosine(float32 value, float32& sine, float32& cosine);
ETPureFunctionHint ETNeverThrowsHint void SineCosine(float64 value, float64& sine, float64& cosine);

ETPureFunctionHint ETNeverThrowsHint float32 FastSine(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 FastSine(float64 value);

ETPureFunctionHint ETNeverThrowsHint float32 FastCosine(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 FastCosine(float64 value);

ETPureFunctionHint ETNeverThrowsHint float32 FastTangent(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 FastTangent(float64 value);

ETPureFunctionHint ETNeverThrowsHint void FastSineCosine(float32 value, float32& sine, float32& cosine);
ETPureFunctionHint ETNeverThrowsHint void FastSineCosine(float64 value, float64& sine, float64& cosine);

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint float32 Sqrt(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 Sqrt(float64 value);

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint float32 Reciprocal(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 Reciprocal(float64 value);

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint float32 ReciprocalSqrt(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 ReciprocalSqrt(float64 value);

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint float32 Pow(float32 base, float32 exponent);
ETPureFunctionHint ETNeverThrowsHint float64 Pow(float64 base, float64 exponent);

ETPureFunctionHint ETNeverThrowsHint float32 Exp2(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 Exp2(float64 value);

// ---------------------------------------------------

template <typename T>
ETConstexpr ETPureFunctionHint ETNeverThrowsHint T Min(T lhs);
template <typename T, typename... U>
ETCpp14Constexpr ETPureFunctionHint ETNeverThrowsHint T Min(T lhs, U... rhs);

template <typename T>
ETConstexpr ETPureFunctionHint ETNeverThrowsHint T Max(T lhs);
template <typename T, typename... U>
ETCpp14Constexpr ETPureFunctionHint ETNeverThrowsHint T Max(T lhs, U... rhs);

template <typename T>
ETConstexpr ETPureFunctionHint ETNeverThrowsHint T Clamp(T value, T min, T max);

ETPureFunctionHint ETNeverThrowsHint float32 Saturate(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 Saturate(float64 value);

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint float32 Sign(float32 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint float64 Sign(float64 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint int8 Sign(int8 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint int16 Sign(int16 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint int32 Sign(int32 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint int64 Sign(int64 value);

ETPureFunctionHint ETNeverThrowsHint float32 AbsoluteValue(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 AbsoluteValue(float64 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint int8 AbsoluteValue(int8 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint int16 AbsoluteValue(int16 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint int32 AbsoluteValue(int32 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint int64 AbsoluteValue(int64 value);

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint float32 Floor(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 Floor(float64 value);

ETPureFunctionHint ETNeverThrowsHint float32 Ceil(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 Ceil(float64 value);

ETPureFunctionHint ETNeverThrowsHint float32 Frac(float32 value);
ETPureFunctionHint ETNeverThrowsHint float64 Frac(float64 value);

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint float32 Fmod(float32 dividend, float32 divisor);
ETPureFunctionHint ETNeverThrowsHint float64 Fmod(float64 dividend, float64 divisor);

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint int32 AsInt(float32 value);
ETPureFunctionHint ETNeverThrowsHint int32 AsInt(float64 value);

ETPureFunctionHint ETNeverThrowsHint int64 AsInt64(float32 value);
ETPureFunctionHint ETNeverThrowsHint int64 AsInt64(float64 value);

ETConstexpr ETPureFunctionHint ETNeverThrowsHint int32 AsIntBits(float32 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint int64 AsIntBits(float64 value);

ETConstexpr ETPureFunctionHint ETNeverThrowsHint uint32 AsUintBits(float32 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint uint64 AsUintBits(float64 value);

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint float32 AsFloat(int32 value);
ETPureFunctionHint ETNeverThrowsHint float32 AsFloat(uint32 value);
ETPureFunctionHint ETNeverThrowsHint float32 AsFloat(int64 value);
ETPureFunctionHint ETNeverThrowsHint float32 AsFloat(uint64 value);

ETPureFunctionHint ETNeverThrowsHint float64 AsFloat64(int32 value);
ETPureFunctionHint ETNeverThrowsHint float64 AsFloat64(uint32 value);
ETPureFunctionHint ETNeverThrowsHint float64 AsFloat64(int64 value);
ETPureFunctionHint ETNeverThrowsHint float64 AsFloat64(uint64 value);

ETConstexpr ETPureFunctionHint ETNeverThrowsHint float32 AsFloatBits(int32 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint float32 AsFloatBits(uint32 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint float64 AsFloatBits(int64 value);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint float64 AsFloatBits(uint64 value);

// ---------------------------------------------------

ETPureFunctionHint ETNeverThrowsHint uint32 CountLeadingZeroes(uint32 value);
ETPureFunctionHint ETNeverThrowsHint uint32 CountLeadingZeroes(uint64 value);

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETNeverThrowsHint float32 Interpolate(float32 lhs, float32 rhs, float32 alpha);
ETConstexpr ETPureFunctionHint ETNeverThrowsHint float64 Interpolate(float64 lhs, float64 rhs, float64 alpha);

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Math.inl>
//------------------------------------------------------------------//
