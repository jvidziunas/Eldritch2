/*==================================================================*\
  VulkanGraphicsScene.RootView.WrenScriptApi.cpp
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
#include <Graphics/Vulkan/DisplayLocator.hpp>
#include <Scripting/Wren/ScriptExecutor.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM*, int);
void   wrenSetSlotDouble(WrenVM*, int, double);

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;

	// ---------------------------------------------------

	ET_IMPLEMENT_WREN_CLASS(RootView, api, moduleName) {
		api.DefineClass<RootView>("RootView", moduleName, // clang-format off
			{/*	Static methods */
				ForeignMethod("new(_,_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					auto viewport(wrenGetSlotAs<DisplayLocator>(vm, 1).TryAcquireViewport(wrenGetSlotAs<VulkanGraphicsScene>(vm, 2).GetLitPipelineBuilder()));
					ET_ABORT_WREN_IF(viewport == ViewportId::Invalid, vm, "Error acquiring display.");

					wrenSetReturn<RootView>(vm, /*classSlot =*/0, Move(viewport), wrenGetSlotAs<Transformation>(vm, 3), Angle(float32(wrenGetSlotDouble(vm, 4))));
				}),
				ForeignMethod("getVerticalFov(_)", [](WrenVM* vm) ETNoexceptHint {
					static ETConstexpr float32 WidescreenAspect(16.0f / 9.0f);
					wrenSetSlotDouble(vm, 0, WidescreenAspect * float32(wrenGetSlotDouble(vm, 1)));
				}), },
			{/* Methods */
				ForeignMethod("worldToView=(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenGetSlotAs<RootView>(vm, 0).SetWorldToView(wrenGetSlotAs<Transformation>(vm, 1));
				}),
				ForeignMethod("worldToView", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Transformation>(vm, wrenGetSlotAs<RootView>(vm, 0).GetWorldToView());
				}),
				ForeignMethod("viewToWorld", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Transformation>(vm, ~wrenGetSlotAs<RootView>(vm, 0).GetWorldToView());
				}),
				ForeignMethod("verticalFovDegrees=(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenGetSlotAs<RootView>(vm, 0).SetVerticalFov(AsDegreeAngle(float32(wrenGetSlotDouble(vm, 1))));
				}),
				ForeignMethod("verticalFovDegrees", [](WrenVM* vm) ETNoexceptHint {
					wrenSetSlotDouble(vm, 0, AsDegrees(wrenGetSlotAs<RootView>(vm, 0).GetVerticalFov()));
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Graphics::Vulkan
