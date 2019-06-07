/*==================================================================*\
  Quaternion.WrenScriptApi.cpp
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
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/ScriptExecutor.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM* vm, int slot);
void   wrenSetSlotDouble(WrenVM* vm, int slot, double value);
void   wrenSetSlotHandle(WrenVM*, int, WrenHandle*);

namespace Eldritch2 { namespace Scripting { namespace Wren {

	ET_IMPLEMENT_WREN_CLASS(Quaternion) {
		api.DefineClass<Quaternion>(ET_BUILTIN_WREN_MODULE_NAME(Math), "Quaternion", // clang-format off
			{/*	Static methods */
				ForeignMethod("identity", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Quaternion>(vm, Quaternion::MakeIdentity());
				}),
				ForeignMethod("new(_,_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Quaternion>(
						vm,
						/*classSlot =*/0,
						float32(wrenGetSlotDouble(vm, 1)),
						float32(wrenGetSlotDouble(vm, 2)),
						float32(wrenGetSlotDouble(vm, 3)),
						float32(wrenGetSlotDouble(vm, 4)));
				}),
				ForeignMethod("forBasis(_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Quaternion>(vm, AsBasis(wrenGetSlotAs<Vector>(vm, 1), wrenGetSlotAs<Vector>(vm, 2)));
				}),
				ForeignMethod("dot(_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, DotProduct(wrenGetSlotAs<Quaternion>(vm, 1), wrenGetSlotAs<Quaternion>(vm, 2)));
				}),
				ForeignMethod("interpolate(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Quaternion>(vm, Interpolate(wrenGetSlotAs<Quaternion>(vm, 1), wrenGetSlotAs<Quaternion>(vm, 2), float32(wrenGetSlotDouble(vm, 3))));
				}), },
			{/*	Methods */
				ForeignMethod("forward", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, wrenGetSlotAs<Quaternion>(vm, 0).GetForward());
				}),
				ForeignMethod("right", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, wrenGetSlotAs<Quaternion>(vm, 0).GetRight());
				}),
				ForeignMethod("up", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, wrenGetSlotAs<Quaternion>(vm, 0).GetUp());
				}),
				ForeignMethod("toString", [](WrenVM* vm) ETNoexceptHint {
					ET16ByteAligned float32 coefficients[4];
					fmt::memory_buffer      string;

					StreamCoefficients(coefficients, wrenGetSlotAs<Quaternion>(vm, 0));
					fmt::format_to(string, "<i={}, j={}, k={}, w={}>", coefficients[Quaternion::I], coefficients[Quaternion::J], coefficients[Quaternion::K], coefficients[Quaternion::W]);
					wrenSetSlotBytes(vm, 0, string.data(), string.size());
				}),
				ForeignMethod("*(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Quaternion>(vm, wrenGetSlotAs<Quaternion>(vm, 0) * wrenGetSlotAs<Quaternion>(vm, 1));
				}),
				ForeignMethod("~", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Quaternion>(vm, ~wrenGetSlotAs<Quaternion>(vm, 0));
				}),
				ForeignMethod("-",[](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Quaternion>(vm, -wrenGetSlotAs<Quaternion>(vm, 0));
				}), });
		} // clang-format on

}}} // namespace Eldritch2::Scripting::Wren
