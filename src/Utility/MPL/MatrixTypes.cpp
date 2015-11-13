/*==================================================================*\
  MatrixTypes.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/MPL/MatrixTypes.hpp>
#include <Utility/MPL/AngleTypes.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2 {

	Float4x4::operator Quaternion() const {
		const float32	wValue( (1.0f + elements[0].coefficients[0] + elements[1].coefficients[1] + elements[2].coefficients[2]) / 2 );
		const float32	divisor( Reciprocal( 4.0f * wValue ) );

		return { (elements[2].coefficients[1] - elements[1].coefficients[2]) * divisor,
				 (elements[2].coefficients[2] - elements[2].coefficients[0]) * divisor,
				 (elements[1].coefficients[0] - elements[0].coefficients[1]) * divisor,
				 wValue };
	}

// ---------------------------------------------------

	ETNoAliasHint Float4x4 Float4x4::Perspective( const RadianMeasure horizontalFieldOfView, const float32 aspect, const float32 near, const float32 far ) {
		return Perspective( horizontalFieldOfView, RadianMeasure( horizontalFieldOfView * aspect ), near, far );
	}

// ---------------------------------------------------

	ETNoAliasHint Float4x4 Float4x4::Perspective( const RadianMeasure horizontalFieldOfView, const RadianMeasure verticalFieldOfView, const float32 near, const float32 far ) {
		const float32	depthRange( far / (far - near) );

		return { Reciprocal( Tangent( horizontalFieldOfView * 0.5f ) ), 0.0f,													0.0f,				0.0f,
				 0.0f,													Reciprocal( Tangent( verticalFieldOfView * 0.5f ) ),	0.0f,				0.0f,
				 0.0f,													0.0f,													depthRange,			1.0f,
				 0.0f,													0.0f,													-depthRange * near,	0.0f };
	}

// ---------------------------------------------------

	ETNoAliasHint Float4x4 Float4x4::Rotation( const Quaternion orientation ) {
		// TODO: MAKE THIS USE SHUFFLE INTRINSICS
		const Float4x4	temp1 {  orientation.coefficients[3],  orientation.coefficients[2], -orientation.coefficients[1], -orientation.coefficients[0],
								-orientation.coefficients[2],  orientation.coefficients[3],  orientation.coefficients[0], -orientation.coefficients[1],
								 orientation.coefficients[1], -orientation.coefficients[0],  orientation.coefficients[3], -orientation.coefficients[2],
								 orientation.coefficients[0],  orientation.coefficients[1],  orientation.coefficients[2],  orientation.coefficients[3] };
		Float4x4		temp0 {  orientation.coefficients[3],  orientation.coefficients[2], -orientation.coefficients[1], orientation.coefficients[0],
								-orientation.coefficients[2],  orientation.coefficients[3],  orientation.coefficients[0], orientation.coefficients[1],
								 orientation.coefficients[1], -orientation.coefficients[0],  orientation.coefficients[3], orientation.coefficients[2],
								-orientation.coefficients[0], -orientation.coefficients[1], -orientation.coefficients[2], orientation.coefficients[3] };

		return temp0 *= temp1;
	}

// ---------------------------------------------------

	ETNoAliasHint Float4x4 Float4x4::RigidTransformation( const Float4 translation, const Quaternion orientation ) {
		Float4x4	temp( Rotation( orientation ) );
		temp.elements[3] = translation;
		return temp;
	}

// ---------------------------------------------------

	ETNoAliasHint Float4x4 Float4x4::Identity() {
		return { 1.0f, 0.0f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0f, 0.0f,
				 0.0f, 0.0f, 1.0f, 0.0f,
				 0.0f, 0.0f, 0.0f, 1.0f };
	}

// ---------------------------------------------------

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
	// MSVC complains about writing to uninitialized memory... thus initializing it.
#	pragma warning( disable : 6001 )
#endif
	ETNoAliasHint Float4x4 Float4x4::Zero() {
		Float4x4	temp;

		::_mm_storeu_ps( temp.elements[0].coefficients, ::_mm_setzero_ps() );
		::_mm_storeu_ps( temp.elements[1].coefficients, ::_mm_setzero_ps() );
		::_mm_storeu_ps( temp.elements[2].coefficients, ::_mm_setzero_ps() );
		::_mm_storeu_ps( temp.elements[3].coefficients, ::_mm_setzero_ps() );

		return temp;
	}
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )	// 6001
#endif

}	// namespace Eldritch2