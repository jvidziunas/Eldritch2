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

namespace Eldritch2 {

ETConstexpr ETForceInlineHint Angle::Angle(float32 radians) ETNoexceptHint : _radians(radians) {}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint Angle::operator float32() const ETNoexceptHint {
	return _radians;
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint Angle& Angle::operator*=(float32 rhs) ETNoexceptHint {
	return (_radians *= rhs, *this);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint Angle& Angle::operator/=(float32 rhs) ETNoexceptHint {
	return (_radians /= rhs, *this);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint Angle& Angle::operator+=(Angle rhs) ETNoexceptHint {
	return (_radians += float32(rhs), *this);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint Angle& Angle::operator-=(Angle rhs) ETNoexceptHint {
	return (_radians -= float32(rhs), *this);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint Angle& Angle::operator=(float32 radians) ETNoexceptHint {
	return (_radians = radians, *this);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint Angle operator*(Angle lhs, float32 rhs)ETNoexceptHint {
	return lhs *= rhs;
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint Angle operator/(Angle lhs, float32 rhs) ETNoexceptHint {
	return lhs /= rhs;
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint Angle operator+(Angle lhs, Angle rhs) ETNoexceptHint {
	return lhs += rhs;
}
// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint Angle operator-(Angle lhs, Angle rhs) ETNoexceptHint {
	return lhs -= rhs;
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint Angle operator-(Angle angle) ETNoexceptHint {
	return Angle(-float32(angle));
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint Angle AsDegreeAngle(float32 degrees) ETNoexceptHint {
	return Angle(degrees * (3.14159265358972f / 180.0f));
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint float32 AsDegrees(Angle angle) ETNoexceptHint {
	return float32(angle) * (180.0f / 3.14159265358972f);
}

} // namespace Eldritch2
