/*==================================================================*\
  VulkanGraphicsScene.PlayerView.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/DisplayLocator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM*, int);
void   wrenSetSlotDouble(WrenVM*, int, double);

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS(PlayerView) {
		api.DefineClass<PlayerView>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "PlayerView", // clang-format off
			{/* Constructors */
				ConstructorMethod("new(_,_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					auto& scene(GetSlotAs<VulkanGraphicsScene>(vm, 1));
					auto  viewport(GetSlotAs<DisplayLocator>(vm, 2).TryAcquireViewport(scene.GetLitPipeline()));
					ET_ABORT_WREN_UNLESS(viewport, "Error acquiring display.");

					SetReturn<PlayerView>(vm, /*classSlot =*/0, eastl::move(viewport), GetSlotAs<Transformation>(vm, 3), Angle(wrenGetSlotDouble(vm, 4)));
				}), },
			{/*	Static methods */
				StaticMethod("getVerticalFov(_)", [](WrenVM* vm) ETNoexceptHint {
					static ETConstexpr float32 WidescreenAspect(16.0 / 9.0);
					wrenSetSlotDouble(vm, 0, WidescreenAspect * wrenGetSlotDouble(vm, 1));
				}), },
			{/*	Properties */
				DefineProperty("worldToView",
					/*getterBody =*/[](WrenVM* vm) ETNoexceptHint {
						SetReturn<Transformation>(vm, GetSlotAs<PlayerView>(vm, 0).GetWorldToView());
					},
					/*setterBody =*/[](WrenVM* vm) ETNoexceptHint {
						GetSlotAs<PlayerView>(vm, 0).SetWorldToView(GetSlotAs<Transformation>(vm, 1));
					}),
				DefineGetter("viewToWorld", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Transformation>(vm, ~GetSlotAs<PlayerView>(vm, 0).GetWorldToView());
				}),
				DefineProperty("verticalFovDegrees",
					/*getterBody =*/[](WrenVM* vm) ETNoexceptHint {
						wrenSetSlotDouble(vm, 0, DegreesFromAngle(GetSlotAs<PlayerView>(vm, 0).GetVerticalFov()));
					},
					/*setterBody =*/[](WrenVM* vm) ETNoexceptHint {
						GetSlotAs<PlayerView>(vm, 0).SetVerticalFov(AngleFromDegrees(float32(wrenGetSlotDouble(vm, 1))));
					})},
			{/* Methods */ }); // clang-format on
	}

}}} // namespace Eldritch2::Graphics::Vulkan
