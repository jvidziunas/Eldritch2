/*==================================================================*\
  Dispatcher.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/Dispatcher.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

WrenHandle*	wrenGetSlotHandle(WrenVM* vm, int slot);
double		wrenGetSlotDouble(WrenVM* vm, int slot);
void		wrenSetSlotDouble(WrenVM* vm, int slot, double value);

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {

			ET_IMPLEMENT_WREN_CLASS(Dispatcher) {
				api.CreateClass<Dispatcher>(ET_BUILTIN_WREN_MODULE_NAME(Core), "DispatcherClass",
											{/* Constructors */ },
											{/*	Static methods */ },
			{/*	Properties */
				DefineGetter("now", [](WrenVM* vm) {
					wrenSetSlotDouble(vm, 0, GetSlotAs<Dispatcher>(vm, 0).GetNow());
				}),
				DefineProperty("timeScalar",
				//	Getter
					[](WrenVM* vm) {
						wrenSetSlotDouble(vm, 0, GetSlotAs<Dispatcher>(vm, 0).GetWorldTimeScalar());
					},
				//	Setter
					[](WrenVM* vm) {
						GetSlotAs<Dispatcher>(vm, 0).SetWorldTimeScalar(wrenGetSlotDouble(vm, 1));
					}
				)
			},
			{/*	Methods */
				ForeignMethod("callAfterDelay(_,_)", [](WrenVM* vm) {
					Dispatcher&		self(GetSlotAs<Dispatcher>(vm, 0));
					const double	delay(wrenGetSlotDouble(vm, 1));

					ET_ABORT_WREN_IF(delay <= 0.0, "Events cannot be scheduled in the past!");

					self.CallAtGameTime(self.GetNow() + AsInt(delay), wrenGetSlotHandle(vm, 2));
				}),
				ForeignMethod("shutDownEngine()", [](WrenVM* vm) {
					GetSlotAs<Dispatcher>(vm, 0).ShutDownWorld( /*andEngine =*/true);
				}),
				ForeignMethod("shutDown()", [](WrenVM* vm) {
					GetSlotAs<Dispatcher>(vm, 0).ShutDownWorld( /*andEngine =*/false);
				})
			}
			);
			}

		}	// namespace Wren
	}	// namespace Scripting
}	// namespace Eldritch2