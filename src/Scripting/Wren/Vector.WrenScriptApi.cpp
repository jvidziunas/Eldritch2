/*==================================================================*\
  Vector.WrenScriptApi.cpp
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
#include <Scripting/Wren/ScriptExecutor.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Marshal.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM* vm, int slot);
void   wrenSetSlotDouble(WrenVM* vm, int slot, double value);
void   wrenSetSlotHandle(WrenVM* vm, int slot, WrenHandle* handle);

namespace Eldritch2 { namespace Scripting { namespace Wren {

	ET_IMPLEMENT_WREN_CLASS(Vector) {
		api.DefineClass<Vector>(ET_BUILTIN_WREN_MODULE_NAME(Math), "Vector", // clang-format off
			{/*	Static methods */
				ForeignMethod("of(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, float32(wrenGetSlotDouble(vm, 1)), float32(wrenGetSlotDouble(vm, 2)), float32(wrenGetSlotDouble(vm, 3)), 0.0f);
				}),
				ForeignMethod("of(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, float32(wrenGetSlotDouble(vm, 1)), float32(wrenGetSlotDouble(vm, 1)), float32(wrenGetSlotDouble(vm, 1)), 0.0f);
				}),
				ForeignMethod("forward", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, Vector::MakeForward());
				}),
				ForeignMethod("up", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, Vector::MakeUp());
				}),
				ForeignMethod("right", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, Vector::MakeRight());
				}),
				ForeignMethod("zero",[](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, Vector::MakeIdentity());
				}),
				ForeignMethod("dot(_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, DotProduct(wrenGetSlotAs<Vector>(vm, 1), wrenGetSlotAs<Vector>(vm, 2)));
				}),
				ForeignMethod("interpolate(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, Interpolate(wrenGetSlotAs<Vector>(vm, 1), wrenGetSlotAs<Vector>(vm, 2), wrenGetSlotDouble(vm, 3)));
				}), },
			{/*	Methods */
				ForeignMethod("magnitudeSquared", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, SquaredLength(wrenGetSlotAs<Vector>(vm, 0)));
				}),
				ForeignMethod("magnitude", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, Length(wrenGetSlotAs<Vector>(vm, 0)));
				}),
				ForeignMethod("abs", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, AbsoluteValue(wrenGetSlotAs<Vector>(vm, 0)));
				}),
				ForeignMethod("normalize", [](WrenVM* vm) ETNoexceptHint {
					wrenGetSlotAs<Vector>(vm, 0).Normalize();
				}),
				ForeignMethod("normalized", [](WrenVM* vm) ETNoexceptHint {
					// Create temporary copy; calling Normalize() directly will modify the calller.
					Vector vector(wrenGetSlotAs<Vector>(vm, 0));
					wrenSetReturn<Vector>(vm, vector.Normalize());
				}),
				ForeignMethod("toString", [](WrenVM* vm) ETNoexceptHint {
					ET16ByteAligned float32 coefficients[4];
					fmt::memory_buffer	    string;

					StreamCoefficients(coefficients, wrenGetSlotAs<Vector>(vm, 0));
					fmt::format_to(string, "<x={}, y={}, z={}>", coefficients[Vector::X], coefficients[Vector::Y], coefficients[Vector::Z]);
					wrenSetSlotBytes(vm, 0, string.data(), string.size());
				}),
				ForeignMethod("+(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, wrenGetSlotAs<Vector>(vm, 0) + wrenGetSlotAs<Vector>(vm, 1));
				}),
				ForeignMethod("-(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, wrenGetSlotAs<Vector>(vm, 0) - wrenGetSlotAs<Vector>(vm, 1));
				}),
				ForeignMethod("*(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, wrenGetSlotAs<Vector>(vm, 0) * float32(wrenGetSlotDouble(vm, 1)));
				}),
				ForeignMethod("/(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, wrenGetSlotAs<Vector>(vm, 0) / float32(wrenGetSlotDouble(vm, 1)));
				}),
				ForeignMethod("^(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, wrenGetSlotAs<Vector>(vm, 0) ^ wrenGetSlotAs<Vector>(vm, 1));
				}),
				ForeignMethod("-", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, -wrenGetSlotAs<Vector>(vm, 0));
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Scripting::Wren
