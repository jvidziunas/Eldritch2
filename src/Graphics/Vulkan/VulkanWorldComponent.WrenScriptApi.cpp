/*==================================================================*\
  VulkanWorldComponent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanWorldComponent.hpp>
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/DisplayLocator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {
	ET_DECLARE_WREN_CLASS(Armature);
}

namespace Graphics {
	namespace Vulkan {
		ET_DECLARE_WREN_CLASS(VulkanGraphicsScene);
		ET_DECLARE_WREN_CLASS(StaticMeshInstance);
		ET_DECLARE_WREN_CLASS(DisplayLocator);
		ET_DECLARE_WREN_CLASS(MeshInstance);
		ET_DECLARE_WREN_CLASS(PlayerView);
		ET_DECLARE_WREN_CLASS(Light);
	} // namespace Vulkan

	ET_DECLARE_WREN_CLASS(RgbColor);
} // namespace Graphics
} // namespace Eldritch2

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Animation;

	void VulkanWorldComponent::DefineScriptApi(ApiBuilder& api) {
		ET_REGISTER_WREN_CLASS(VulkanGraphicsScene, api);
		ET_REGISTER_WREN_CLASS(StaticMeshInstance, api);
		ET_REGISTER_WREN_CLASS(DisplayLocator, api);
		ET_REGISTER_WREN_CLASS(MeshInstance, api);
		ET_REGISTER_WREN_CLASS(PlayerView, api);
		ET_REGISTER_WREN_CLASS(RgbColor, api);
		ET_REGISTER_WREN_CLASS(Armature, api);
		ET_REGISTER_WREN_CLASS(Light, api);

		_displays = api.DefineVariable<DisplayLocator>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "Displays");
		_scene    = api.DefineVariable<VulkanGraphicsScene>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "GraphicsScene");
	}

}}} // namespace Eldritch2::Graphics::Vulkan
