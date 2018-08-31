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

	ET_IMPLEMENT_WREN_CLASS(Quaternion) { // clang-format off
		api.CreateClass<Quaternion>(ET_BUILTIN_WREN_MODULE_NAME(Math), "Quaternion",
			{/* Constructors */
				ConstructorMethod("new(_,_,_,_)", [](WrenVM* vm) {
					SetReturn<Quaternion>(
						vm,
						float32(wrenGetSlotDouble(vm, 1)),
						float32(wrenGetSlotDouble(vm, 2)),
						float32(wrenGetSlotDouble(vm, 3)),
						float32(wrenGetSlotDouble(vm, 4))
					);
				}),
				ConstructorMethod("forBasis(_,_)", [](WrenVM* vm) {
					SetReturn<Quaternion>(vm, AsBasis(GetSlotAs<Vector>(vm, 1), GetSlotAs<Vector>(vm, 2)));
				})},
			{/*	Static methods */
				StaticMethod("identity", [](WrenVM * vm) {
					SetReturn<Quaternion>(vm, Quaternion::MakeIdentity());
				}),
				StaticMethod("dot(_,_)", [](WrenVM* vm) {
					wrenSetSlotDouble(vm, 0, DotProduct(GetSlotAs<Quaternion>(vm, 1), GetSlotAs<Quaternion>(vm, 2)));
				}),
				StaticMethod("lerp(_,_,_)", [](WrenVM* vm) {
					SetReturn<Quaternion>(vm, LinearInterpolate(GetSlotAs<Quaternion>(vm, 1), GetSlotAs<Quaternion>(vm, 2), wrenGetSlotDouble(vm, 3)));
				})},
			{/*	Properties */
				DefineGetter("forward", [](WrenVM* vm) {
					const Quaternion&	self(GetSlotAs<Quaternion>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Vector>());
					SetReturn<Vector>(vm, self.GetForward());
				}),
				DefineGetter("right", [](WrenVM* vm) {
					const Quaternion&	self(GetSlotAs<Quaternion>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Vector>());
					SetReturn<Vector>(vm, self.GetRight());
				}),
				DefineGetter("up", [](WrenVM* vm) {
					const Quaternion&	self(GetSlotAs<Quaternion>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Vector>());
					SetReturn<Vector>(vm, self.GetUp());
				}),
				DefineGetter("toString", [](WrenVM* vm) {
					fmt::memory_buffer	string;
					float32				coefficients[4];

					GetSlotAs<Quaternion>(vm, 0).ExtractCoefficients(coefficients);
					fmt::format_to(string, "<i={}, j={}, k={}, w={}>", coefficients[3], coefficients[2], coefficients[1], coefficients[0]);

					wrenSetSlotBytes(vm, 0, string.data(), string.size());
				})},
			{/*	Methods */
				ForeignMethod("*(_)", [](WrenVM* vm) {
					const Quaternion&	self(GetSlotAs<Quaternion>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Quaternion>());
					SetReturn<Quaternion>(vm, self) *= GetSlotAs<Quaternion>(vm, 1);
				}),
				ForeignMethod("~", [](WrenVM* vm) {
					const Quaternion&	self(GetSlotAs<Quaternion>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Quaternion>());
					SetReturn<Quaternion>(vm, self.GetReverse());
				})});
		} // clang-format on

}}} // namespace Eldritch2::Scripting::Wren
