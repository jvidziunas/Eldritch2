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
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM* vm, int slot);
void   wrenSetSlotDouble(WrenVM* vm, int slot, double value);
void   wrenSetSlotHandle(WrenVM* vm, int slot, WrenHandle* handle);

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ET_IMPLEMENT_WREN_CLASS(Vector) { // clang-format off
		api.CreateClass<Vector>(ET_BUILTIN_WREN_MODULE_NAME(Math), "Vector",
			{/* Constructors */
				ConstructorMethod("new(_,_,_)", [](WrenVM* vm) {
					SetReturn<Vector>(vm, float32(wrenGetSlotDouble(vm, 1)), float32(wrenGetSlotDouble(vm, 2)), float32(wrenGetSlotDouble(vm, 3)), 0.0f);
				}),
				ConstructorMethod("splat(_)", [](WrenVM* vm) {
					SetReturn<Vector>(vm, float32(wrenGetSlotDouble(vm, 1)), float32(wrenGetSlotDouble(vm, 1)), float32(wrenGetSlotDouble(vm, 1)), 0.0f);
				})},
			{/*	Static methods */
				StaticMethod("forward", [](WrenVM* vm) {
					SetReturn<Vector>(vm, 0.0f, 0.0f, 1.0f, 0.0f);
				}),
				StaticMethod("up", [](WrenVM* vm) {
					SetReturn<Vector>(vm, 0.0f, 1.0f, 0.0f, 0.0f);
				}),
				StaticMethod("right", [](WrenVM* vm) {
					SetReturn<Vector>(vm, 1.0f, 0.0f, 0.0f, 0.0f);
				}),
				StaticMethod("dot(_,_)", [](WrenVM* vm) {
					wrenSetSlotDouble(vm, 0, DotProduct(GetSlotAs<Vector>(vm, 1), GetSlotAs<Vector>(vm, 2)));
				}),
				StaticMethod("cross(_,_)", [](WrenVM* vm) {
					SetReturn<Vector>(vm, CrossProduct(GetSlotAs<Vector>(vm, 1), GetSlotAs<Vector>(vm, 2)));
				}),
				StaticMethod("lerp(_,_,_)", [](WrenVM* vm) {
					SetReturn<Vector>(vm, LinearInterpolate(GetSlotAs<Vector>(vm, 1), GetSlotAs<Vector>(vm, 2), wrenGetSlotDouble(vm, 3)));
				})},
			{/*	Properties */
				DefineGetter("lengthSquared", [](WrenVM* vm) {
					wrenSetSlotDouble(vm, 0, SquaredLength(GetSlotAs<Vector>(vm, 0)));
				}),
				DefineGetter("length", [](WrenVM* vm) {
					wrenSetSlotDouble(vm, 0, Length(GetSlotAs<Vector>(vm, 0)));
				}),
				DefineGetter("abs", [](WrenVM* vm) {
					const Vector&	self(GetSlotAs<Vector>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Vector>());
					SetReturn<Vector>(vm, AbsoluteValue(self));
				}),
				DefineGetter("toString", [](WrenVM* vm) {
					fmt::memory_buffer	string;
					float32				coefficients[4];

					GetSlotAs<Vector>(vm, 0).ExtractCoefficients(coefficients);
					fmt::format_to(string, "<x={}, y={}, z={}>", coefficients[3], coefficients[2], coefficients[1]);

					wrenSetSlotBytes(vm, 0, string.data(), string.size());
				})},
			{/*	Methods */
				ForeignMethod("+(_)", [](WrenVM* vm) {
					const Vector&	self(GetSlotAs<Vector>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Vector>());
					SetReturn<Vector>(vm, self + GetSlotAs<Vector>(vm, 1));
				}),
				ForeignMethod("-(_)", [](WrenVM* vm) {
					const Vector&	self(GetSlotAs<Vector>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Vector>());
					SetReturn<Vector>(vm, self - GetSlotAs<Vector>(vm, 1));
				}),
				ForeignMethod("*(_)", [](WrenVM* vm) {
					const Vector&	self(GetSlotAs<Vector>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Vector>());
					SetReturn<Vector>(vm, self * float32(wrenGetSlotDouble(vm, 1)));
				}),
				ForeignMethod("/(_)", [](WrenVM* vm) {
					const Vector&	self(GetSlotAs<Vector>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Vector>());
					SetReturn<Vector>(vm, self / float32(wrenGetSlotDouble(vm, 1)));
				}),
				ForeignMethod("-", [](WrenVM* vm) {
					const Vector&	self(GetSlotAs<Vector>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Vector>());
					SetReturn<Vector>(vm, -self);
				})});
	} // clang-format on

}}} // namespace Eldritch2::Scripting::Wren
