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
#include <Assets/PackageManager.hpp>
#include <Assets/AssetManager.hpp>
#include <Scripting/Wren/Game.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {
			ET_DECLARE_WREN_CLASS(DialogueSet);
			ET_DECLARE_WREN_CLASS(Dispatcher);
			ET_DECLARE_WREN_CLASS(GameObject);
			ET_DECLARE_WREN_CLASS(ActionSet);
			ET_DECLARE_WREN_CLASS(Planner);

			ET_DECLARE_WREN_CLASS(Transformation);
			ET_DECLARE_WREN_CLASS(Quaternion);
			ET_DECLARE_WREN_CLASS(Vector);
		}
	}

	namespace Assets {
		ET_DECLARE_WREN_CLASS(AsynchronousImport);
		ET_DECLARE_WREN_CLASS(PackageManager);
		ET_DECLARE_WREN_CLASS(AssetManager);
		ET_DECLARE_WREN_CLASS(Asset);
	}
}

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {

			using namespace ::Eldritch2::Assets;
			using namespace ::Eldritch2::Core;

			void WrenWorldComponent::AcceptVisitor(ApiBuilder& api) {
				ET_REGISTER_WREN_CLASS(Transformation, api);
				ET_REGISTER_WREN_CLASS(Quaternion, api);
				ET_REGISTER_WREN_CLASS(Vector, api);

				ET_REGISTER_WREN_CLASS(AsynchronousImport, api);
				ET_REGISTER_WREN_CLASS(PackageManager, api);
				ET_REGISTER_WREN_CLASS(AssetManager, api);
				ET_REGISTER_WREN_CLASS(DialogueSet, api);
				ET_REGISTER_WREN_CLASS(Dispatcher, api);
				ET_REGISTER_WREN_CLASS(GameObject, api);
				ET_REGISTER_WREN_CLASS(ActionSet, api);
				ET_REGISTER_WREN_CLASS(Planner, api);
				ET_REGISTER_WREN_CLASS(Asset, api);

				_packages = api.CreateVariable<PackageManager>(ET_BUILTIN_WREN_MODULE_NAME(Core), "Packages", FindService<PackageDatabase>());
				_assets = api.CreateVariable<AssetManager>(ET_BUILTIN_WREN_MODULE_NAME(Core), "Assets", FindService<AssetDatabase>());
				_dispatcher = api.CreateVariable<Dispatcher>(ET_BUILTIN_WREN_MODULE_NAME(Core), "Dispatcher", FindService<World>());
			}

		}	// namespace Wren
	}	// namespace Scripting
}	// namespace Eldritch2