/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SparseImageManager.hpp>
#include <Renderer/Vulkan/DeviceMemoryPool.hpp>
#include <Utility/Containers/Utf8String.hpp>
#include <Renderer/Vulkan/SmartPointers.hpp>
#include <Renderer/Vulkan/HostAllocator.hpp>
#include <Platform/UserReadWriteMutex.hpp>
#include <Utility/Containers/ArraySet.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Utility/Containers/HashSet.hpp>
#include <Renderer/Vulkan/Device.hpp>
#include <Core/EngineService.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		namespace Vulkan {
			class	SwapChain;
		}
	}

	namespace Core {
		class	Engine;
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class EngineService : public Core::EngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref EngineService instance.
	/*!	@param[in] engine @ref Engine instance that will manage the lifetime of the new @ref EngineService. */
		EngineService( const Core::Engine& engine );
	//!	Disable copying.
		EngineService( const EngineService& ) = delete;

		~EngineService() = default;

	// - DEBUG/LOGGING INFORMATION -----------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override;

	// ---------------------------------------------------

	public:
		Eldritch2::Result<Eldritch2::UniquePointer<Core::WorldService>>	CreateWorldService( Eldritch2::Allocator& allocator, const Core::World& world ) override;

	// ---------------------------------------------------

	public:
		void	AcceptVisitor( Scheduling::JobFiber& executor, const ConfigurationBroadcastVisitor ) override;
		void	AcceptVisitor( Scheduling::JobFiber& executor, const ServiceTickVisitor ) override;
		void	AcceptVisitor( Configuration::ConfigurationRegistrar& registrar ) override;
		void	AcceptVisitor( Assets::AssetViewFactoryRegistrar& registrar ) override;
		void	AcceptVisitor( Core::ServiceBlackboard& serviceBlackboard ) override;
		void	AcceptVisitor( Scripting::ApiRegistrar& registrar ) override;

	// ---------------------------------------------------

	public:
		Vulkan::HostAllocator&	GetVulkanAllocator() const;

		Eldritch2::Allocator&	GetAllocator() const;

		Vulkan::Device&			GetDeviceForCurrentFrame() const;

		Vulkan::Device&			GetDeviceForNextFrame() const;

		VkInstance				GetVulkan() const;

	// ---------------------------------------------------

	//!	Disable assignment.
		EngineService&	operator=( const EngineService& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Top-level allocator for use by rendering-related operations.
		mutable Vulkan::HostAllocator											_allocator;
	//!	Top-level log for rendering-related operations.
		mutable Logging::ChildLog												_log;

	//	Configuration.
		Eldritch2::HashSet<Eldritch2::Utf8String<>>								_instanceLayers;
		Eldritch2::HashSet<Eldritch2::Utf8String<>>								_deviceLayers;
		Eldritch2::Utf8String<>													_preferredAdapterName;
		bool																	_allowAfrMultiGpu;
		bool																	_installDebugMessageHook;

		Vulkan::UniquePointer<VkInstance>										_vulkan;
		Eldritch2::UniquePointer<Vulkan::Device>								_device;
		Vulkan::UniquePointer<VkDebugReportCallbackEXT>							_debugCallback;

		Eldritch2::HashMap<Eldritch2::Utf8String<>, Vulkan::DeviceMemoryPool>	_imagePools;
		Eldritch2::HashMap<Eldritch2::Utf8String<>, Vulkan::DeviceMemoryPool>	_meshPools;

		Vulkan::SparseImageManager												_sparseImageManager;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/EngineService.inl>
//------------------------------------------------------------------//