/*==================================================================*\
  ActionSet.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/ActionSet.hpp>
#include <Input/InputBus.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM* vm, int slot);

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	using namespace ::Eldritch2::Input;

	ET_IMPLEMENT_WREN_CLASS(ActionSet) {
		// clang-format off
		api.CreateClass<ActionSet>(ET_BUILTIN_WREN_MODULE_NAME(Input), "ActionSet",
			{ /*	Constructors */
				ConstructorMethod("new(_)", [](WrenVM* vm) {
					SetReturn<ActionSet>(vm).BindResources(vm, 1);
				})},
			{ /*	Static methods */ },
			{ /*	Properties */ },
			{ /*	Methods */
				ForeignMethod("acquireLocalDevice(_,_)", [](WrenVM* vm) {
					InputBus& bus(GetSlotAs<InputBus>(vm, 1));
					DeviceId  id(AsInt(wrenGetSlotDouble(vm, 2)));

					ET_ABORT_WREN_UNLESS(GetSlotAs<ActionSet>(vm, 0).TryAcquireDevice(bus, id), "Unable to acquire device.");
				}),
				ForeignMethod("releaseDevices()", [](WrenVM* vm) {
					GetSlotAs<ActionSet>(vm, 0).ReleaseDevices();
				}) });
		// clang-format on
	}

}
}
} // namespace Eldritch2::Scripting::Wren
