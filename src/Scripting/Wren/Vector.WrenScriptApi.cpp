/*==================================================================*\
  Vector.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Marshal.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM* vm, int slot);
void   wrenSetSlotDouble(WrenVM* vm, int slot, double value);
void   wrenSetSlotHandle(WrenVM* vm, int slot, WrenHandle* handle);

namespace Eldritch2 { namespace Scripting { namespace Wren {

	ET_IMPLEMENT_WREN_CLASS(Vector) {
		api.DefineClass<Vector>(ET_BUILTIN_WREN_MODULE_NAME(Math), "Vector", // clang-format off
			{/*	Static methods */
				ForeignMethod("of(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, float32(wrenGetSlotDouble(vm, 1)), float32(wrenGetSlotDouble(vm, 2)), float32(wrenGetSlotDouble(vm, 3)), 0.0f);
				}),
				ForeignMethod("of(_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, float32(wrenGetSlotDouble(vm, 1)), float32(wrenGetSlotDouble(vm, 1)), float32(wrenGetSlotDouble(vm, 1)), 0.0f);
				}),
				ForeignMethod("forward", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, Vector::MakeForward());
				}),
				ForeignMethod("up", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, Vector::MakeUp());
				}),
				ForeignMethod("right", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, Vector::MakeRight());
				}),
				ForeignMethod("dot(_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, DotProduct(GetSlotAs<Vector>(vm, 1), GetSlotAs<Vector>(vm, 2)));
				}),
				ForeignMethod("interpolate(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, Interpolate(GetSlotAs<Vector>(vm, 1), GetSlotAs<Vector>(vm, 2), wrenGetSlotDouble(vm, 3)));
				}), },
			{/*	Methods */
				ForeignMethod("lengthSquared", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, SquaredLength(GetSlotAs<Vector>(vm, 0)));
				}),
				ForeignMethod("length", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, Length(GetSlotAs<Vector>(vm, 0)));
				}),
				ForeignMethod("abs", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, AbsoluteValue(GetSlotAs<Vector>(vm, 0)));
				}),
				ForeignMethod("toString", [](WrenVM* vm) ETNoexceptHint {
					ET16ByteAligned float32 coefficients[4];
					fmt::memory_buffer	    string;

					StreamCoefficients(coefficients, GetSlotAs<Vector>(vm, 0));
					fmt::format_to(string, "<x={}, y={}, z={}>", coefficients[Vector::X], coefficients[Vector::Y], coefficients[Vector::Z]);
					wrenSetSlotBytes(vm, 0, string.data(), string.size());
				}),
				ForeignMethod("+(_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, GetSlotAs<Vector>(vm, 0) + GetSlotAs<Vector>(vm, 1));
				}),
				ForeignMethod("-(_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, GetSlotAs<Vector>(vm, 0) - GetSlotAs<Vector>(vm, 1));
				}),
				ForeignMethod("*(_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, GetSlotAs<Vector>(vm, 0) * float32(wrenGetSlotDouble(vm, 1)));
				}),
				ForeignMethod("/(_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, GetSlotAs<Vector>(vm, 0) / float32(wrenGetSlotDouble(vm, 1)));
				}),
				ForeignMethod("^(_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, GetSlotAs<Vector>(vm, 0) ^ GetSlotAs<Vector>(vm, 1));
				}),
				ForeignMethod("-", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, -GetSlotAs<Vector>(vm, 0));
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Scripting::Wren
