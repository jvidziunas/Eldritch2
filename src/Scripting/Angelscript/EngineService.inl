/*==================================================================*\
  EngineService.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	ETInlineHint ::asIScriptEngine& EngineService::GetScriptEngine() const {
		return *_scriptEngine;
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2