/*==================================================================*\
  Dispatcher.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/Dispatcher.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

WrenHandle* wrenGetSlotHandle(WrenVM* vm, int slot);
double      wrenGetSlotDouble(WrenVM* vm, int slot);
void        wrenSetSlotDouble(WrenVM* vm, int slot, double value);

namespace Eldritch2 { namespace Scripting { namespace Wren {

	ET_IMPLEMENT_WREN_CLASS(Dispatcher) {
		api.DefineClass<Dispatcher>(ET_BUILTIN_WREN_MODULE_NAME(Core), "DispatcherClass", // clang-format off
			{ /* Static methods */ },
			{ /* Methods */
				ForeignMethod("now", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, GetSlotAs<Dispatcher>(vm, 0).GetNow());
				}),
				ForeignMethod("timeScalar", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, GetSlotAs<Dispatcher>(vm, 0).GetTimeScalar());
				}),
				ForeignMethod("timeScalar=(_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Dispatcher>(vm, 0).SetTimeScalar(wrenGetSlotDouble(vm, 1));
				}),
				ForeignMethod("callAfterDelay(_,_)", [](WrenVM* vm) ETNoexceptHint {
					const double delay(wrenGetSlotDouble(vm, 1));
					ET_ABORT_WREN_IF(vm, delay <= 0.0, "Events cannot be scheduled in the past!");

					GetSlotAs<Dispatcher>(vm, 0).CallOnDelay(AsInt(delay), wrenGetSlotHandle(vm, 2));
				}),
				ForeignMethod("shutDown()", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Dispatcher>(vm, 0).ShutDownWorld();
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Scripting::Wren
