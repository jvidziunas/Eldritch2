/*==================================================================*\
  VulkanEngineComponent.WrenScriptApi.cpp
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
#include <Graphics/Vulkan/VulkanEngineComponent.hpp>
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/DisplayLocator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {
	namespace Vulkan {
		ET_DECLARE_WREN_CLASS(VulkanGraphicsScene);
		ET_DECLARE_WREN_CLASS(DisplayLocator);
		ET_DECLARE_WREN_CLASS(MeshInstance);
		ET_DECLARE_WREN_CLASS(RootView);
		ET_DECLARE_WREN_CLASS(Light);
	} // namespace Vulkan

	ET_DECLARE_WREN_CLASS(RgbColor);
}} // namespace Eldritch2::Graphics

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;

	// ---------------------------------------------------

	void VulkanEngineComponent::WorldComponent::PublishApi(ApiBuilder& api) {
		static ETConstexpr StringSpan GraphicsModule(ET_BUILTIN_WREN_MODULE_NAME(Graphics));

		ET_REGISTER_WREN_CLASS(VulkanGraphicsScene, api, GraphicsModule);
		ET_REGISTER_WREN_CLASS(DisplayLocator, api, GraphicsModule);
		ET_REGISTER_WREN_CLASS(MeshInstance, api, GraphicsModule);
		ET_REGISTER_WREN_CLASS(RootView, api, GraphicsModule);
		ET_REGISTER_WREN_CLASS(RgbColor, api, GraphicsModule);
		ET_REGISTER_WREN_CLASS(Light, api, GraphicsModule);

		_displays = api.DefineVariable<DisplayLocator>("Displays", GraphicsModule);
		_scene    = api.DefineVariable<VulkanGraphicsScene>("GraphicsScene", GraphicsModule);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
