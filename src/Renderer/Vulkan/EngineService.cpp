/*==================================================================*\
  EngineService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <packages/ResourceViewFactoryPublishingInitializationVisitor.hpp>
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Utility/Containers/ResizableArray.hpp>
#include <Renderer/Vulkan/VulkanProperties.hpp>
#include <Renderer/Vulkan/VulkanBuilders.hpp>
#include <Renderer/Vulkan/EngineService.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Renderer/Vulkan/WorldView.hpp>
#include <Scheduler/WorkerContext.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <vulkan/vulkan.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "vulkan-1.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	EngineService::EngineService( GameEngine& owningEngine ) : GameEngineService( owningEngine ),
															   _allocator( GetEngineAllocator(), UTF8L("Vulkan Root Allocator") ),
															   _vulkan( nullptr, { static_cast<const VkAllocationCallbacks*>(_allocator) } ),
															   _enabledInstanceLayers( GetEngineAllocator() ),
															   _enabledDeviceLayers( GetEngineAllocator() ),
															   _preferredAdapterNameForSingleGpu( GetEngineAllocator() ),
															   _allowAfrMultiGpu( false ) {}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const {
		return UTF8L("Vulkan Renderer");
	}

// ---------------------------------------------------

	ErrorCode EngineService::AllocateWorldView( Allocator& allocator, World& world ) {
		return new(allocator, Allocator::AllocationDuration::Normal) WorldView( world ) ? Error::None : Error::OutOfMemory;
	}

// ---------------------------------------------------

	void EngineService::OnEngineConfigurationBroadcast( WorkerContext& /*executingContext*/ ) {
		GetLogger()( UTF8L("Creating Vulkan core objects.") ET_UTF8_NEWLINE_LITERAL );

		{	// Create the core Vulkan instance.
			auto	createInstanceResult( VulkanBuilder( _allocator ).Create( _allocator ) );

			if( !createInstanceResult ) {
				GetLogger( LogMessageType::Error )( UTF8L("Unable to create Vulkan instance!") ET_UTF8_NEWLINE_LITERAL );
				return;
			}

			_vulkan = ::std::move( createInstanceResult.object );
		}

		PopulateDevicesSingleGpu();

		GetLogger()( UTF8L("Vulkan core objects created successfully.") ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptApiRegistrationInitializationVisitor& /*visitor*/ ) {
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("Vulkan") );

		visitor.Register( UTF8L("EnabledInstanceLayers"), _enabledInstanceLayers ).Register( UTF8L("EnabledDeviceLayers"), _enabledDeviceLayers );
		visitor.Register( UTF8L("PreferredAdapterNameForSingleGpu"), _preferredAdapterNameForSingleGpu ).Register( UTF8L("AllowAfrMultiGpu"), _allowAfrMultiGpu );

		_pipelineFactory.AcceptInitializationVisitor( visitor );
		_imageFactory.AcceptInitializationVisitor( visitor );
		_meshFactory.AcceptInitializationVisitor( visitor );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( _pipelineFactory.GetSerializedDataTag(), _pipelineFactory );
		visitor.PublishFactory( _imageFactory.GetSerializedDataTag(), _imageFactory );
		visitor.PublishFactory( _meshFactory.GetSerializedDataTag(), _meshFactory );
	}

// ---------------------------------------------------

	void EngineService::OnServiceTickStarted( WorkerContext& /*executingContext*/ ) {
	}

// ---------------------------------------------------

	void EngineService::PopulateDevicesSingleGpu() {
		GetLogger()( UTF8L("Creating Vulkan logical device.") ET_UTF8_NEWLINE_LITERAL );

		PhysicalDeviceEnumerator	enumerator( _vulkan.get(), _allocator );

		// If the user specifies, filter out all devices not matching the preferred adapter name.
		if( _preferredAdapterNameForSingleGpu ) {
			const auto	preferredDeviceName( _preferredAdapterNameForSingleGpu.GetCharacterArray() );

			enumerator.FilterDevices( [preferredDeviceName] ( const Vulkan::PhysicalDeviceProperties& deviceProperties ) {
				return deviceProperties.DescribesNamedDevice( preferredDeviceName );
			} );
		}

		if( const auto desiredPhysicalDevice = enumerator.GetTopDevice() ) {
			auto	createDeviceResult( LogicalDeviceBuilder( _allocator ).Create( desiredPhysicalDevice, _allocator ) );

			if( createDeviceResult ) {
				const auto	deviceContext( new(_allocator, Allocator::AllocationDuration::Normal) DeviceContext( ::std::move( createDeviceResult.object ), _allocator ) );

				_imageFactory.CreateDeviceResources( *deviceContext );
				_meshFactory.CreateDeviceResources( *deviceContext );
			}
		}
	}

// ---------------------------------------------------

	void EngineService::PopulateDevicesAfr() {
		GetLogger()( UTF8L("Creating Vulkan logical device (AFR).") ET_UTF8_NEWLINE_LITERAL );

		PhysicalDeviceEnumerator	enumerator( _vulkan.get(), _allocator );

		// Remove all devices that do not support necessary features.
		enumerator.FilterDevices( [] ( const Vulkan::PhysicalDeviceProperties& deviceProperties ) {
			return deviceProperties.IsDiscreteGpu();
		} );
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

