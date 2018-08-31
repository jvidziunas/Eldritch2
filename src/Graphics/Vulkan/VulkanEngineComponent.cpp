/*==================================================================*\
  VulkanEngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanEngineComponent.hpp>
#include <Graphics/Vulkan/VulkanWorldComponent.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Core/PropertyRegistrar.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("vulkan-1.lib")
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Core;

	VulkanEngineComponent::VulkanEngineComponent(const ObjectLocator& services) :
		EngineComponent(services),
		_vulkan(FindService<Engine>()->GetLog()),
		_preferredGpu(MallocAllocator("Vulkan Preferred Adapter Name Allocator")),
		_instanceLayers(MallocAllocator("Vulkan Instance Layers Collection Allocator")),
		_deviceLayers(MallocAllocator("Vulkan Device Layers Collection Allocator")) {
	}

	// ---------------------------------------------------

	UniquePointer<WorldComponent> VulkanEngineComponent::CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) {
		return MakeUnique<VulkanWorldComponent>(allocator, services);
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::BindConfigurableResources(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Initialize Vulkan", 0xBBBBBB);
		if (Failed(_vulkan.BindResources())) {
			FindService<Engine>()->SetShouldShutDown();
			return;
		}
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::PublishConfiguration(PropertyRegistrar& properties) {
		ET_PROFILE_SCOPE("Engine/Initialization/Properties/Vulkan", "Property registration", 0xBBBBBB);
		properties.BeginSection("Vulkan")
			.DefineProperty("PreferredGpu", [this](StringView value) {
				_preferredGpu = value;
			});
		properties.BeginSection("Vulkan.InstanceLayers")
			.DefineDynamicProperty([this](StringView name, StringView /*value*/) {
				_instanceLayers.Emplace(MallocAllocator("Vulkan Instance Layer Name Allocator"), name);
			});
		properties.BeginSection("Vulkan.DeviceLayers")
			.DefineDynamicProperty([this](StringView name, StringView /*value*/) {
				_deviceLayers.Emplace(MallocAllocator("Vulkan Device Layer Name Allocator"), name);
			});
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::PublishServices(ObjectLocator& services) {
		services.PublishService<Vulkan>(_vulkan);
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::TickEarly(JobExecutor& executor) {
		ET_PROFILE_SCOPE("Engine/ServiceTick/Vulkan", "Begin frame", 0xBBBBBB);
		auto& gpu(_vulkan.GetPrimaryDevice());

		executor.AwaitWork(
			[&](JobExecutor& executor) {
				executor.AwaitCondition(gpu.GetTransfersConsumed(gpu));
				gpu.SubmitFrameIo(gpu);
			},
			[&](JobExecutor& /*executor*/) {
				gpu.PresentSwapchainImages(gpu);
				// ET_PROFILE_FRAME_BEGIN_GPU(nullptr);
				gpu.AcquireSwapchainImages(gpu);
			});
	}

}}} // namespace Eldritch2::Graphics::Vulkan
