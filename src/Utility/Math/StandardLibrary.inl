/*==================================================================*\
  StandardLibrary.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
#	include <intrin.h>
#endif
#include <cmath>
//------------------------------------------------------------------//

#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
#	pragma intrinsic( _BitScanReverse )
#	pragma intrinsic( pow )
#	pragma intrinsic( log )
#	if( ET_PLATFORM_64BIT )
#		pragma intrinsic( _BitScanReverse64 )
#	endif
#endif

namespace Eldritch2 {

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::uint32 Log2( ::Eldritch2::uint16 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
		::Eldritch2::uint32	temp = Log2( static_cast<::Eldritch2::uint32>( value ) );
		return( !!temp ? ( temp - 16u ) : temp );
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::uint32 Log2( ::Eldritch2::uint32 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
		unsigned long	result;

		return _BitScanReverse( &result, value ) ? static_cast<::Eldritch2::uint32>(result) : 0u;
#else
#	error No Log2 implementation available!
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::uint32 Log2( ::Eldritch2::uint64 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
		unsigned long	result;

		return _BitScanReverse64( &result, value ) ? static_cast<::Eldritch2::uint32>(result) : 0u;
#else
#	error No Log2 implementation available!
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Log2( ::Eldritch2::float32 value ) {
		static const ::Eldritch2::float32	baseConverter( ::Eldritch2::Reciprocal( ::log( 2.0f ) ) );

		return ::log( value ) * baseConverter;
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Log2( ::Eldritch2::float64 value ) {
		static const ::Eldritch2::float64	baseConverter( ::Eldritch2::Reciprocal( ::log( 2.0 ) ) );

		return ::log( value ) * baseConverter;
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Sine( ::Eldritch2::RadianMeasure value ) {
		return ::Eldritch2::Sine( static_cast<::Eldritch2::float32>(value) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Sine( ::Eldritch2::DegreeMeasure value ) {
		return ::Eldritch2::Sine( ::Eldritch2::RadianMeasure(value) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Sine( ::Eldritch2::float32 value ) {
		static const ::Eldritch2::float32	P( 0.225f );
		const ::Eldritch2::float32			temp( ::Eldritch2::FastSine( value ) );

		return( P * ( temp * ::Eldritch2::Abs( temp ) - temp ) + temp );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Sine( ::Eldritch2::float64 value ) {
		static const ::Eldritch2::float64	P( 0.225 );
		const ::Eldritch2::float64			temp( ::Eldritch2::FastSine( value ) );

		return( P * ( temp * ::Eldritch2::Abs( temp ) - temp ) + temp );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint Eldritch2::float32 Cosine( ::Eldritch2::RadianMeasure value ) {
		return ::Eldritch2::Cosine( static_cast<::Eldritch2::float32>( value ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint Eldritch2::float32 Cosine( ::Eldritch2::DegreeMeasure value ) {
		return ::Eldritch2::Cosine( ::Eldritch2::RadianMeasure( value ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint Eldritch2::float32 Cosine( ::Eldritch2::float32 value ) {
		static const ::Eldritch2::float32	piOverTwo( 1.5707963267948966192313216916398f );
		static const ::Eldritch2::float32	twoPi( 6.283185307179586476925286766559f );
		const ::Eldritch2::float32			temp( value + piOverTwo );
		const ::Eldritch2::int32			temp2( reinterpret_cast<const ::Eldritch2::int32&>(twoPi) & static_cast<::Eldritch2::int32>(temp > twoPi) );

		return ::Eldritch2::Sine( temp - reinterpret_cast<const ::Eldritch2::float32&>( temp2 ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Cosine( ::Eldritch2::float64 value ) {
		static const ::Eldritch2::float64	piOverTwo( 1.5707963267948966192313216916398f );
		static const ::Eldritch2::float64	twoPi( 6.283185307179586476925286766559f );
		const ::Eldritch2::float64			temp( value + piOverTwo );
		const ::Eldritch2::int64			temp2( reinterpret_cast<const ::Eldritch2::int64&>(twoPi) & static_cast<::Eldritch2::int64>(temp > twoPi) );

		return ::Eldritch2::Sine( temp - reinterpret_cast<const ::Eldritch2::float64&>( temp2 ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Tangent( ::Eldritch2::RadianMeasure value ) {
		return ::Eldritch2::Tangent( static_cast<::Eldritch2::float32>(value) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Tangent( ::Eldritch2::DegreeMeasure value ) {
		return ::Eldritch2::Tangent( ::Eldritch2::RadianMeasure( value ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Tangent( ::Eldritch2::float32 value ) {
		::Eldritch2::float32	sine, cosine;

		::Eldritch2::SineCosine( value, sine, cosine );
		return( sine * ::Eldritch2::Reciprocal( cosine ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Tangent( ::Eldritch2::float64 value ) {
		::Eldritch2::float64	sine, cosine;

		::Eldritch2::SineCosine( value, sine, cosine );
		return( sine * ::Eldritch2::Reciprocal( cosine ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 FastSine( ::Eldritch2::RadianMeasure value ) {
		return ::Eldritch2::FastSine( static_cast<::Eldritch2::float32>(value) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 FastSine( ::Eldritch2::DegreeMeasure value ) {
		return ::Eldritch2::FastSine( ::Eldritch2::RadianMeasure( value ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 FastSine( ::Eldritch2::float32 value ) {
		static const ::Eldritch2::float32	fourOverPi( 1.2732395447351626861510701069801f );
		static const ::Eldritch2::float32	fourOverPiSquared( -0.40528473456935108577551785283891f );

		return( fourOverPi * value + ( fourOverPiSquared * value ) * ::Eldritch2::Abs( value ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint Eldritch2::float64 FastSine( ::Eldritch2::float64 value ) {
		static const ::Eldritch2::float64	fourOverPi( 1.2732395447351626861510701069801 );
		static const ::Eldritch2::float64	fourOverPiSquared( -0.40528473456935108577551785283891 );

		return( fourOverPi * value + ( fourOverPiSquared * value ) * ::Eldritch2::Abs( value ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 FastCosine( ::Eldritch2::RadianMeasure value ) {
		return ::Eldritch2::FastCosine( static_cast<::Eldritch2::float32>(value) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 FastCosine( ::Eldritch2::DegreeMeasure value ) {
		return ::Eldritch2::FastCosine( ::Eldritch2::RadianMeasure( value ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 FastCosine( ::Eldritch2::float32 value ) {
		static const ::Eldritch2::float32	piOverTwo( 1.5707963267948966192313216916398f );
		static const ::Eldritch2::float32	twoPi( 6.283185307179586476925286766559f );
		const ::Eldritch2::float32			temp( value + piOverTwo );
		const ::Eldritch2::int32			temp2( reinterpret_cast<const ::Eldritch2::int32&>(twoPi) & static_cast<::Eldritch2::int32>(temp > twoPi) );

		return ::Eldritch2::FastSine( temp - reinterpret_cast<const ::Eldritch2::float32&>(temp2) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 FastCosine( ::Eldritch2::float64 value ) {
		static const ::Eldritch2::float64	piOverTwo( 1.5707963267948966192313216916398f );
		static const ::Eldritch2::float64	twoPi( 6.283185307179586476925286766559f );
		const ::Eldritch2::float64			temp( value + piOverTwo );
		const ::Eldritch2::int64			temp2( reinterpret_cast<const ::Eldritch2::int64&>(twoPi) & static_cast<::Eldritch2::int64>(temp > twoPi) );

		return ::Eldritch2::FastSine( temp - reinterpret_cast<const ::Eldritch2::float64&>(temp2) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint Eldritch2::float32 FastTangent( ::Eldritch2::RadianMeasure value ) {
		return ::Eldritch2::FastTangent( static_cast<::Eldritch2::float32>(value) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint Eldritch2::float32 FastTangent( ::Eldritch2::DegreeMeasure value ) {
		return ::Eldritch2::FastTangent( ::Eldritch2::RadianMeasure( value ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint Eldritch2::float32 FastTangent( ::Eldritch2::float32 value ) {
		::Eldritch2::float32	sine, cosine;

		::Eldritch2::FastSineCosine( value, sine, cosine );
		return( sine * ::Eldritch2::Reciprocal( cosine ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint Eldritch2::float64 FastTangent( ::Eldritch2::float64 value ) {
		::Eldritch2::float64	sine, cosine;

		::Eldritch2::FastSineCosine( value, sine, cosine );
		return( sine * ::Eldritch2::Reciprocal( cosine ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint void SineCosine( ::Eldritch2::RadianMeasure value, ::Eldritch2::float32& sine, ::Eldritch2::float32& cosine ) {
		return ::Eldritch2::SineCosine( static_cast<::Eldritch2::float32>(value), sine, cosine );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint void SineCosine( ::Eldritch2::DegreeMeasure value, ::Eldritch2::float32& sine, ::Eldritch2::float32& cosine ) {
		return ::Eldritch2::SineCosine( ::Eldritch2::RadianMeasure( value ), sine, cosine );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint void SineCosine( ::Eldritch2::float32 value, ::Eldritch2::float32& sine, ::Eldritch2::float32& cosine ) {
		sine	= ::Eldritch2::Sine( value );
		cosine	= ::Eldritch2::Cosine( value );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint void SineCosine( ::Eldritch2::float64 value, ::Eldritch2::float64& sine, ::Eldritch2::float64& cosine ) {
		sine	= ::Eldritch2::Sine( value );
		cosine	= ::Eldritch2::Cosine( value );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint void FastSineCosine( ::Eldritch2::RadianMeasure value, ::Eldritch2::float32& sine, ::Eldritch2::float32& cosine ) {
		return ::Eldritch2::FastSineCosine( static_cast<::Eldritch2::float32>(value), sine, cosine );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint void FastSineCosine( ::Eldritch2::DegreeMeasure value, ::Eldritch2::float32& sine, ::Eldritch2::float32& cosine ) {
		return ::Eldritch2::FastSineCosine( ::Eldritch2::RadianMeasure( value ), sine, cosine );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint void FastSineCosine( ::Eldritch2::float32 value, ::Eldritch2::float32& sine, ::Eldritch2::float32& cosine ) {
		sine	= ::Eldritch2::FastSine( value );
		cosine	= ::Eldritch2::FastCosine( value );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint void FastSineCosine( ::Eldritch2::float64 value, ::Eldritch2::float64& sine, ::Eldritch2::float64& cosine ) {
		sine	= ::Eldritch2::FastSine( value );
		cosine	= ::Eldritch2::FastCosine( value );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Sqrt( ::Eldritch2::float32 value ) {
#if( ET_PLATFORM_X86 )
		return ::_mm_cvtss_f32( ::_mm_sqrt_ss( ::_mm_load_ss( &value ) ) );
#else
		return( ::sqrtf( value ) );
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Sqrt( ::Eldritch2::float64 value ) {
#if( ET_PLATFORM_X86 )
		const __m128d	temp = ::_mm_load_sd( &value );
		return ::_mm_cvtsd_f64( ::_mm_sqrt_sd( temp, temp ) );
#else
		return( ::sqrt( value ) );
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 ReciprocalSqrt( ::Eldritch2::float32 value ) {
#if( ET_PLATFORM_X86 )
#	if 0
		union {
			::Eldritch2::float32	f;
			::Eldritch2::uint32		ul;
		} y;

		y.f		= value;
		y.ul	= ( 0xBE6EB50C - y.ul ) >> 1;
		y.f		= 0.5f * y.f * ( 3.0f - value * y.f * y.f );
		return y.f;
#	else
		return ::_mm_cvtss_f32( ::_mm_rsqrt_ss( ::_mm_load_ss( &value ) ) );
#	endif
#else
		return( 1.0 / ::Eldritch2::Sqrt( value ) );
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 ReciprocalSqrt( ::Eldritch2::float64 value ) {
#if( ET_PLATFORM_X86 )
		union {
			::Eldritch2::float64	f;
			::Eldritch2::uint64		ul;
		} y;

		y.f		= value;
		y.ul	= ( 0xBFCDD6A18F6A6F54 - y.ul ) >> 1;
		y.f		=  static_cast<::Eldritch2::float64>(0.5) * y.f * ( static_cast<::Eldritch2::float64>(3.0) - value * y.f * y.f );
		return y.f;
#else
		return( 1.0 / ::Eldritch2::Sqrt( value ) );
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Reciprocal( ::Eldritch2::float32 value ) {
#if( ET_PLATFORM_X86 )
		return ::_mm_cvtss_f32( ::_mm_rcp_ss( ::_mm_load_ss( &value ) ) );
#else
		return( 1.0f / value );
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Reciprocal( ::Eldritch2::float64 value ) {
		return( 1.0 / value );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Pow( ::Eldritch2::float32 base, ::Eldritch2::float32 exponent ) {
		return ::powf( base, exponent );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Pow( ::Eldritch2::float64 base, ::Eldritch2::float64 exponent ) {
		return ::pow( base, exponent );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Exp2( ::Eldritch2::float32 value ) {
		return ::Eldritch2::Pow( 2.0f, value );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Exp2( ::Eldritch2::float64 value ) {
		return ::Eldritch2::Pow( 2.0, value );
	}

// ---------------------------------------------------

	template <typename T>
	ETNoAliasHint ETNoThrowHint ETInlineHint T Min( const T lValue, const T rValue ) {
		return lValue < rValue ? lValue : rValue;
	}

// ---------------------------------------------------

	template <>
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Min<::Eldritch2::float32>( const ::Eldritch2::float32 lValue, const ::Eldritch2::float32 rValue ) {
#	if( ET_PLATFORM_X86 )
		return _mm_cvtss_f32( ::_mm_min_ss( ::_mm_set_ss( lValue ), ::_mm_set_ss( rValue ) ) );
#	else
		return lValue > rValue ? rValue : lValue;
#	endif
	}

// ---------------------------------------------------

	template <>
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Min<::Eldritch2::float64>( const ::Eldritch2::float64 lValue, const ::Eldritch2::float64 rValue ) {
#	if( ET_PLATFORM_X86 )
		return ::_mm_cvtsd_f64( ::_mm_min_sd( ::_mm_set_sd( lValue ), ::_mm_set_sd( rValue ) ) );
#	else
		return lValue > rValue ? rValue : lValue;
#	endif
	}

// ---------------------------------------------------

	template <typename T>
	ETNoAliasHint ETNoThrowHint ETInlineHint T Max( const T lValue, const T rValue ) {
		return lValue < rValue ? rValue : lValue;
	}

// ---------------------------------------------------

	template <>
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Max<::Eldritch2::float32>( const ::Eldritch2::float32 lValue, const ::Eldritch2::float32 rValue ) {
#	if( ET_PLATFORM_X86 )
		return ::_mm_cvtss_f32( ::_mm_max_ss( ::_mm_set_ss( lValue ), ::_mm_set_ss( rValue ) ) );
#	else
		return lValue > rValue ? lValue : rValue;
#	endif
	}

// ---------------------------------------------------

	template <>
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Max<::Eldritch2::float64>( const ::Eldritch2::float64 lValue, const ::Eldritch2::float64 rValue ) {
#	if( ET_PLATFORM_X86 )
		return ::_mm_cvtsd_f64( ::_mm_max_sd( ::_mm_set_sd( lValue ), ::_mm_set_sd( rValue ) ) );
#	else
		return lValue > rValue ? lValue : rValue;
#	endif
	}

// ---------------------------------------------------

    template <typename T>
	ETNoAliasHint ETNoThrowHint ETInlineHint T Clamp( const T value, const T minValue, const T maxValue ) {
		return ::Eldritch2::Max( ::Eldritch2::Min( maxValue, value ), minValue );
	}

// ---------------------------------------------------

	template<>
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Clamp<::Eldritch2::float32>( const ::Eldritch2::float32 value, const ::Eldritch2::float32 minValue, const ::Eldritch2::float32 maxValue ) {
#	if( ET_PLATFORM_X86 )
		return ::_mm_cvtss_f32( ::_mm_min_ss( ::_mm_max_ss( ::_mm_set_ss( value ), ::_mm_set_ss( minValue ) ), ::_mm_set_ss( maxValue ) ) );
#	else
		return ::Eldritch2::Min( ::Eldritch2::Max( value, minValue ), maxValue );
#	endif
	}

// ---------------------------------------------------

	template<>
	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Clamp<Eldritch2::float64>( const ::Eldritch2::float64 value, const ::Eldritch2::float64 minValue, const ::Eldritch2::float64 maxValue ) {
#	if( ET_PLATFORM_X86 )
		return ::_mm_cvtsd_f64( ::_mm_min_sd( ::_mm_max_sd( ::_mm_set_sd( value ), ::_mm_set_sd( minValue ) ), ::_mm_set_sd( maxValue ) ) );
#	else
		return ::Eldritch2::Min( ::Eldritch2::Max( value, minValue ), maxValue );
#	endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Saturate( ::Eldritch2::float32 value ) {
		return ::Eldritch2::Clamp( value, 0.0f, 1.0f );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Saturate( ::Eldritch2::float64 value ) {
		return ::Eldritch2::Clamp( value, 0.0, 1.0 );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Sign( ::Eldritch2::float32 value ) {
		return( 0.0f != value ? ( value / ::Eldritch2::Abs( value ) ) : 0.0f );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Sign( ::Eldritch2::float64 value ) {
		return( 0.0f != value ? ( value / ::Eldritch2::Abs( value ) ) : 0.0f );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int8 Sign( ::Eldritch2::int8 value ) {
		return( 0 != value ? ( value / ::Eldritch2::Abs( value ) ) : 0 );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int16 Sign( ::Eldritch2::int16 value ) {
		return( 0 != value ? ( value / ::Eldritch2::Abs( value ) ) : 0 );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int32 Sign( ::Eldritch2::int32 value ) {
		return( 0 != value ? ( value / ::Eldritch2::Abs( value ) ) : 0 );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int64 Sign( ::Eldritch2::int64 value ) {
		return( 0 != value ? ( value / ::Eldritch2::Abs( value ) ) : 0 );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Abs( ::Eldritch2::float32 value ) {
#if( ET_PLATFORM_X86 )
#	if 0
		const ::Eldritch2::int32	y( reinterpret_cast<::Eldritch2::int32&>(value) & 0x7FFFFFFF );

		return reinterpret_cast<const ::Eldritch2::float32&>(y);
#	else
		return ::_mm_cvtss_f32( ::_mm_andnot_ps( ::_mm_set_ss( -0.0f ), ::_mm_set_ss( value ) ) );
#	endif
#else
		return ::fabsf( value );
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Abs( ::Eldritch2::float64 value ) {
#if( ET_PLATFORM_X86 )
#	if 0
		const ::Eldritch2::int64	y( reinterpret_cast<::Eldritch2::int64&>(value) & 0x7FFFFFFFFFFFFFFF );

		return reinterpret_cast<const ::Eldritch2::float64&>(y);
#	else
		return ::_mm_cvtsd_f64( ::_mm_andnot_pd( ::_mm_set_sd( -0.0 ), ::_mm_set_sd( value ) ) );
#	endif
#else
		return ::fabs( value );
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int8 Abs( ::Eldritch2::int8 value ) {
		return value > 0 ? value : -value;
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int16 Abs( ::Eldritch2::int16 value ) {
		return value > 0 ? value : -value;
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int32 Abs( ::Eldritch2::int32 value ) {
		return value > 0 ? value : -value;
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int64 Abs( ::Eldritch2::int64 value ) {
		return value > 0 ? value : -value;
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Floor( ::Eldritch2::float32 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
		// cvttss2si    ebx,    [float_value]
		// mov          eax,    [float_value]
		// shr          eax,    31
		// sub          ebx     eax
		// cvtsi2ss     xmm0,   ebx
		__m128				xmm0	= ::_mm_set_ss( value );
		::Eldritch2::int32	ebx		= ::_mm_cvttss_si32( xmm0 );
		::Eldritch2::uint32	eax		= reinterpret_cast<::Eldritch2::uint32&>(value) >> 31u;

		return ::_mm_cvtss_f32( ::_mm_cvtsi32_ss( xmm0, ebx - reinterpret_cast<::Eldritch2::int32&>(eax) ) );
#else
		return ::floorf( value );	
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Floor( ::Eldritch2::float64 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
		// cvttsd2si eax, float_value
		// mov ecx, dword ptr float_value+4
		// shr ecx, 31
		// sub eax, ecx
		// cvtsi2ss xmm0, ecx
		__m128d				xmm0	= ::_mm_set_sd( value );
		::Eldritch2::int32	eax		= ::_mm_cvttsd_si32( xmm0 );
		::Eldritch2::int32	ecx		= reinterpret_cast<::Eldritch2::uint32*>(&value)[1] >> 31u;

		return ::_mm_cvtsd_f64( ::_mm_cvtsi32_sd( xmm0, eax - reinterpret_cast<::Eldritch2::uint32&>(ecx) ) );
#else
		return ::floor( value );	
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Ceil( ::Eldritch2::float32 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
		// movss xmm0, [value]
		// addss xmm0, xmm0
		// movss xmm1, sngMinusOneHalf
		// subss xmm1, xmm0
		// cvtss2si eax, xmm1
		// sar eax, 1
		// neg eax
		// cvtsi2ss xmm0, eax

		::Eldritch2::int32	eax;
		__m128				xmm0 = ::_mm_set_ss( value );
		__m128				xmm1 = ::_mm_set_ss( -0.5f );

		xmm0	= ::_mm_add_ss( xmm0, xmm0 );
		xmm1	= ::_mm_sub_ss( xmm1, xmm0 );
		eax		= ::_mm_cvttss_si32( xmm1 ) >> 1u;

		return ::_mm_cvtss_f32( ::_mm_cvtsi32_ss( xmm0, -eax ) );
#else
		return ::ceilf( value );	
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Ceil( ::Eldritch2::float64 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
		// movsd xmm0, [value]
		// addsd xmm0, xmm0
		// movsd xmm1, dblMinusOneHalf
		// subsd xmm1, xmm0
		// cvtsd2si eax, xmm1
		// sar eax, 1
		// neg eax
		// cvtsi2sd xmm0, eax
		// movsd [return], xmm0

		::Eldritch2::int32	eax;
		__m128d				xmm0 = ::_mm_set_sd( value );
		__m128d				xmm1 = ::_mm_set_sd( -0.5 );

		xmm0	= ::_mm_add_sd( xmm0, xmm0 );
		xmm1	= ::_mm_sub_sd( xmm1, xmm0 );
		eax		= ::_mm_cvttsd_si32( xmm1 ) >> 1u;

		return ::_mm_cvtsd_f64( ::_mm_cvtsi32_sd( xmm0, -eax ) );
#else
		return ::ceil( value );	
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Frac( ::Eldritch2::float32 value ) {
		return( value - ::Eldritch2::Floor( value ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Frac( ::Eldritch2::float64 value ) {
		return( value - ::Eldritch2::Floor( value ) );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 Fmod( ::Eldritch2::float32 dividend, ::Eldritch2::float32 divisor ) {
		return ::fmodf( dividend, divisor );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Fmod( ::Eldritch2::float64 dividend, ::Eldritch2::float64 divisor ) {
		return ::fmod( dividend, divisor );
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int32 IntFromFloat( ::Eldritch2::float32 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
		return ::_mm_cvttss_si32( ::_mm_set_ss( value ) );
#else
		return static_cast<::Eldritch2::int32>(value);
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int32 IntFromFloat( ::Eldritch2::float64 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
		return ::_mm_cvttsd_si32( ::_mm_set_sd( value ) );
#else
		return static_cast<::Eldritch2::int32>(value);
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int64 Int64FromFloat( ::Eldritch2::float32 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
		return ::_mm_cvttsd_si64( ::_mm_cvtps_pd( ::_mm_set_ss( value ) ) );
#else
		return static_cast<::Eldritch2::int64>(value);
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::int64 Int64FromFloat( ::Eldritch2::float64 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
		return ::_mm_cvttsd_si64( ::_mm_set_sd( value ) );
#else
		return static_cast<::Eldritch2::int64>(value);
#endif
	}

// ---------------------------------------------------

	// MSVC doesn't like that we use 'temp' in the below functions without initializing it,
	// but for these specific use cases it doesn't actually matter.
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
#		pragma warning( disable : 4700 )
#endif

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 FloatFromInt( ::Eldritch2::int32 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
		return ::_mm_cvtss_f32( ::_mm_cvtsi32_ss( ::_mm_setzero_ps(), value ) );
#else
		return static_cast<::Eldritch2::float32>(value);
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 FloatFromInt( ::Eldritch2::uint32 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 )
		return ::_mm_cvtss_f32( ::_mm_cvtsi32_ss( ::_mm_setzero_ps(), static_cast<::Eldritch2::int32>(value) ) );
#else
		return static_cast<::Eldritch2::float32>(value);
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 FloatFromInt( ::Eldritch2::int64 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
		return ::_mm_cvtss_f32( ::_mm_cvtpd_ps( ::_mm_cvtsi64_sd( ::_mm_setzero_pd(), value ) ) );
#else
		return static_cast<::Eldritch2::float32>(value);
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float32 FloatFromInt( ::Eldritch2::uint64 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
		return ::_mm_cvtss_f32( ::_mm_cvtpd_ps( ::_mm_cvtsi64_sd( ::_mm_setzero_pd(), static_cast<::Eldritch2::int64>(value) ) ) );
#else
		return static_cast<::Eldritch2::float32>(value);
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint Eldritch2::float64 Float64FromInt( ::Eldritch2::int32 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
		return ::_mm_cvtsd_f64( ::_mm_cvtsi64_sd( ::_mm_setzero_pd(), static_cast<::Eldritch2::int64>(value) ) );
#else
		return static_cast<::Eldritch2::float64>(value);
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint Eldritch2::float64 Float64FromInt( ::Eldritch2::uint32 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
		return ::_mm_cvtsd_f64( ::_mm_cvtsi64_sd( ::_mm_setzero_pd(), static_cast<::Eldritch2::int64>( value ) ) );
#else
		return static_cast<::Eldritch2::float64>( value );
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Float64FromInt( ::Eldritch2::int64 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
		return ::_mm_cvtsd_f64( ::_mm_cvtsi64_sd( ::_mm_setzero_pd(), value ) );
#else
		return static_cast<::Eldritch2::float64>( value );
#endif
	}

// ---------------------------------------------------

	ETNoAliasHint ETNoThrowHint ETInlineHint ::Eldritch2::float64 Float64FromInt( ::Eldritch2::uint64 value ) {
#if( ET_COMPILER_IS_MSVC && ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
		return ::_mm_cvtsd_f64( ::_mm_cvtsi64_sd( ::_mm_setzero_pd(), static_cast<::Eldritch2::int64>( value ) ) );
#else
		return static_cast<::Eldritch2::float64>( value );
#endif
	}

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif

}	// namespace Eldritch2