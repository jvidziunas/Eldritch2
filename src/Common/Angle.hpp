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
	//!	Constructs this @ref Angle instance.
	explicit ETConstexpr Angle(float32 radians) ETNoexceptHint;
	//!	Constructs this @ref Angle instance.
	ETConstexpr Angle(const Angle&) ETNoexceptHint = default;
	//!	Constructs this @ref Angle instance.
	Angle() ETNoexceptHint = default;

	~Angle() = default;

	// ---------------------------------------------------

public:
	ETConstexpr explicit operator float32() const ETNoexceptHint;

	ETConstexpr Angle& operator*=(float32) ETNoexceptHint;
	ETConstexpr Angle& operator/=(float32) ETNoexceptHint;
	ETConstexpr Angle& operator+=(Angle) ETNoexceptHint;
	ETConstexpr Angle& operator-=(Angle) ETNoexceptHint;
	ETConstexpr Angle& operator=(float32) ETNoexceptHint;
	ETConstexpr Angle& operator=(const Angle&) ETNoexceptHint = default;

	// - DATA MEMBERS ------------------------------------

private:
	float32 _radians;

	// ---------------------------------------------------

	friend ETConstexpr ETPureFunctionHint Angle operator*(Angle, float32)ETNoexceptHint;
	friend ETConstexpr ETPureFunctionHint Angle operator/(Angle, float32) ETNoexceptHint;
	friend ETConstexpr ETPureFunctionHint Angle operator+(Angle, Angle) ETNoexceptHint;
	friend ETConstexpr ETPureFunctionHint Angle operator-(Angle, Angle) ETNoexceptHint;
	friend ETConstexpr ETPureFunctionHint Angle operator-(Angle) ETNoexceptHint;
};

// ---

ETConstexpr ETPureFunctionHint Angle AsDegreeAngle(float32 angle) ETNoexceptHint;

ETConstexpr ETPureFunctionHint float32 AsDegrees(Angle angle) ETNoexceptHint;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Mpl/Angle.inl>
//------------------------------------------------------------------//
