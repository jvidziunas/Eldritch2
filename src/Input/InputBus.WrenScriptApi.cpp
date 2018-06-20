/*==================================================================*\
  InputBus.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Input/InputBus.hpp>
//------------------------------------------------------------------//

void wrenSetSlotDouble(WrenVM* vm, int slot, double value);

namespace Eldritch2 { namespace Input {

	using namespace ::Eldritch2::Scripting::Wren;

	ET_IMPLEMENT_WREN_CLASS(InputBus) {
		// clang-format off
		api.CreateClass<InputBus>(ET_BUILTIN_WREN_MODULE_NAME(Input), "InputBus",
			{ /*	Constructors */ },
			{ /*	Static methods */ },
			{ /*	Properties */
				DefineGetter("deviceCount", [](WrenVM* vm) {
					wrenSetSlotDouble(vm, 0, AsFloat64(GetSlotAs<InputBus>(vm, 0).GetDeviceCount()));
				}) },
			{ /*	Methods */ });
		// clang-format on
	}

}} // namespace Eldritch2::Input
