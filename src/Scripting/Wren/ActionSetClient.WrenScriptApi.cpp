/*==================================================================*\
  ActionSetClient.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ActionSetClient.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Input/DeviceLocator.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM* vm, int slot);

namespace {

ETInlineHint ETForceInlineHint ::Eldritch2::Input::DeviceId wrenGetSlotDeviceId(WrenVM* vm, int slot) ETNoexceptHint {
	return ::Eldritch2::Input::DeviceId(::Eldritch2::AsInt(wrenGetSlotDouble(vm, 2)));
}

} // anonymous namespace

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Input;

	ET_IMPLEMENT_WREN_CLASS(ActionSetClient) {
		api.DefineClass<ActionSetClient>(ET_BUILTIN_WREN_MODULE_NAME(Input), "ActionSet", // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<ActionSetClient>(vm, /*classSlot =*/0).BindResources(vm, 1);
				}), },
			{ /* Methods */
				ForeignMethod("acquireLocalDevice(_,_)", [](WrenVM* vm) ETNoexceptHint {
					ActionSetClient& self(GetSlotAs<ActionSetClient>(vm, 0));
					ET_ABORT_WREN_UNLESS(GetSlotAs<DeviceLocator>(vm, 1).TryAcquireDevice(wrenGetSlotDeviceId(vm, 2), self, {}), "Unable to acquire device.");
				}),
				ForeignMethod("acquireLocalDevice(_)", [](WrenVM* vm) ETNoexceptHint {
					ActionSetClient& self(GetSlotAs<ActionSetClient>(vm, 0));
					ET_ABORT_WREN_UNLESS(GetSlotAs<DeviceLocator>(vm, 1).TryAcquireDevice(self, {}), "Unable to acquire device.");
				}),
				ForeignMethod("releaseDevices()", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<ActionSetClient>(vm, 0).ReleaseDevices();
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Scripting::Wren
