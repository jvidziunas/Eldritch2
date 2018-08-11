/*==================================================================*\
  PackageManager.WrenScriptApi.cpp
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
#include <Assets/PackageLocator.hpp>
//------------------------------------------------------------------//

const char* wrenGetSlotString(WrenVM* vm, int slot);
void        wrenGetListElement(WrenVM* vm, int listSlot, int index, int elementSlot);
void        wrenSetSlotBool(WrenVM* vm, int slot, bool value);
void        wrenEnsureSlots(WrenVM* vm, int numSlots);
int         wrenGetListCount(WrenVM* vm, int slot);

namespace Eldritch2 { namespace Assets {

	using namespace ::Eldritch2::Scripting::Wren;

	ET_IMPLEMENT_WREN_CLASS(AsynchronousImport) { // clang-format off
		api.CreateClass<AsynchronousImport>(ET_BUILTIN_WREN_MODULE_NAME(Core), "AsynchronousImport",
			{ /* Constructors */
				ConstructorMethod("of(_,_)", [](WrenVM* vm) {
					AsynchronousImport::PackageList packages(MallocAllocator("Asynchronous Import Package List Allocator"));
					PackageLocator&                 locator(GetSlotAs<PackageLocator>(vm, 1));

					wrenEnsureSlots(vm, 4);

					for (int element(0), count(wrenGetListCount(vm, 2)); element < count; ++element) {
						wrenGetListElement(vm, 2, element, 3);

						const Utf8Char* const path(wrenGetSlotString(vm, 3));
						const ErrorCode       result(locator.Require(path));

						ET_ABORT_WREN_UNLESS_FMT(Succeeded(result), "Unable to load package {}: {}.", path, AsCString(result));
					}

					SetReturn<AsynchronousImport>(vm, eastl::move(packages));
				}) },
			{ /* Static methods */ },
			{ /* Properties */
				DefineGetter("isComplete", [](WrenVM* vm) {
					wrenSetSlotBool(vm, 0, GetSlotAs<AsynchronousImport>(vm, 0).IsComplete());
				}) },
			{ /* Methods */ }
		);
	} // clang-format on

	ET_IMPLEMENT_WREN_CLASS(PackageLocator) { // clang-format off
		api.CreateClass<PackageLocator>(ET_BUILTIN_WREN_MODULE_NAME(Core), "PackageManager",
			{ /* Constructors */ },
			{ /* Static methods */ },
			{ /* Properties */ },
			{ /* Methods */
				ForeignMethod("require(_)", [](WrenVM* vm) {
					PackageLocator&       locator(GetSlotAs<PackageLocator>(vm, 0));
					const Utf8Char* const path(wrenGetSlotString(vm, 1));
					const ErrorCode       result(locator.Require(path));

					ET_ABORT_WREN_UNLESS_FMT(Succeeded(result), "Unable to load package {}: {}.", path, AsCString(result));
				})}
		);
	} // clang-format on

}} // namespace Eldritch2::Assets
