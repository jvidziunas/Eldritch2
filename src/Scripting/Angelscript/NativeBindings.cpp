/*==================================================================*\
  NativeBindings.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/NativeBindings.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace {

	static const float32	µSecPerSec			= 1000000.0f;
	static const float64	µSecPerSec64		= 1000000.0;

}	// anonymous namespace

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	void RegisterCMath( asIScriptEngine* const engine ) {
	//	--- TRIG FUNCTIONS ---
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Cosine( float )", asFUNCTIONPR( Cosine, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Cosine( double )", asFUNCTIONPR( Cosine, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Sine( float )", asFUNCTIONPR( Sine, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Sine( double )", asFUNCTIONPR( Sine, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Tangent( float )", asFUNCTIONPR( Tangent, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Tangent( double )", asFUNCTIONPR( Tangent, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "void SineCosine( float, float&out, float&out )", asFUNCTIONPR( SineCosine, (float32, float32&, float32&), void ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "void SineCosine( double, double&out, double&out )", asFUNCTIONPR( SineCosine, (float64, float64&, float64&), void ), asCALL_CDECL ) );

	//	--- INVERSE TRIG FUNCTIONS ---
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float ArcCosine( float )", asFUNCTIONPR( acosf, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double ArcCosine( double )", asFUNCTIONPR( acos, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float ArcSine( float )", asFUNCTIONPR( asinf, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double ArcSine( double )", asFUNCTIONPR( asin, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float ArcTangent( float )", asFUNCTIONPR( atanf, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double ArcTangent( double )", asFUNCTIONPR( atan, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float ArcTangent2( float, float )", asFUNCTIONPR( atan2f, (float32, float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double ArcTangent2( double, double )", asFUNCTIONPR( atan2, (float64, float64), float64 ), asCALL_CDECL ) );

	//	--- HYPERBOLIC TRIG FUNCTIONS ---
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float CosineHyperbolic( float )", asFUNCTIONPR( coshf, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double CosineHyperbolic( double )", asFUNCTIONPR( cosh, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float SineHyperbolic( float )", asFUNCTIONPR( sinhf, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double SineHyperbolic( double )", asFUNCTIONPR( sinh, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float TangentHyperbolic( float )", asFUNCTIONPR( tanhf, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double TangentHyperbolic( double )", asFUNCTIONPR( tanh, (float64), float64 ), asCALL_CDECL ) );

	//	--- EXPONENTIAL/LOGARITHMIC FUNCTIONS ---
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float NaturalLog( float )", asFUNCTIONPR( logf, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double NaturalLog( double )", asFUNCTIONPR( log, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float LogBase10( float )", asFUNCTIONPR( log10f, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double LogBase10( double )", asFUNCTIONPR( log10, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Exp( float )", asFUNCTIONPR( expf, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Exp( double )", asFUNCTIONPR( exp, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Exp2( float )", asFUNCTIONPR( Exp2, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Exp2( double )", asFUNCTIONPR( Exp2, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float LDExp( float, int )", asFUNCTIONPR( ldexpf, (float32, int), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double LDExp( double, int )", asFUNCTIONPR( ldexp, (float64, int), float64 ), asCALL_CDECL ) );

	//	--- POWER FUNCTIONS ---
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Pow( float, float )", asFUNCTIONPR( Pow, (float32, float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Pow( double, double )", asFUNCTIONPR( Pow, (float64, float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float SquareRoot( float )", asFUNCTIONPR( Sqrt, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double SquareRoot( double )", asFUNCTIONPR( Sqrt, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Reciprocal( float )", asFUNCTIONPR( Reciprocal, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Reciprocal( double )", asFUNCTIONPR( Reciprocal, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float ReciprocalSquareRoot( float )", asFUNCTIONPR( ReciprocalSqrt, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double ReciprocalSquareRoot( double )", asFUNCTIONPR( ReciprocalSqrt, (float64), float64 ), asCALL_CDECL ) );

	//	--- MISC. FUNCTIONS ---
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Ceil( float )", asFUNCTIONPR( Ceil, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Ceil( double )", asFUNCTIONPR( Ceil, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Floor( float )", asFUNCTIONPR( Floor, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Floor( double )", asFUNCTIONPR( Floor, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Frac( float )", asFUNCTIONPR( Frac, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Frac( double )", asFUNCTIONPR( Frac, (float64), float64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float AbsoluteValue( float )", asFUNCTIONPR( AbsoluteValue, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double AbsoluteValue( double )", asFUNCTIONPR( AbsoluteValue, (float64), float64 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int8 AbsoluteValue( int8 )", asFUNCTIONPR( AbsoluteValue, (int8), int8 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int16 AbsoluteValue( int16 )", asFUNCTIONPR( AbsoluteValue, (int16), int16 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int32 AbsoluteValue( int32 )", asFUNCTIONPR( AbsoluteValue, (int32), int32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int64 AbsoluteValue( int64 )", asFUNCTIONPR( AbsoluteValue, (int64), int64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Sign( float )", asFUNCTIONPR( Sign, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Sign( double )", asFUNCTIONPR( Sign, (float64), float64 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int8 Sign( int8 )", asFUNCTIONPR( Sign, (int8), int8 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int16 Sign( int16 )", asFUNCTIONPR( Sign, (int16), int16 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int32 Sign( int32 )", asFUNCTIONPR( Sign, (int32), int32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int64 Sign( int64 )", asFUNCTIONPR( Sign, (int64), int64 ), asCALL_CDECL ) );
	}

// ---------------------------------------------------

	void RegisterAlgorithms( asIScriptEngine* const engine ) {
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Max( float, float )", asFUNCTIONPR( Max<float32>, (const float32, const float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Max( double, double )", asFUNCTIONPR( Max<float64>, (const float64, const float64), float64 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int8 Max( int8, int8 )", asFUNCTIONPR( Max<int8>, (const int8, const int8), int8 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "uint8 Max( uint8, uint8 )", asFUNCTIONPR( Max<uint8>, (const uint8, const uint8), uint8 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int16 Max( int16, int16 )", asFUNCTIONPR( Max<int16>, (const int16, const int16), int16 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "uint16 Max( uint16, uint16 )", asFUNCTIONPR( Max<uint16>, (const uint16, const uint16), uint16 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int32 Max( int32, int32 )", asFUNCTIONPR( Max<int32>, (const int32, const int32), int32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "uint32 Max( uint32, uint32 )", asFUNCTIONPR( Max<uint32>, (const uint32, const uint32), uint32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int64 Max( int64, int64 )", asFUNCTIONPR( Max<int64>, (const int64, const int64), int64 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "uint64 Max( uint64, uint64 )", asFUNCTIONPR( Max<uint64>, (const uint64, const uint64), uint64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Min( float, float )", asFUNCTIONPR( Min<float32>, (const float32, const float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Min( double, double )", asFUNCTIONPR( Min<float64>, (const float64, const float64), float64 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int8 Min( int8, int8 )", asFUNCTIONPR( Min<int8>, (const int8, const int8), int8 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "uint8 Min( uint8, uint8 )", asFUNCTIONPR( Min<uint8>, (const uint8, const uint8), uint8 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int16 Min( int16, int16 )", asFUNCTIONPR( Min<int16>, (const int16, const int16), int16 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "uint16 Min( uint16, uint16 )", asFUNCTIONPR( Min<uint16>, (const uint16, const uint16), uint16 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int32 Min( int32, int32 )", asFUNCTIONPR( Min<int32>, (const int32, const int32), int32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "uint32 Min( uint32, uint32 )", asFUNCTIONPR( Min<uint32>, (const uint32, const uint32), uint32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int64 Min( int64, int64 )", asFUNCTIONPR( Min<int64>, (const int64, const int64), int64 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "uint64 Min( uint64, uint64 )", asFUNCTIONPR( Min<uint64>, (const uint64, const uint64), uint64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Clamp( float, float, float )", asFUNCTIONPR( Clamp<float32>, (const float32, const float32, const float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Clamp( double, double, double )", asFUNCTIONPR( Clamp<float64>, (const float64, const float64, const float64), float64 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int8 Clamp( int8, int8, int8 )", asFUNCTIONPR( Clamp<int8>, (const int8, const int8, const int8), int8 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "uint8 Clamp( uint8, uint8, uint8 )", asFUNCTIONPR( Clamp<uint8>, (const uint8, const uint8, const uint8), uint8 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int16 Clamp( int16, int16, int16 )", asFUNCTIONPR( Clamp<int16>, (const int16, const int16, const int16), int16 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "uint16 Clamp( uint16, uint16, uint16 )", asFUNCTIONPR( Clamp<uint16>, (const uint16, const uint16, const uint16), uint16 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int32 Clamp( int32, int32, int32 )", asFUNCTIONPR( Clamp<int32>, (const int32, const int32, const int32), int32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "uint32 Clamp( uint32, uint32, uint32 )", asFUNCTIONPR( Clamp<uint32>, (const uint32, const uint32, const uint32), uint32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "int64 Clamp( int64, int64, int64 )", asFUNCTIONPR( Clamp<int64>, (const int64, const int64, const int64), int64 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "uint64 Clamp( uint64, uint64, uint64 )", asFUNCTIONPR( Clamp<uint64>, (const uint64, const uint64, const uint64), uint64 ), asCALL_CDECL ) );

		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "float Saturate( float )", asFUNCTIONPR( Saturate, (float32), float32 ), asCALL_CDECL ) );
		ETRuntimeVerification( 0 <= engine->RegisterGlobalFunction( "double Saturate( double )", asFUNCTIONPR( Saturate, (float64), float64 ), asCALL_CDECL ) );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2