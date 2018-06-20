/*==================================================================*\
  BipedalAgent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/NavigationScene.hpp>
#include <Navigation/Recast/BipedalAgent.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Navigation {
		namespace Recast {

			using namespace ::Eldritch2::Scripting::Wren;
			using namespace ::Eldritch2::Scripting;
			using namespace ::Eldritch2::Animation;

			ET_IMPLEMENT_WREN_CLASS(BipedalAgent) {
				api.CreateClass<BipedalAgent>(ET_BUILTIN_WREN_MODULE_NAME(Navigation), "BipedalAgent",
											  {/* Constructors */
												  ConstructorMethod("new(_)", [](WrenVM* vm) {
													  SetReturn<BipedalAgent>(vm, GetSlotAs<Armature>(vm, 1));
												  })
											  },
												  {/*	Static methods */ },
												  {/*	Properties */ },
			{/*	Methods */
				ForeignMethod("navigateTo(_,_)", [](WrenVM* vm) {
					NavigationScene&	scene(GetSlotAs<NavigationScene>(vm, 1));
					const Vector&		target(GetSlotAs<Vector>(vm, 2));

					ET_ABORT_WREN_UNLESS(GetSlotAs<BipedalAgent>(vm, 0).TryBeginNavigation(scene, target), "Error adding navigation agent to scene.");
				})
			}
			);
			}

		}	// namespace Recast
	}	// namespace Navigation
}	// namespace Eldritch2