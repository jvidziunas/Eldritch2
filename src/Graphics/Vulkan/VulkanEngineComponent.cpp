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
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace {

	ETPureFunctionHint VkApplicationInfo MakeApplicationInfo( const char* name = PROJECT_NAME ) {
		return VkApplicationInfo {
			VK_STRUCTURE_TYPE_APPLICATION_INFO,
			nullptr,
			name,
			1,
			"Eldritch2",
			1,
			VK_API_VERSION_1_0
		};
	}

}	// anonymous namespace
	
	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::Eldritch2::Core;

	VulkanEngineComponent::VulkanEngineComponent(
		const Blackboard& services,
		Log& log
	) : EngineComponent( services ),
		_vulkan( log ),
		_instanceLayers( MallocAllocator( "Vulkan Instance Layers Collection Allocator" ) ),
		_deviceLayers( MallocAllocator( "Vulkan Device Layers Collection Allocator" ) ),
		_preferredGpuName( MallocAllocator( "Vulkan Preferred Adapter Name Allocator" ) ) {}

// ---------------------------------------------------

	Result<UniquePointer<WorldComponent>> VulkanEngineComponent::CreateWorldComponent( Allocator& allocator, const World& world ) {
		UniquePointer<WorldComponent> vulkanComponent( CreateUnique<VulkanWorldComponent>( allocator, world ) );
		if (!vulkanComponent) {
			return Error::OutOfMemory;
		}

		return eastl::move( vulkanComponent );
	}

// ---------------------------------------------------

	void VulkanEngineComponent::AcceptVisitor( JobExecutor& executor, const ConfigurationBroadcastVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Initialize Vulkan", 0xBBBBBB );

		if (Failed( _vulkan.BindResources( executor, MakeApplicationInfo() ) )) {
			FindService<Engine>().SetShouldShutDown();
			return;
		}
	}

// ---------------------------------------------------

	void VulkanEngineComponent::AcceptVisitor( PropertyRegistrar& properties ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization/Properties/Vulkan", "Property registration", 0xBBBBBB );

		properties.BeginSection( "Vulkan" )
			.WhenPropertyChanged( "PreferredSingleGpuAdapterName", [this] ( Range<const Utf8Char*> value ) {
				_preferredGpuName.Assign( value.Begin(), value.End() );
			} );

	//	Debug/validation layers.
		properties.BeginSection( "Vulkan.InstanceLayers" )
			.WhenDynamicPropertyChanged( [this] ( const Utf8Char* name, Range<const Utf8Char*> /*value*/ ) {
				_instanceLayers.Emplace( name, MallocAllocator( "Vulkan Instance Layer Name Allocator" ) );
			} );

		properties.BeginSection( "Vulkan.DeviceLayers" )
			.WhenDynamicPropertyChanged( [this] ( const Utf8Char* name, Range<const Utf8Char*> /*value*/ ) {
				_deviceLayers.Emplace( name, MallocAllocator( "Vulkan Device Layer Name Allocator" ) );
			} );
	}

// ---------------------------------------------------

	void VulkanEngineComponent::AcceptVisitor( JobExecutor& executor, const ServiceTickVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/ServiceTick/Vulkan", "Begin frame", 0xBBBBBB );

		_vulkan.BeginFrame( executor );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2