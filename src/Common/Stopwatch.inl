/*==================================================================*\
  Stopwatch.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	ETInlineHint ETForceInlineHint float32 Stopwatch::GetDurationMillisecondsAndZero() ETNoexceptHint {
		return AsMilliseconds(this->GetDurationAndZero());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint float32 Stopwatch::GetDurationMilliseconds() const ETNoexceptHint {
		return AsMilliseconds(this->GetDuration());
	}

}	// namespace Eldritch2