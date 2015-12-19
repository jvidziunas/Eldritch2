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

	ETInlineHint ::asIScriptEngine& EngineService::BytecodePackageViewFactory::GetScriptEngine() const {
		return *_scriptEngine.get();
	}

// ---------------------------------------------------

	ETInlineHint void EngineService::BytecodePackageViewFactory::SetScriptEngine( AngelScript::EngineHandle&& engine ) {
		_scriptEngine = ::std::move( engine );
	}

// ---------------------------------------------------

	ETInlineHint ::asIScriptEngine& EngineService::GetScriptEngine() const {
		return _bytecodePackageFactory.GetScriptEngine();
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2