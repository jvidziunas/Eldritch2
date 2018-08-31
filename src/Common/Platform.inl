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

ETConstexpr ETPureFunctionHint float32 AsMilliseconds(MicrosecondTime microseconds) ETNoexceptHint {
	return AsFloat(microseconds) / /*microseconds per millisecond*/ 1000.0f;
}

// ---------------------------------------------------

ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint float32 AsSeconds(MicrosecondTime microseconds) ETNoexceptHint {
	return AsFloat(microseconds) / /*microseconds per second*/ 1000000.0f;
}

} // namespace Eldritch2
