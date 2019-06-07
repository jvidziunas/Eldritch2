/*==================================================================*\
  RgbColor.WrenScriptApi.cpp
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
#include <Graphics/RgbColor.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM* vm, int slot);

namespace Eldritch2 { namespace Graphics {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	// ---------------------------------------------------

	ET_IMPLEMENT_WREN_CLASS(RgbColor, api, moduleName) {
		api.DefineClass<RgbColor>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "RgbColor", // clang-format off
			{ /* Constructors */
				ForeignMethod("ofRgb(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<RgbColor>(
						vm,
						/*classSlot =*/0,
						float32(wrenGetSlotDouble(vm, 1)),
						float32(wrenGetSlotDouble(vm, 2)),
						float32(wrenGetSlotDouble(vm, 3)));
				}),
				ForeignMethod("ofXyz(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<RgbColor>(
						vm,
						/*classSlot =*/0,
						GetRgbFromCieXyz(float32(wrenGetSlotDouble(vm, 1)), float32(wrenGetSlotDouble(vm, 2)), float32(wrenGetSlotDouble(vm, 3))));
				}),
				ForeignMethod("ofDegreesKelvin(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<RgbColor>(vm, GetRgbFromKelvin(wrenGetSlotDouble(vm, 1)));
				}), },
			{ /* Methods */
				ForeignMethod("toString", [](WrenVM* vm) ETNoexceptHint {
					fmt::memory_buffer string;
					const RgbColor&    self(wrenGetSlotAs<RgbColor>(vm, 0));

					fmt::format_to(string, "<r={}, g={}, b={}>", self.GetRed(), self.GetGreen(), self.GetBlue());
					wrenSetSlotBytes(vm, /*slot =*/0, string.data(), string.size());
				}), }); // clang-format on
	}

}} // namespace Eldritch2::Graphics
