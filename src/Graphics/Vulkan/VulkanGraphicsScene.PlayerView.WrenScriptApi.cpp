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
#include <Graphics/Vulkan/DisplayBus.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM* vm, int slot);
void   wrenSetSlotDouble(WrenVM* vm, int slot, double value);

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	// clang-format off
	ET_IMPLEMENT_WREN_CLASS(PlayerView) {
		api.CreateClass<PlayerView>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "PlayerView",
			{/* Constructors */
				ConstructorMethod("new(_,_)", [](WrenVM* vm) {
					PlayerView& view(SetReturn<PlayerView>(vm, Angle(wrenGetSlotDouble(vm, 3))));

				//	ET_ABORT_WREN_UNLESS(GetSlotAs<DisplayBus>(vm, 2).TryAcquireDisplay("", view), "Error acquiring display.");
				})
			},
			{/*	Static methods */
				StaticMethod("getVerticalFov(_)", [](WrenVM* vm) {
					wrenSetSlotDouble(vm, 0, (16.0 / 9.0) * wrenGetSlotDouble(vm, 1));
				})
			},
			{/*	Properties */
				DefineProperty("localToWorld",
				//	Getter
					[](WrenVM* vm) {
						const PlayerView&	self(GetSlotAs<PlayerView>(vm, 0));

						wrenSetSlotHandle(vm, 0, AsContext(vm).FindForeignClass<Transformation>());
						SetReturn<Transformation>(vm, self.GetLocalToWorld());
					},
				//	Setter
					[](WrenVM* vm) {
						GetSlotAs<PlayerView>(vm, 0).SetLocalToWorld(GetSlotAs<Transformation>(vm, 1));
					}
				),
				DefineGetter("worldToLocal", [](WrenVM* vm) {
					const PlayerView&	self(GetSlotAs<PlayerView>(vm, 0));

					wrenSetSlotHandle(vm, 0, AsContext(vm).FindForeignClass<Transformation>());
					SetReturn<Transformation>(vm, self.GetLocalToWorld().GetInverse());
				}),
				DefineProperty("verticalFovDegrees",
				//	Getter
					[](WrenVM* vm) {
						wrenSetSlotDouble(vm, 0, DegreesFromAngle(GetSlotAs<PlayerView>(vm, 0).GetVerticalFov()));
					},
				//	Setter
					[](WrenVM* vm) {
						GetSlotAs<PlayerView>(vm, 0).SetVerticalFov(AngleFromDegrees(wrenGetSlotDouble(vm, 1)));
					}
				),
			},
			{/* Methods */ }
		);
	}
	// clang-format on

}}} // namespace Eldritch2::Graphics::Vulkan
