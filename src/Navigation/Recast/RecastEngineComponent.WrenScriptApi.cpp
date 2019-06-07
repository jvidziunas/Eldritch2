/*==================================================================*\
  RecastEngineComponent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/RecastEngineComponent.hpp>
#include <Navigation/Recast/NavigationScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Navigation { namespace Recast {
	ET_DECLARE_WREN_CLASS(NavigationScene);
	ET_DECLARE_WREN_CLASS(BipedalAgent);
}}} // namespace Eldritch2::Navigation::Recast

namespace Eldritch2 { namespace Navigation { namespace Recast {

	using namespace ::Eldritch2::Scripting::Wren;

	void RecastEngineComponent::WorldComponent::PublishApi(ApiBuilder& api) {
		ET_REGISTER_WREN_CLASS(NavigationScene, api);
		ET_REGISTER_WREN_CLASS(BipedalAgent, api);

		_scene = api.DefineVariable<NavigationScene>(ET_BUILTIN_WREN_MODULE_NAME(Navigation), "NavigationScene");
	}

}}} // namespace Eldritch2::Navigation::Recast
