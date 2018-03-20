/*==================================================================*\
  Angle.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/FloatTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	class Angle {
	// - CONSTRUCTOR/DESTRUCTOR -------------------------- 

	public:
	//!	Constructs this @ref RadianAngle instance.
		explicit Angle( float32 angle );
	//!	Constructs this @ref RadianAngle instance.
		Angle( const Angle& ) = default;
	//!	Constructs this @ref RadianAngle instance.
		Angle() = default;

		~Angle() = default;

	// ---------------------------------------------------

	public:
		operator	float32() const;

	// ---------------------------------------------------

	public:
		friend ETPureFunctionHint Angle	operator*( Angle left, float32 scalar );

		friend ETPureFunctionHint Angle	operator/( Angle left, float32 scalar );

		Angle&							operator*=( float32 scalar );

		Angle&							operator/=( float32 scalar );

	// ---------------------------------------------------

	public:
		Angle&	operator=( float32 angle );
		Angle&	operator=( const Angle& ) = default;

	// ---------------------------------------------------

	private:
		float32	_angle;
	};

// ---

	ETPureFunctionHint Angle	AngleFromDegrees( float32 angle );

	ETPureFunctionHint float32	DegreesFromAngle( Angle angle );

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Mpl/Angle.inl>
//------------------------------------------------------------------//