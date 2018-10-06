/*==================================================================*\
  Light.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM* vm, int slot);

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;

	namespace {

		class Light : public Graphics::Light {
		public:
			//!	Constructs this @ref Light instance.
			ETInlineHint ETForceInlineHint Light(Transformation localToWorld, RgbColor color, float16 radius) ETNoexceptHint : Graphics::Light(localToWorld, color, radius) {}
			//! Disable copy assignment.
			Light(const Light&) = delete;

			~Light() = default;

			//!	Disable copy construction.
			Light& operator=(const Light&) = delete;
		};

	} // anonymous namespace

	ET_IMPLEMENT_WREN_CLASS(Light) {
		api.DefineClass<Light>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "Light", // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Light>(vm, /*classSlot =*/0, GetSlotAs<Transformation>(vm, 2), GetSlotAs<RgbColor>(vm, 3), float16(wrenGetSlotDouble(vm, 4)));
				}), },
			{ /* Methods */
				ForeignMethod("color", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<RgbColor>(vm, GetSlotAs<Light>(vm, 0).color);
				}),
				ForeignMethod("color=(_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Light>(vm, 0).color = GetSlotAs<RgbColor>(vm, 1);
				}),
				ForeignMethod("localToWorld", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Transformation>(vm, GetSlotAs<Light>(vm, 0).localToWorld);
				}),
				ForeignMethod("localToWorld=(_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Light>(vm, 0).localToWorld = GetSlotAs<Transformation>(vm, 1);
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Graphics::Vulkan
