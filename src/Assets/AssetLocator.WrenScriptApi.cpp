/*==================================================================*\
  AssetLocator.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Assets/AssetLocator.hpp>
//------------------------------------------------------------------//

const char* wrenGetSlotString(WrenVM* vm, int slot);

namespace Eldritch2 { namespace Assets {

	using namespace ::Eldritch2::Scripting::Wren;

	ET_IMPLEMENT_WREN_CLASS(Asset) { // clang-format off
		api.CreateClass<AssetReference>(ET_BUILTIN_WREN_MODULE_NAME(Core), "AssetReference",
			{ /* Constructors */
				ConstructorMethod("require(_,_)", [](WrenVM* vm) {
					const Utf8Char* const path(wrenGetSlotString(vm, 2));
					const Asset* const    asset(GetSlotAs<AssetLocator>(vm, 1).Find(path));
					ET_ABORT_WREN_UNLESS_FMT(asset, "Asset '{}' not loaded.", path);

					SetReturn<AssetReference>(vm, *asset);
				}) },
			{ /* Static methods */ },
			{ /* Properties */ },
			{ /* Methods */ });
	} // clang-format on

	ET_IMPLEMENT_WREN_CLASS(AssetLocator) { // clang-format off
		api.CreateClass<AssetLocator>(ET_BUILTIN_WREN_MODULE_NAME(Core), "AssetLocator",
			{ /* Constructors */ },
			{ /* Static methods */ },
			{ /* Properties */ },
			{ /* Methods */ });
	} // clang-format on

}} // namespace Eldritch2::Assets
