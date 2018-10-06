/*==================================================================*\
  Quaternion.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM* vm, int slot);
void   wrenSetSlotDouble(WrenVM* vm, int slot, double value);
void   wrenSetSlotHandle(WrenVM*, int, WrenHandle*);

namespace Eldritch2 { namespace Scripting { namespace Wren {

	ET_IMPLEMENT_WREN_CLASS(Quaternion) {
		api.DefineClass<Quaternion>(ET_BUILTIN_WREN_MODULE_NAME(Math), "Quaternion", // clang-format off
			{/*	Static methods */
				ForeignMethod("new(_,_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Quaternion>(
						vm,
						/*classSlot =*/0,
						float32(wrenGetSlotDouble(vm, 1)),
						float32(wrenGetSlotDouble(vm, 2)),
						float32(wrenGetSlotDouble(vm, 3)),
						float32(wrenGetSlotDouble(vm, 4)));
				}),
				ForeignMethod("forBasis(_,_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Quaternion>(vm, AsBasis(GetSlotAs<Vector>(vm, 1), GetSlotAs<Vector>(vm, 2)));
				}),
				ForeignMethod("identity", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Quaternion>(vm, Quaternion::MakeIdentity());
				}),
				ForeignMethod("dot(_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, DotProduct(GetSlotAs<Quaternion>(vm, 1), GetSlotAs<Quaternion>(vm, 2)));
				}),
				ForeignMethod("interpolate(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Quaternion>(vm, Interpolate(GetSlotAs<Quaternion>(vm, 1), GetSlotAs<Quaternion>(vm, 2), float32(wrenGetSlotDouble(vm, 3))));
				}), },
			{/*	Methods */
				ForeignMethod("forward", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, GetSlotAs<Quaternion>(vm, 0).GetForward());
				}),
				ForeignMethod("right", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, GetSlotAs<Quaternion>(vm, 0).GetRight());
				}),
				ForeignMethod("up", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, GetSlotAs<Quaternion>(vm, 0).GetUp());
				}),
				ForeignMethod("toString", [](WrenVM* vm) ETNoexceptHint {
					ET16ByteAligned float32 coefficients[4];
					fmt::memory_buffer      string;

					StreamCoefficients(coefficients, GetSlotAs<Quaternion>(vm, 0));
					fmt::format_to(string, "<i={}, j={}, k={}, w={}>", coefficients[Quaternion::I], coefficients[Quaternion::J], coefficients[Quaternion::K], coefficients[Quaternion::W]);
					wrenSetSlotBytes(vm, 0, string.data(), string.size());
				}),
				ForeignMethod("*(_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Quaternion>(vm, GetSlotAs<Quaternion>(vm, 0) * GetSlotAs<Quaternion>(vm, 1));
				}),
				ForeignMethod("~", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Quaternion>(vm, GetSlotAs<Quaternion>(vm, 0).GetReverse());
				}), });
		} // clang-format on

}}} // namespace Eldritch2::Scripting::Wren
