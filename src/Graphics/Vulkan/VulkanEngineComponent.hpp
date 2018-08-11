/*==================================================================*\
  VulkanEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Core/EngineComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Logging {
	class Log;
}} // namespace Eldritch2::Logging

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class VulkanEngineComponent : public Core::EngineComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref VulkanEngineComponent instance.
		VulkanEngineComponent(const ObjectLocator& services, Logging::Log& log);
		//!	Disable copy construction.
		VulkanEngineComponent(const VulkanEngineComponent&) = delete;

		~VulkanEngineComponent() = default;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		UniquePointer<Core::WorldComponent> CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) override;

		void BindConfigurableResources(Scheduling::JobExecutor& executor) override;

		void PublishConfiguration(Core::PropertyRegistrar& properties) override;

		void PublishAssetTypes(Assets::AssetApiBuilder& factories) override;

		void PublishServices(ObjectLocator& services) override;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		void TickEarly(Scheduling::JobExecutor& executor) override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		VulkanEngineComponent& operator=(const VulkanEngineComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Vulkan            _vulkan;
		HashSet<String<>> _instanceLayers;
		HashSet<String<>> _deviceLayers;
		String<>          _preferredGpu;
	};

}}} // namespace Eldritch2::Graphics::Vulkan
