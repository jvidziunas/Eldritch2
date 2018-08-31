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
void   wrenSetSlotHandle(WrenVM*, int, WrenHandle*);

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS(PlayerView) { // clang-format off
		api.CreateClass<PlayerView>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "PlayerView",
			{/* Constructors */
				ConstructorMethod("new(_,_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					auto&      scene(GetSlotAs<VulkanGraphicsScene>(vm, 1));
					const auto viewport(GetSlotAs<DisplayLocator>(vm, 2).TryAcquireViewport(scene.GetOpaqueLitPipeline()));
					ET_ABORT_WREN_UNLESS(viewport, "Error acquiring display.");

					SetReturn<PlayerView>(vm, *viewport, GetSlotAs<Transformation>(vm, 3), Angle(wrenGetSlotDouble(vm, 4)));
				})},
			{/*	Static methods */
				StaticMethod("getVerticalFov(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, (16.0 / 9.0) * wrenGetSlotDouble(vm, 1));
				})},
			{/*	Properties */
				DefineProperty("worldToView",
				//	Getter
					[](WrenVM* vm) ETNoexceptHint {
						const PlayerView& self(GetSlotAs<PlayerView>(vm, 0));

						wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Transformation>());
						SetReturn<Transformation>(vm, self.GetWorldToView());
					},
				//	Setter
					[](WrenVM* vm) ETNoexceptHint {
						GetSlotAs<PlayerView>(vm, 0).SetWorldToView(GetSlotAs<Transformation>(vm, 1));
					}),
				DefineGetter("viewToWorld", [](WrenVM* vm) ETNoexceptHint {
					const PlayerView& self(GetSlotAs<PlayerView>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Transformation>());
					SetReturn<Transformation>(vm, self.GetWorldToView().GetInverse());
				}),
				DefineProperty("verticalFovDegrees",
				//	Getter
					[](WrenVM* vm) ETNoexceptHint {
						wrenSetSlotDouble(vm, 0, DegreesFromAngle(GetSlotAs<PlayerView>(vm, 0).GetVerticalFov()));
					},
				//	Setter
					[](WrenVM* vm) ETNoexceptHint {
						GetSlotAs<PlayerView>(vm, 0).SetVerticalFov(AngleFromDegrees(wrenGetSlotDouble(vm, 1)));
					})},
			{/* Methods */ });
	} // clang-format on

}}} // namespace Eldritch2::Graphics::Vulkan
