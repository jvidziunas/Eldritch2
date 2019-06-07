/*==================================================================*\
  Platform.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
template <typename T>
ETConstexpr ETPureFunctionHint ETNeverThrowsHint T Clamp(T value, T min, T max);
} // namespace Eldritch2

namespace Eldritch2 {

ETConstexpr ETForceInlineHint ETPureFunctionHint float32 AsMilliseconds(MicrosecondTime microseconds) ETNoexceptHint {
	return float32((uint64(microseconds))) / /*microseconds per millisecond*/ 1000.0f;
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint MicrosecondTime MicrosecondsPerFrame(uint32 framerateInHz) ETNoexceptHint {
	return MicrosecondTime(MicrosecondsPerSecond / Clamp(framerateInHz, 1u, uint32(MicrosecondsPerSecond)));
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint MicrosecondTime operator+(MicrosecondTime lhs, MicrosecondTime rhs) ETNoexceptHint {
	return MicrosecondTime(uint64(lhs) + uint64(rhs));
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint MicrosecondTime operator-(MicrosecondTime lhs, MicrosecondTime rhs) ETNoexceptHint {
	return MicrosecondTime(uint64(lhs) + uint64(rhs));
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint MicrosecondTime& operator+=(MicrosecondTime& lhs, MicrosecondTime rhs) ETNoexceptHint {
	return lhs = (lhs + rhs);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint MicrosecondTime& operator-=(MicrosecondTime& lhs, MicrosecondTime rhs) ETNoexceptHint {
	return lhs = (lhs - rhs);
}

} // namespace Eldritch2
