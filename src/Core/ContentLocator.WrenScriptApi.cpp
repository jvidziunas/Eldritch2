/*==================================================================*\
  ContentLocator.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Marshal.hpp>
#include <Core/ContentLocator.hpp>
//------------------------------------------------------------------//

void wrenGetListElement(WrenVM* vm, int listSlot, int index, int elementSlot);
void wrenSetSlotBool(WrenVM* vm, int slot, bool value);
void wrenEnsureSlots(WrenVM* vm, int numSlots);
int  wrenGetListCount(WrenVM* vm, int slot);

namespace Eldritch2 { namespace Core {

	using namespace ::Eldritch2::Scripting::Wren;

	// ---------------------------------------------------

	ET_IMPLEMENT_WREN_CLASS(AssetReference, api, moduleName) {
		api.DefineClass<AssetReference>("AssetReference", moduleName, 
			{ /* Static methods/constructors */
				ForeignMethod("require(_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<AssetReference>(vm, wrenGetSlotAs<ContentLocator>(vm, 1).GetContent(), wrenGetSlotStringView(vm, 2));
				}), },
			{ /* Methods */ }); // clang-format on
	}

	// ---------------------------------------------------

	ET_IMPLEMENT_WREN_CLASS(AsynchronousImport, api, moduleName) {
		api.DefineClass<AsynchronousImport>("AsynchronousImport", moduleName, // clang-format off
			{ /* Static methods/constructors */
				ForeignMethod("of(_,_)", [](WrenVM* vm) ETNoexceptHint {
					auto& self(wrenSetReturn<AsynchronousImport>(vm, /*classSlot =*/0));
					auto& content(wrenGetSlotAs<ContentLocator>(vm, 1).GetContent());
					
					wrenEnsureSlots(vm, 4);
					for (int element(0), count(wrenGetListCount(vm, 2)); element < count; ++element) {
						wrenGetListElement(vm, 2, element, 3);
						self.InitiateLoad(content, wrenGetSlotStringView(vm, 3));
					}

					ET_ABORT_WREN_IF(self.HasFailed(), vm, "Unable to load required content!");
				}), },
			{ /* Methods */
				ForeignMethod("hasCompleted", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotBool(vm, 0, wrenGetReceiver<AsynchronousImport>(vm).HasCompleted());
				}),
				ForeignMethod("hasFailed",[](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotBool(vm, 0, wrenGetReceiver<AsynchronousImport>(vm).HasFailed());
				}), }); // clang-format on
	}

	// ---------------------------------------------------

	ET_IMPLEMENT_WREN_CLASS(ContentLocator, api, moduleName) {
		api.DefineClass<ContentLocator>("ContentLocatorClass", moduleName, { /* Static methods */ }, { /* Methods */ });
	}

}} // namespace Eldritch2::Assets
