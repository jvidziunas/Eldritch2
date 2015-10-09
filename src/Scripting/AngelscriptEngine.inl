/*==================================================================*\
  AngelscriptEngine.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
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

	ETInlineHint ::asIScriptEngine& AngelscriptEngine::GetScriptEngine() const {
		return *_scriptEngine;
	}

}	// namespace Scripting
}	// namespace Eldritch2