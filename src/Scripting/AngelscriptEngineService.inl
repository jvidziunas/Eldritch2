/*==================================================================*\
  AngelscriptEngineService.inl
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

	namespace Scripting		= ::Eldritch2::Scripting;

// ---------------------------------------------------

	ETInlineHint ::asIScriptEngine& AngelscriptEngineService::GetScriptEngine() const {
		return *_scriptEngine;
	}

}	// namespace Scripting
}	// namespace Eldritch2