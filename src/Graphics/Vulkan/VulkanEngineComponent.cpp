/*==================================================================*\
  VulkanEngineComponent.cpp
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
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Core/PropertyApiBuilder.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Core/Profiler.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("vulkan-1.lib")
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	VulkanEngineComponent::VulkanEngineComponent(const ObjectInjector& services) ETNoexceptHint : EngineComponent(services),
																								  _preferredGpu(String::AllocatorType("Vulkan Preferred Adapter Name Allocator")),
																								  _layers(String::AllocatorType("Vulkan Instance Layer Set Allocator")),
																								  _deviceLayers(String::AllocatorType("Vulkan Device Layer Set Allocator")) {
#if ET_DEBUG_BUILD
		_layers.Emplace("VK_LAYER_LUNARG_standard_validation");
#endif
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::BindConfigurableResources(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Initialization", "Initialize Vulkan", 0xBBBBBB);

		const char** layers(ETStackAlloc(const char*, _layers.GetSize()));
		Transform(_layers.Begin(), _layers.End(), layers, [](const String& layer) ETNoexceptHint -> const char* { 
			return layer.AsCString();
		});

		ET_TERMINATE_ENGINE_IF_FAILED(_vulkan.BindResources(*Inject<Log>(), VK_MAKE_VERSION(1u, 1u, 0u), { layers, layers + _layers.GetSize() }));
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::PublishApi(PropertyApiBuilder& api) {
		api.DefineGroup("Vulkan")
			.DefineProperty("PreferredGpu", [this](StringSpan name) ETNoexceptHint { _preferredGpu = name; });
		api.DefineGroup("Vulkan.InstanceLayers")
			.DefineDynamicProperty([this](StringSpan layer, StringSpan value) ETNoexceptHint {
				if (value == "Enabled") {
					_layers.Emplace(String::AllocatorType("Vulkan Instance Layer Name Allocator"), layer);
				} else if (value == "Disabled") {
					_layers.Erase(layer, _layers.GetHash(), _layers.GetEqualityPredicate());
				}
			});
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::PublishApi(ObjectInjector& api) {
		Device& gpu(_vulkan.GetDevice());

		api.PublishService<Gpu>(gpu);
		api.PublishService<ResourceBus>(gpu);
		api.PublishService<DisplayBus>(gpu);
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::TickEarly(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/ServiceTick/Vulkan", "Begin frame", 0xBBBBBB);

		Device& gpu(_vulkan.GetDevice());
		gpu.FlipSwapchainImages(gpu);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
