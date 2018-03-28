/*==================================================================*\
  Matrices.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Matrices.hpp>
#include <Common/Angle.hpp>
#include <Common/Math.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ETPureFunctionHint Matrix4x4 ETSimdCall Matrix4x4::AsPerspectiveProjection( Angle verticalFov, float32 aspect, float32 nearPlane, float32 farPlane ) {
		return AsPerspectiveProjection( Angle( verticalFov * aspect ), verticalFov, nearPlane, farPlane );
	}

// ---------------------------------------------------

	ETPureFunctionHint Matrix4x4 ETSimdCall Matrix4x4::AsPerspectiveProjection( Angle horizontalFov, Angle verticalFov, float32 nearPlane, float32 farPlane ) {
	//	Restrict far - near for numerical stability.
		const float32	depthRange( farPlane / Max( farPlane - nearPlane, FLT_EPSILON ) );

		return Matrix4x4{
			Reciprocal( Tangent( horizontalFov * 0.5f ) ), 0.0f,                                                 0.0f,                   0.0f,
			0.0f,                                                  Reciprocal( Tangent( verticalFov * 0.5f ) ),  0.0f,                   0.0f,
			0.0f,                                                  0.0f,                                         depthRange,             1.0f,
			0.0f,                                                  0.0f,                                        -depthRange * nearPlane, 0.0f
		};
	}

// ---------------------------------------------------

	ETPureFunctionHint Matrix4x4 ETSimdCall Matrix4x4::AsRotation( Quaternion orientation ) {
		float32	coefficients[4];

		orientation.ExtractCoefficients( coefficients );

	//	TODO: MAKE THIS USE SHUFFLE INTRINSICS
		return Matrix4x4{
			Vector(  coefficients[3],  coefficients[2], -coefficients[1], -coefficients[0] ),
			Vector( -coefficients[2],  coefficients[3],  coefficients[0], -coefficients[1] ),
			Vector(  coefficients[1], -coefficients[0],  coefficients[3], -coefficients[2] ),
			Vector(  coefficients[0],  coefficients[1],  coefficients[2],  coefficients[3] )
		} * Matrix4x4{
			Vector(  coefficients[3],  coefficients[2], -coefficients[1], coefficients[0] ),
			Vector( -coefficients[2],  coefficients[3],  coefficients[0], coefficients[1] ),
			Vector(  coefficients[1], -coefficients[0],  coefficients[3], coefficients[2] ),
			Vector( -coefficients[0], -coefficients[1], -coefficients[2], coefficients[3] )
		};
	}

// ---------------------------------------------------

	ETPureFunctionHint Matrix4x4 ETSimdCall Matrix4x4::AsRigidTransformation( Vector translation, Quaternion orientation ) {
		Matrix4x4	temp( AsRotation( orientation ) );

		temp.rows[3] = translation;

		return temp;
	}

// ---------------------------------------------------

	ETPureFunctionHint Matrix4x4 ETSimdCall Matrix4x4::AsRigidTransformation( Transformation transform ) {
		return AsRigidTransformation( transform.translation, transform.rotation );
	}

// ---------------------------------------------------

	ETPureFunctionHint Matrix4x4 ETSimdCall Matrix4x4::Identity() {
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

// ---------------------------------------------------

#if ( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
//	MSVC complains about writing to uninitialized memory... thus initializing it.
#	pragma warning( disable : 6001 )
#endif
	ETPureFunctionHint Matrix4x4 ETSimdCall Matrix4x4::Zero() {
		return { _mm_setzero_ps(), _mm_setzero_ps(), _mm_setzero_ps(), _mm_setzero_ps() };
	}
#if ( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )	// 6001
#endif

}	// namespace Eldritch2