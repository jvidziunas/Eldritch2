/*==================================================================*\
  MathUtils.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/AngleTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::uint32	Log2( ::Eldritch2::uint16 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::uint32	Log2( ::Eldritch2::uint32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::uint32	Log2( ::Eldritch2::uint64 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Log2( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Log2( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Sine( ::Eldritch2::RadianMeasure value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Sine( ::Eldritch2::DegreeMeasure value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Sine( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Sine( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Cosine( ::Eldritch2::RadianMeasure value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Cosine( ::Eldritch2::DegreeMeasure value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Cosine( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Cosine( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Tangent( ::Eldritch2::RadianMeasure value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Tangent( ::Eldritch2::DegreeMeasure value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Tangent( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Tangent( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FastSine( ::Eldritch2::RadianMeasure value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FastSine( ::Eldritch2::DegreeMeasure value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FastSine( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	FastSine( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FastCosine( ::Eldritch2::RadianMeasure value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FastCosine( ::Eldritch2::DegreeMeasure value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FastCosine( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	FastCosine( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FastTangent( ::Eldritch2::RadianMeasure value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FastTangent( ::Eldritch2::DegreeMeasure value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FastTangent( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	FastTangent( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint void	SineCosine( ::Eldritch2::RadianMeasure value, ::Eldritch2::float32& sine, ::Eldritch2::float32& cosine );
	ETNoAliasHint ETNoThrowHint ETInlineHint void	SineCosine( ::Eldritch2::DegreeMeasure value, ::Eldritch2::float32& sine, ::Eldritch2::float32& cosine );
	ETNoAliasHint ETNoThrowHint ETInlineHint void	SineCosine( ::Eldritch2::float32 value, ::Eldritch2::float32& sine, ::Eldritch2::float32& cosine );
	ETNoAliasHint ETNoThrowHint ETInlineHint void	SineCosine( ::Eldritch2::float64 value, ::Eldritch2::float64& sine, ::Eldritch2::float64& cosine );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint void	FastSineCosine( ::Eldritch2::RadianMeasure value, ::Eldritch2::float32& sine, ::Eldritch2::float32& cosine );
	ETNoAliasHint ETNoThrowHint ETInlineHint void	FastSineCosine( ::Eldritch2::DegreeMeasure value, ::Eldritch2::float32& sine, ::Eldritch2::float32& cosine );
	ETNoAliasHint ETNoThrowHint ETInlineHint void	FastSineCosine( ::Eldritch2::float32 value, ::Eldritch2::float32& sine, ::Eldritch2::float32& cosine );
	ETNoAliasHint ETNoThrowHint ETInlineHint void	FastSineCosine( ::Eldritch2::float64 value, ::Eldritch2::float64& sine, ::Eldritch2::float64& cosine );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Sqrt( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Sqrt( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	ReciprocalSqrt( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	ReciprocalSqrt( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Reciprocal( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Reciprocal( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Pow( ::Eldritch2::float32 base, ::Eldritch2::float32 exponent );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Pow( ::Eldritch2::float64 base, ::Eldritch2::float64 exponent );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Exp2( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Exp2( ::Eldritch2::float64 value );

// ---------------------------------------------------

	template <typename T>
	ETNoAliasHint ETNoThrowHint ETInlineHint T	Min( const T& lValue, const T& rValue );

// ---------------------------------------------------

	template <typename T>
	ETNoAliasHint ETNoThrowHint ETInlineHint T	Max( const T& lValue, const T& rValue );

// ---------------------------------------------------

	template <typename T>
	ETNoAliasHint ETNoThrowHint ETInlineHint T	Clamp( const T& value, const T& minValue, const T& maxValue );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Saturate( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Saturate( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Abs( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Abs( ::Eldritch2::float64 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int8		Abs( ::Eldritch2::int8 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int16		Abs( ::Eldritch2::int16 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int32		Abs( ::Eldritch2::int32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int64		Abs( ::Eldritch2::int64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Sign( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Sign( ::Eldritch2::float64 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int8		Sign( ::Eldritch2::int8 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int16		Sign( ::Eldritch2::int16 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int32		Sign( ::Eldritch2::int32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int64		Sign( ::Eldritch2::int64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Floor( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Floor( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Ceil( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Ceil( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Frac( ::Eldritch2::float32 value );
    ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Frac( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	Fmod( ::Eldritch2::float32 dividend, ::Eldritch2::float32 divisor );
    ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Fmod( ::Eldritch2::float64 dividend, ::Eldritch2::float64 divisor );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int32	IntFromFloat( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int32	IntFromFloat( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int64	Int64FromFloat( ::Eldritch2::float32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int64	Int64FromFloat( ::Eldritch2::float64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FloatFromInt( ::Eldritch2::int32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FloatFromInt( ::Eldritch2::uint32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FloatFromInt( ::Eldritch2::int64 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32	FloatFromInt( ::Eldritch2::uint64 value );

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Float64FromInt( ::Eldritch2::int32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Float64FromInt( ::Eldritch2::uint32 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Float64FromInt( ::Eldritch2::int64 value );
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64	Float64FromInt( ::Eldritch2::uint64 value );

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Math/MathUtils.inl>
//------------------------------------------------------------------//