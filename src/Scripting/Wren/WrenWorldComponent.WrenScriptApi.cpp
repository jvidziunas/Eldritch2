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
#include <Assets/PackageLocator.hpp>
#include <Assets/AssetLocator.hpp>
#include <Scripting/Wren/Game.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting { namespace Wren {
	ET_DECLARE_WREN_CLASS(DialogueSet);
	ET_DECLARE_WREN_CLASS(Dispatcher);
	ET_DECLARE_WREN_CLASS(GameObject);
	ET_DECLARE_WREN_CLASS(ActionSet);
	ET_DECLARE_WREN_CLASS(Planner);

	ET_DECLARE_WREN_CLASS(Transformation);
	ET_DECLARE_WREN_CLASS(Quaternion);
	ET_DECLARE_WREN_CLASS(Vector);
}} // namespace Scripting::Wren

namespace Assets {
	ET_DECLARE_WREN_CLASS(AsynchronousImport);
	ET_DECLARE_WREN_CLASS(PackageLocator);
	ET_DECLARE_WREN_CLASS(AssetLocator);
	ET_DECLARE_WREN_CLASS(Asset);
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
		ET_REGISTER_WREN_CLASS(PackageLocator, api);
		ET_REGISTER_WREN_CLASS(AssetLocator, api);
		ET_REGISTER_WREN_CLASS(DialogueSet, api);
		ET_REGISTER_WREN_CLASS(Dispatcher, api);
		ET_REGISTER_WREN_CLASS(GameObject, api);
		ET_REGISTER_WREN_CLASS(ActionSet, api);
		ET_REGISTER_WREN_CLASS(Planner, api);
		ET_REGISTER_WREN_CLASS(Asset, api);

		_packageLocator = api.CreateVariable<PackageLocator>(ET_BUILTIN_WREN_MODULE_NAME(Core), "Packages", FindService<PackageDatabase>());
		_assetLocator   = api.CreateVariable<AssetLocator>(ET_BUILTIN_WREN_MODULE_NAME(Core), "Assets", FindService<AssetDatabase>());
		_dispatcher     = api.CreateVariable<Dispatcher>(ET_BUILTIN_WREN_MODULE_NAME(Core), "Dispatcher", FindService<World>());
	}

}}} // namespace Eldritch2::Scripting::Wren
