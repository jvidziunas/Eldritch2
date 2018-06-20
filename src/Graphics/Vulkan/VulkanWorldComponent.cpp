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
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Core;

	VulkanWorldComponent::VulkanWorldComponent(const World& owner) :
		WorldComponent(owner.GetServices()),
		_scene(nullptr),
		_displayBus(nullptr) {}

	// ---------------------------------------------------

	void VulkanWorldComponent::AcceptVisitor(JobExecutor& executor, const LateInitializationVisitor) {
		MICROPROFILE_SCOPEI("World/LateInitialization", "Bind Vulkan scene resources", 0x0C11F0);

		Vulkan::Device& device(FindService<Vulkan>().GetPrimaryDevice());
		if (_scene && Failed(_scene->BindResources(executor, device, device.GetBus()))) {
			return FindService<World>().SetShouldShutDown();
		}
	}

	// ---------------------------------------------------

	void VulkanWorldComponent::AcceptVisitor(JobExecutor& executor, const VariableTickVisitor&) {
		MICROPROFILE_SCOPEI("World/VariableTick", "Draw scene", 0x0C11F0);

		Vulkan::Device& device(FindService<Vulkan>().GetPrimaryDevice());
		_scene->SubmitViewIndependentCommands(executor, device);
		_scene->SubmitViewDependentCommands(executor, device);
	}

	// ---------------------------------------------------

	void VulkanWorldComponent::AcceptVisitor(JobExecutor& executor, const TearDownVisitor) {
		if (_scene) {
			Vulkan::Device& device(FindService<Vulkan>().GetPrimaryDevice());
			_scene->FreeResources(executor, device, device.GetBus());
		}
	}

}}} // namespace Eldritch2::Graphics::Vulkan
