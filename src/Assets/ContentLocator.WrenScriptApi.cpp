/*==================================================================*\
  ContentLocator.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Marshal.hpp>
#include <Assets/ContentLocator.hpp>
//------------------------------------------------------------------//

void wrenGetListElement(WrenVM* vm, int listSlot, int index, int elementSlot);
void wrenSetSlotBool(WrenVM* vm, int slot, bool value);
void wrenEnsureSlots(WrenVM* vm, int numSlots);
int  wrenGetListCount(WrenVM* vm, int slot);

namespace Eldritch2 { namespace Assets {

	using namespace ::Eldritch2::Scripting::Wren;

	ET_IMPLEMENT_WREN_CLASS(AssetReference) {
		api.DefineClass<AssetReference>(ET_BUILTIN_WREN_MODULE_NAME(Core), "AssetReference", // clang-format off
			{ /* Static methods */
				ForeignMethod("require(_,_)", [](WrenVM* vm) ETNoexceptHint {
					const auto path(GetSlotStringView(vm, 2));
					const auto asset(GetSlotAs<ContentLocator>(vm, 1).FindAsset(path));
					ET_ABORT_WREN_UNLESS_FMT(asset, "Asset '{}' not loaded.", path);

					SetReturn<AssetReference>(vm, *asset);
				}), },
			{ /* Methods */ }); // clang-format on
	}

	ET_IMPLEMENT_WREN_CLASS(AsynchronousImport) {
		api.DefineClass<AsynchronousImport>(ET_BUILTIN_WREN_MODULE_NAME(Core), "AsynchronousImport", // clang-format off
			{ /* Static methods */
				ForeignMethod("of(_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenEnsureSlots(vm, 4);
					
					auto& locator(GetSlotAs<ContentLocator>(vm, 1));
					auto& import(SetReturn<AsynchronousImport>(vm));
					for (int element(0), count(wrenGetListCount(vm, 2)); element < count; ++element) {
						wrenGetListElement(vm, 2, element, 3);
						const StringView path(GetSlotStringView(vm, 3));
						const ErrorCode  result(locator.RequirePackageAsynchronous(path, import));
						ET_ABORT_WREN_UNLESS_FMT(Succeeded(result), "Unable to load package {}: {}.", path, result);
					}
				}), },
			{ /* Methods */
				ForeignMethod("isComplete", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotBool(vm, 0, GetSlotAs<AsynchronousImport>(vm, 0).IsComplete());
				}), }); // clang-format on
	}

	ET_IMPLEMENT_WREN_CLASS(ContentLocator) {
		api.DefineClass<ContentLocator>(ET_BUILTIN_WREN_MODULE_NAME(Core), "ContentLocatorClass", // clang-format off
			{ /* Static methods */ },
			{ /* Methods */
				ForeignMethod("require(_)", [](WrenVM* vm) ETNoexceptHint {
			        ContentLocator&  locator(GetSlotAs<ContentLocator>(vm, 0));
					const StringView path(GetSlotStringView(vm, 1));
					const ErrorCode  result(locator.RequirePackageAsynchronous(path, locator));
					ET_ABORT_WREN_UNLESS_FMT(Succeeded(result), "Unable to load package {}: {}.", path, result);
				}), }); // clang-format on
	}

}} // namespace Eldritch2::Assets
