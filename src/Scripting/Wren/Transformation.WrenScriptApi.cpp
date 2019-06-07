/*==================================================================*\
  Transformation.WrenScriptApi.cpp
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
//------------------------------------------------------------------//

void wrenSetSlotBytes(WrenVM* vm, int slot, const char* text, size_t length);
void wrenSetSlotHandle(WrenVM* vm, int slot, WrenHandle* handle);

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ET_IMPLEMENT_WREN_CLASS(Transformation) {
		api.DefineClass<Transformation>(ET_BUILTIN_WREN_MODULE_NAME(Math), "Transformation", // clang-format off
			{/*	Static methods */
				ForeignMethod("new(_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Transformation>(vm, /*classSlot=*/0, wrenGetSlotAs<Vector>(vm, 1), wrenGetSlotAs<Quaternion>(vm, 2));
				}),
				ForeignMethod("identity", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Transformation>(vm, /*classSlot =*/0, Transformation::MakeIdentity());
				}), },
			{/*	Methods */
				ForeignMethod("translation", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, wrenGetSlotAs<Transformation>(vm, 0).translation);
				}),
				ForeignMethod("translation=(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenGetSlotAs<Transformation>(vm, 0).translation = wrenGetSlotAs<Vector>(vm, 1);
				}),
				ForeignMethod("rotation", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Quaternion>(vm, wrenGetSlotAs<Transformation>(vm, 0).rotation);
				}),
				ForeignMethod("rotation=(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenGetSlotAs<Transformation>(vm, 0).rotation = wrenGetSlotAs<Quaternion>(vm, 1);
				}),
				ForeignMethod("toString", [](WrenVM* vm) ETNoexceptHint {
					ET16ByteAligned float32 translation[4];
					ET16ByteAligned float32 rotation[4];
					fmt::memory_buffer		string;
					const Transformation    self(wrenGetSlotAs<Transformation>(vm, 0));

					StreamCoefficients(translation, self.translation);
					StreamCoefficients(rotation, self.rotation);
					fmt::format_to(string, "<<x={}, y={}, z={}>, <i={}, j={}, k={}, w={}>>",
						translation[Vector::X], translation[Vector::Y], translation[Vector::Z],
						rotation[Quaternion::I], rotation[Quaternion::J], rotation[Quaternion::K], rotation[Quaternion::W]);

					wrenSetSlotBytes(vm, 0, string.data(), string.size());
				}),
				ForeignMethod("~", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Transformation>(vm, ~wrenGetSlotAs<Transformation>(vm, 0));
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Scripting::Wren
