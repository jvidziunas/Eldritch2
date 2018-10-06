/*==================================================================*\
  NavigationScene.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/NavigationScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Navigation { namespace Recast {

	ET_IMPLEMENT_WREN_CLASS(NavigationScene) {
		api.DefineClass<NavigationScene>(ET_BUILTIN_WREN_MODULE_NAME(Navigation), "NavigationSceneClass", { /* Static methods */ }, { /* Methods */ });
	}

}}} // namespace Eldritch2::Navigation::Recast
