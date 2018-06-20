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

namespace Eldritch2 {
	namespace Logging {
		class	Log;
	}
}

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {

			class VulkanEngineComponent : public Core::EngineComponent {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
			//!	Constructs this @ref VulkanEngineComponent instance.
				VulkanEngineComponent(const Blackboard& services, Logging::Log& log);
			//!	Disable copy construction.
				VulkanEngineComponent(const VulkanEngineComponent&) = delete;

				~VulkanEngineComponent() = default;

			// - ENGINE SERVICE SANDBOX METHODS ------------------

			public:
				Result<UniquePointer<Core::WorldComponent>>	CreateWorldComponent(Allocator& allocator, const Core::World& world) override;

				void										AcceptVisitor(Scheduling::JobExecutor& executor, const ConfigurationBroadcastVisitor) override;
				void										AcceptVisitor(Scheduling::JobExecutor& executor, const ServiceTickVisitor) override;
				void										AcceptVisitor(Core::PropertyRegistrar& properties) override;
				void										AcceptVisitor(Assets::AssetApiBuilder& factories) override;
				void										AcceptVisitor(Blackboard& services) override;

			// ---------------------------------------------------

			//!	Disable copy assignment.
				VulkanEngineComponent&	operator=(const VulkanEngineComponent&) = delete;

			// - DATA MEMBERS ------------------------------------

			private:
				Vulkan				_vulkan;

				HashSet<String<>>	_instanceLayers;
				HashSet<String<>>	_deviceLayers;
				String<>			_preferredGpuName;
			};

		}	// namespace Vulkan
	}	// namespace Graphics
}	// namespace Eldritch2