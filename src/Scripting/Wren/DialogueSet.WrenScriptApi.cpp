/*==================================================================*\
  DialogueSet.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/DialogueSetAsset.hpp>
#include <Scripting/Wren/DialogueSet.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Assets/AssetLocator.hpp>
//------------------------------------------------------------------//

WrenHandle* wrenGetSlotHandle(WrenVM* vm, int slot);
void        wrenReleaseHandle(WrenVM* vm, WrenHandle* handle);

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Scripting::Wren::AssetViews;
	using namespace ::Eldritch2::Assets;

	ET_IMPLEMENT_WREN_CLASS(DialogueSet) {
		// clang-format off
		api.CreateClass<DialogueSet>(ET_BUILTIN_WREN_MODULE_NAME(Core), "DialogueSet",
			{/* Constructors */
				ConstructorMethod("new(_)", [](WrenVM* vm) {
					const DialogueSetAsset* const	asset(Cast<DialogueSetAsset>(GetSlotAs<AssetReference>(vm, 1)));

					ET_ABORT_WREN_UNLESS(asset, "Asset must be a DialogueSetAsset.");

					SetReturn<DialogueSet>(vm);
				})
			},
			{/*	Static methods */ },
			{/*	Properties */ },
			{/*	Methods */
				ForeignMethod("match(_,_)", [](WrenVM* vm) {
					WrenHandle* const	target(wrenGetSlotHandle(vm, 1));
					WrenHandle* const	facts(wrenGetSlotHandle(vm, 2));

					GetSlotAs<DialogueSet>(vm, 0).Match(facts);

					wrenReleaseHandle(vm, facts);
					wrenReleaseHandle(vm, target);
				})
			}
		); // clang-format on
	}

}}} // namespace Eldritch2::Scripting::Wren
