/*==================================================================*\
  DeviceLocator.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Input/DeviceLocator.hpp>
//------------------------------------------------------------------//

void wrenSetSlotDouble(WrenVM* vm, int slot, double value);

namespace Eldritch2 { namespace Input {

	using namespace ::Eldritch2::Scripting::Wren;

	ET_IMPLEMENT_WREN_CLASS(DeviceLocator) {
		api.DefineClass<DeviceLocator>(ET_BUILTIN_WREN_MODULE_NAME(Input), "DeviceLocator", // clang-format off
			{ /* Static methods */ },
			{ /* Methods */
				ForeignMethod("deviceCount", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, AsFloat64(GetSlotAs<DeviceLocator>(vm, 0).GetDeviceCount()));
				}) }); // clang-format on
	}

}} // namespace Eldritch2::Input
