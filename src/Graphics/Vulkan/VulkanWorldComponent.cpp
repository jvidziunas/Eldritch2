/*==================================================================*\
  VulkanWorldComponent.cpp
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
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Core;

	VulkanWorldComponent::VulkanWorldComponent(const ObjectLocator& services) :
		WorldComponent(services),
		_scene(nullptr),
		_displays(nullptr) {}

	// ---------------------------------------------------

	void VulkanWorldComponent::BindResources(JobExecutor& executor) {
		ET_PROFILE_SCOPE("World/LateInitialization", "Bind Vulkan scene resources", 0x0C11F0);
		Vulkan::Device& device(FindService<Vulkan>().GetPrimaryDevice());

		if (_scene == nullptr || Failed(_scene->BindResources(executor, device, device.GetTransferBus()))) {
			return FindService<World>().SetShouldShutDown();
		}
	}

	// ---------------------------------------------------

	void VulkanWorldComponent::FreeResources(JobExecutor& executor) {
		if (VulkanGraphicsScene* const scene = eastl::exchange(_scene, nullptr)) {
			Vulkan::Device& device(FindService<Vulkan>().GetPrimaryDevice());

			scene->FreeResources(executor, device, device.GetTransferBus());
		}
	}

	// ---------------------------------------------------

	void VulkanWorldComponent::OnVariableRateTick(Scheduling::JobExecutor& executor, MicrosecondTime /*tickDuration*/, float32 /*residualFraction*/) {
		ET_PROFILE_SCOPE("World/VariableTick", "Draw scene", 0x0C11F0);
		Vulkan::Device& device(FindService<Vulkan>().GetPrimaryDevice());

		_scene->SubmitViewIndependentCommands(executor, device);
		_scene->SubmitViewDependentCommands(executor, device);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
