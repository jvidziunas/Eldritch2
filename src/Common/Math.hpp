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
#include <Common/Mpl/IntTypes.hpp>
#include <Common/Angle.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ETPureFunctionHint ETNeverThrowsHint uint32		Log2( uint16 value );
	ETPureFunctionHint ETNeverThrowsHint uint32		Log2( uint32 value );
	ETPureFunctionHint ETNeverThrowsHint uint32		Log2( uint64 value );
	ETPureFunctionHint ETNeverThrowsHint float32	Log2( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	Log2( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Log10( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	Log10( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Sine( Angle value );
	ETPureFunctionHint ETNeverThrowsHint float32	Sine( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	Sine( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Cosine( Angle value );
	ETPureFunctionHint ETNeverThrowsHint float32	Cosine( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	Cosine( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Tangent( Angle value );
	ETPureFunctionHint ETNeverThrowsHint float32	Tangent( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	Tangent( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	ApproxSine( Angle value );
	ETPureFunctionHint ETNeverThrowsHint float32	ApproxSine( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	ApproxSine( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	ApproxCosine( Angle value );
	ETPureFunctionHint ETNeverThrowsHint float32	ApproxCosine( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	ApproxCosine( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	ApproxTangent( Angle value );
	ETPureFunctionHint ETNeverThrowsHint float32	ApproxTangent( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	ApproxTangent( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint void	SineCosine( Angle value, float32& sine, float32& cosine );
	ETPureFunctionHint ETNeverThrowsHint void	SineCosine( float32 value, float32& sine, float32& cosine );
	ETPureFunctionHint ETNeverThrowsHint void	SineCosine( float64 value, float64& sine, float64& cosine );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint void	ApproxSineCosine( Angle value, float32& sine, float32& cosine );
	ETPureFunctionHint ETNeverThrowsHint void	ApproxSineCosine( float32 value, float32& sine, float32& cosine );
	ETPureFunctionHint ETNeverThrowsHint void	ApproxSineCosine( float64 value, float64& sine, float64& cosine );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Sqrt( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	Sqrt( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Reciprocal( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	Reciprocal( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	ReciprocalSqrt( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	ReciprocalSqrt( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Pow( float32 base, float32 exponent );
	ETPureFunctionHint ETNeverThrowsHint float64	Pow( float64 base, float64 exponent );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Exp2( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	Exp2( float64 value );

// ---------------------------------------------------

	template <typename T>
	ETPureFunctionHint ETNeverThrowsHint T	Min( const T lValue, const T rValue );

// ---------------------------------------------------

	template <typename T>
	ETPureFunctionHint ETNeverThrowsHint T	Max( const T lValue, const T rValue );

// ---------------------------------------------------

	template <typename T>
	ETPureFunctionHint ETNeverThrowsHint T	Clamp( const T value, const T minValue, const T maxValue );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Saturate( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	Saturate( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	AbsoluteValue( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	AbsoluteValue( float64 value );
	ETPureFunctionHint ETNeverThrowsHint int8		AbsoluteValue( int8 value );
	ETPureFunctionHint ETNeverThrowsHint int16		AbsoluteValue( int16 value );
	ETPureFunctionHint ETNeverThrowsHint int32		AbsoluteValue( int32 value );
	ETPureFunctionHint ETNeverThrowsHint int64		AbsoluteValue( int64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Sign( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	Sign( float64 value );
	ETPureFunctionHint ETNeverThrowsHint int8		Sign( int8 value );
	ETPureFunctionHint ETNeverThrowsHint int16		Sign( int16 value );
	ETPureFunctionHint ETNeverThrowsHint int32		Sign( int32 value );
	ETPureFunctionHint ETNeverThrowsHint int64		Sign( int64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Floor( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	Floor( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Ceil( float32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	Ceil( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Frac( float32 value );
    ETPureFunctionHint ETNeverThrowsHint float64	Frac( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	Fmod( float32 dividend, float32 divisor );
    ETPureFunctionHint ETNeverThrowsHint float64	Fmod( float64 dividend, float64 divisor );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint int32	AsInt( float32 value );
	ETPureFunctionHint ETNeverThrowsHint int32	AsInt( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint int64	AsInt64( float32 value );
	ETPureFunctionHint ETNeverThrowsHint int64	AsInt64( float64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float32	AsFloat( int32 value );
	ETPureFunctionHint ETNeverThrowsHint float32	AsFloat( uint32 value );
	ETPureFunctionHint ETNeverThrowsHint float32	AsFloat( int64 value );
	ETPureFunctionHint ETNeverThrowsHint float32	AsFloat( uint64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint float64	AsFloat64( int32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	AsFloat64( uint32 value );
	ETPureFunctionHint ETNeverThrowsHint float64	AsFloat64( int64 value );
	ETPureFunctionHint ETNeverThrowsHint float64	AsFloat64( uint64 value );

// ---------------------------------------------------

	ETPureFunctionHint ETNeverThrowsHint uint32	CountLeadingZeroes( uint32 value );
	ETPureFunctionHint ETNeverThrowsHint uint32	CountLeadingZeroes( uint64 value );

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Math.inl>
//------------------------------------------------------------------//