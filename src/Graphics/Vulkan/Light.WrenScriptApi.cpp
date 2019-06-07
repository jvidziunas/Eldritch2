/*==================================================================*\
  Light.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM* vm, int slot);

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;

	// ---------------------------------------------------

	namespace {

		class Light : public RenderLight {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Light instance.
			ETForceInlineHint Light(Transformation localToWorld, RgbColor color, float32 radius) ETNoexceptHint : RenderLight(localToWorld, color, radius) {}
			//! Disable copy assignment.
			Light(const Light&) = delete;

			~Light() = default;

			// ---------------------------------------------------

			//!	Disable copy construction.
			Light& operator=(const Light&) = delete;
		};

	} // anonymous namespace

	ET_IMPLEMENT_WREN_CLASS(Light, api, moduleName) {
		api.DefineClass<Light>("Light", moduleName, // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Light>(vm, /*classSlot =*/0, wrenGetSlotAs<Transformation>(vm, 2), wrenGetSlotAs<RgbColor>(vm, 3), float32(wrenGetSlotDouble(vm, 4)));
				}), },
			{ /* Methods */
				ForeignMethod("color", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<RgbColor>(vm, wrenGetSlotAs<Light>(vm, 0).color);
				}),
				ForeignMethod("color=(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenGetSlotAs<Light>(vm, 0).color = wrenGetSlotAs<RgbColor>(vm, 1);
				}),
				ForeignMethod("localToWorld", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Transformation>(vm, wrenGetSlotAs<Light>(vm, 0).localToWorld);
				}),
				ForeignMethod("localToWorld=(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenGetSlotAs<Light>(vm, 0).localToWorld = wrenGetSlotAs<Transformation>(vm, 1);
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Graphics::Vulkan
