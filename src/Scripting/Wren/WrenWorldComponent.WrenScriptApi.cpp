/*==================================================================*\
  WrenWorldComponent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenWorldComponent.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Dispatcher.hpp>
#include <Assets/PackageDatabase.hpp>
#include <Assets/ContentLocator.hpp>
#include <Scripting/Wren/Game.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting { namespace Wren {
	ET_DECLARE_WREN_CLASS(ActionSetClient);
	ET_DECLARE_WREN_CLASS(DialogueSet);
	ET_DECLARE_WREN_CLASS(Dispatcher);
	ET_DECLARE_WREN_CLASS(GameObject);
	ET_DECLARE_WREN_CLASS(Planner);
	ET_DECLARE_WREN_CLASS(Game);

	ET_DECLARE_WREN_CLASS(Transformation);
	ET_DECLARE_WREN_CLASS(Quaternion);
	ET_DECLARE_WREN_CLASS(Vector);
}} // namespace Scripting::Wren

namespace Assets {
	ET_DECLARE_WREN_CLASS(AsynchronousImport);
	ET_DECLARE_WREN_CLASS(ContentLocator);
	ET_DECLARE_WREN_CLASS(AssetReference);
} // namespace Assets
} // namespace Eldritch2

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Assets;
	using namespace ::Eldritch2::Core;

	void WrenWorldComponent::DefineScriptApi(ApiBuilder& api) {
		ET_REGISTER_WREN_CLASS(Transformation, api);
		ET_REGISTER_WREN_CLASS(Quaternion, api);
		ET_REGISTER_WREN_CLASS(Vector, api);

		ET_REGISTER_WREN_CLASS(AsynchronousImport, api);
		ET_REGISTER_WREN_CLASS(ActionSetClient, api);
		ET_REGISTER_WREN_CLASS(AssetReference, api);
		ET_REGISTER_WREN_CLASS(ContentLocator, api);
		ET_REGISTER_WREN_CLASS(DialogueSet, api);
		ET_REGISTER_WREN_CLASS(Dispatcher, api);
		ET_REGISTER_WREN_CLASS(GameObject, api);
		ET_REGISTER_WREN_CLASS(Planner, api);
		ET_REGISTER_WREN_CLASS(Game, api);

		_contentLocator   = api.DefineVariable<ContentLocator>(ET_BUILTIN_WREN_MODULE_NAME(Core), "ContentLocator");
		_requiredPackages = api.DefineVariable<AsynchronousImport>(ET_BUILTIN_WREN_MODULE_NAME(Core), "RequiredPackages");
		_dispatcher       = api.DefineVariable<Dispatcher>(ET_BUILTIN_WREN_MODULE_NAME(Core), "Dispatcher");
		_game             = api.DefineVariable<Game>(ET_BUILTIN_WREN_MODULE_NAME(Core), "Game");
	}

}}} // namespace Eldritch2::Scripting::Wren
