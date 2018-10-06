/*==================================================================*\
  Transformation.WrenScriptApi.cpp
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

void wrenSetSlotBytes(WrenVM* vm, int slot, const char* text, size_t length);
void wrenSetSlotHandle(WrenVM* vm, int slot, WrenHandle* handle);

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ET_IMPLEMENT_WREN_CLASS(Transformation) {
		api.DefineClass<Transformation>(ET_BUILTIN_WREN_MODULE_NAME(Math), "Transformation", // clang-format off
			{/*	Static methods */
				ForeignMethod("new(_,_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Transformation>(vm, /*classSlot=*/0, GetSlotAs<Vector>(vm, 1), GetSlotAs<Quaternion>(vm, 2));
				}),
				ForeignMethod("identity", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Transformation>(vm, /*classSlot =*/0, Transformation::MakeIdentity());
				}), },
			{/*	Methods */
				ForeignMethod("translation", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Vector>(vm, GetSlotAs<Transformation>(vm, 0).translation);
				}),
				ForeignMethod("translation=(_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Transformation>(vm, 0).translation = GetSlotAs<Vector>(vm, 1);
				}),
				ForeignMethod("rotation", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Quaternion>(vm, GetSlotAs<Transformation>(vm, 0).rotation);
				}),
				ForeignMethod("rotation=(_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Transformation>(vm, 0).rotation = GetSlotAs<Quaternion>(vm, 1);
				}),
				ForeignMethod("toString", [](WrenVM* vm) ETNoexceptHint {
					ET16ByteAligned float32 translation[4];
					ET16ByteAligned float32 rotation[4];
					fmt::memory_buffer		string;

					const Transformation& self(GetSlotAs<Transformation>(vm, 0));
					StreamCoefficients(translation, self.translation);
					StreamCoefficients(rotation, self.rotation);
					fmt::format_to(string, "<<x={}, y={}, z={}>, <i={}, j={}, k={}, w={}>>",
						translation[Vector::X], translation[Vector::Y], translation[Vector::Z],
						rotation[Quaternion::I], rotation[Quaternion::J], rotation[Quaternion::K], rotation[Quaternion::W]);

					wrenSetSlotBytes(vm, 0, string.data(), string.size());
				}),
				ForeignMethod("~", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Transformation>(vm, GetSlotAs<Transformation>(vm, 0).GetInverse());
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Scripting::Wren
