/*==================================================================*\
  Game.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Game.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	ET_IMPLEMENT_WREN_CLASS(Game) {
		api.DefineClass<Game>(ET_BUILTIN_WREN_MODULE_NAME(Core), "GameClass", { /* Static methods */ }, { /* Methods */ });
	}

}}} // namespace Eldritch2::Scripting::Wren
