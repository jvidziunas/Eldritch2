/*==================================================================*\
  WrenWorldComponent.WrenScriptApi.cpp
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
#include <Scripting/Wren/WrenEngineComponent.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Dispatcher.hpp>
#include <Scripting/Wren/Game.hpp>
#include <Core/ContentLocator.hpp>
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

namespace Core {
	ET_DECLARE_WREN_CLASS(AsynchronousImport);
	ET_DECLARE_WREN_CLASS(ContentLocator);
	ET_DECLARE_WREN_CLASS(AssetReference);
} // namespace Core
} // namespace Eldritch2

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	void WrenEngineComponent::WorldComponent::PublishApi(ApiBuilder& api) {
		static ETConstexpr StringSpan CoreModule(ET_BUILTIN_WREN_MODULE_NAME(Core));

		ET_REGISTER_WREN_CLASS(Transformation, api, CoreModule);
		ET_REGISTER_WREN_CLASS(Quaternion, api, CoreModule);
		ET_REGISTER_WREN_CLASS(Vector, api, CoreModule);

		ET_REGISTER_WREN_CLASS(AsynchronousImport, api, CoreModule);
		ET_REGISTER_WREN_CLASS(ActionSetClient, api, CoreModule);
		ET_REGISTER_WREN_CLASS(AssetReference, api, CoreModule);
		ET_REGISTER_WREN_CLASS(ContentLocator, api, CoreModule);
		ET_REGISTER_WREN_CLASS(DialogueSet, api, CoreModule);
		ET_REGISTER_WREN_CLASS(Dispatcher, api, CoreModule);
		ET_REGISTER_WREN_CLASS(GameObject, api, CoreModule);
		ET_REGISTER_WREN_CLASS(Planner, api, CoreModule);
		ET_REGISTER_WREN_CLASS(Game, api, CoreModule);

		_content       = api.DefineVariable<ContentLocator>("ContentLocator", CoreModule, *Inject<AssetDatabase>(), *Inject<ContentDatabase>());
		_worldPackages = api.DefineVariable<AsynchronousImport>("WorldPackages", CoreModule);
		_dispatcher    = api.DefineVariable<Dispatcher>("Dispatcher", CoreModule);
		_game          = api.DefineVariable<Game>("Game", CoreModule);
	}

}}} // namespace Eldritch2::Scripting::Wren
