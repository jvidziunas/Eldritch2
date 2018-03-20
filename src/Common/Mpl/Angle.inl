/*==================================================================*\
  Angle.inl
  ------------------------------------------------------------------
  Purpose:
  Definitions for helper classes that encapsulate angles measured in
  both degrees and radians, using the built-in C++ type system to
  convert between the two automatically.

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

//	Fake anonymous/static functions; these will be cleaned up at the end of this file.
#if !defined( RAD2DEG )
#	define RAD2DEG(x) ((x) * 180.0f / 3.14159265358972f )
#	define RAD2DEGCLEANUP
#endif

#if !defined( DEG2RAD )
#	define DEG2RAD(x) ((x) * 3.14159265358972f / 180.0f )
#	define DEG2RADCLEANUP
#endif

namespace Eldritch2 {

	ETInlineHint Angle::Angle( float32 angle ) : _angle( angle ) {}

// ---------------------------------------------------

	ETInlineHint Angle::operator float32() const {
		return _angle;
	}

// ---------------------------------------------------

	ETInlineHint Angle& Angle::operator *=( float32 scalar ) {
		_angle *= scalar;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Angle& Angle::operator /=( float32 scalar ) {
		_angle /= scalar;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Angle operator*( Angle measure, float32 scalar ) {
		return measure *= scalar;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Angle operator/( Angle measure, float32 scalar ) {
		return measure /= scalar;
	}

// ---------------------------------------------------

	ETInlineHint Angle& Angle::operator=( float32 angle ) {
		_angle = angle;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Angle AngleFromDegrees( float32 angle ) {
		return Angle( DEG2RAD( angle ) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint float32	DegreesFromAngle( Angle angle ) {
		return RAD2DEG( static_cast<float32>(angle) );
	}

}	// namespace Eldritch2

#if defined( RAD2DEGCLEANUP )
#	undef RAD2DEG
#	undef RAD2DEGCLEANUP
#endif

#if defined( DEG2RADCLEANUP )
#	undef DEG2RAD
#	undef DEG2RADCLEANUP
#endif