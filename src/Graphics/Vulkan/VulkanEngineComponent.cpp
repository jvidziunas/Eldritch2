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
#include <Build.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::Eldritch2::Core;

	VulkanEngineComponent::VulkanEngineComponent(const ObjectLocator& services, Log& log) :
		EngineComponent(services),
		_vulkan(log),
		_instanceLayers(MallocAllocator("Vulkan Instance Layers Collection Allocator")),
		_deviceLayers(MallocAllocator("Vulkan Device Layers Collection Allocator")),
		_preferredGpu(MallocAllocator("Vulkan Preferred Adapter Name Allocator")) {
	}

	// ---------------------------------------------------

	UniquePointer<WorldComponent> VulkanEngineComponent::CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) {
		return MakeUnique<VulkanWorldComponent>(allocator, services);
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::BindConfigurableResources(JobExecutor& executor) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Initialize Vulkan", 0xBBBBBB);
		if (Failed(_vulkan.BindResources(executor, VkApplicationInfo { VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, PROJECT_NAME, 1, "Eldritch2", 1, VK_MAKE_VERSION(1, 1, 0) }))) {
			FindService<Engine>().SetShouldShutDown();
			return;
		}
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::PublishConfiguration(PropertyRegistrar& properties) {
		ET_PROFILE_SCOPE("Engine/Initialization/Properties/Vulkan", "Property registration", 0xBBBBBB);
		properties.BeginSection("Vulkan")
			.DefineProperty("PreferredGpu", [this](StringView<Utf8Char> value) {
				_preferredGpu = value;
			});

		//	Debug/validation layers.
		properties.BeginSection("Vulkan.InstanceLayers")
			.DefineDynamicProperty([this](StringView<Utf8Char> name, StringView<Utf8Char> /*value*/) {
				_instanceLayers.Emplace(name, MallocAllocator("Vulkan Instance Layer Name Allocator"));
			});

		properties.BeginSection("Vulkan.DeviceLayers")
			.DefineDynamicProperty([this](StringView<Utf8Char> name, StringView<Utf8Char> /*value*/) {
				_deviceLayers.Emplace(name, MallocAllocator("Vulkan Device Layer Name Allocator"));
			});
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::PublishServices(ObjectLocator& services) {
		services.PublishService<Vulkan>(_vulkan);
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::TickEarly(JobExecutor& executor) {
		ET_PROFILE_SCOPE("Engine/ServiceTick/Vulkan", "Begin frame", 0xBBBBBB);
		_vulkan.BeginFrame(executor);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
