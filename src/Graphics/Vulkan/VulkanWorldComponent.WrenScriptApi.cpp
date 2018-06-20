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
#include <Graphics/Vulkan/DisplayBus.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {
	ET_DECLARE_WREN_CLASS(Armature);
}

namespace Graphics {
	namespace Vulkan {
		ET_DECLARE_WREN_CLASS(VulkanGraphicsScene);
		ET_DECLARE_WREN_CLASS(StaticMeshInstance);
		ET_DECLARE_WREN_CLASS(MeshInstance);
		ET_DECLARE_WREN_CLASS(DisplayBus);
		ET_DECLARE_WREN_CLASS(PlayerView);
		ET_DECLARE_WREN_CLASS(Light);
	} // namespace Vulkan

	ET_DECLARE_WREN_CLASS(RgbColor);
} // namespace Graphics
} // namespace Eldritch2

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Core;

	void VulkanWorldComponent::AcceptVisitor(ApiBuilder& api) {
		ET_REGISTER_WREN_CLASS(VulkanGraphicsScene, api);
		ET_REGISTER_WREN_CLASS(StaticMeshInstance, api);
		ET_REGISTER_WREN_CLASS(MeshInstance, api);
		ET_REGISTER_WREN_CLASS(DisplayBus, api);
		ET_REGISTER_WREN_CLASS(PlayerView, api);
		ET_REGISTER_WREN_CLASS(RgbColor, api);
		ET_REGISTER_WREN_CLASS(Armature, api);
		ET_REGISTER_WREN_CLASS(Light, api);

		Vulkan::Device& device(FindService<Vulkan>().GetPrimaryDevice());

		_scene      = api.CreateVariable<VulkanGraphicsScene>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "GraphicsScene");
		_displayBus = api.CreateVariable<DisplayBus>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "DisplayBus", device.GetDisplayCollection(), device.GetDisplayMutex());
	}

}}} // namespace Eldritch2::Graphics::Vulkan
