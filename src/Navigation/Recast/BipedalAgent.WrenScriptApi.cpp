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
#include <Navigation/Recast/BipedalAgent.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Navigation { namespace Recast {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Animation;

	ET_IMPLEMENT_WREN_CLASS(BipedalAgent) {
		api.DefineClass<BipedalAgent>(ET_BUILTIN_WREN_MODULE_NAME(Navigation), "BipedalAgent", // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<BipedalAgent>(vm, /*classSlot =*/0, GetSlotAs<Armature>(vm, 1));
				}) },
			{ /* Methods */
				ForeignMethod("navigateTo(_,_)", [](WrenVM* vm) ETNoexceptHint {
					ET_ABORT_WREN_UNLESS(GetSlotAs<BipedalAgent>(vm, 0).TryBeginNavigation(GetSlotAs<NavigationScene>(vm, 1), GetSlotAs<Vector>(vm, 2)), "Error adding navigation agent to scene.");
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Navigation::Recast
