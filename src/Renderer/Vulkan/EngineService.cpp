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
#include <Renderer/Vulkan/AssetViews/SpirVShaderPipelineView.hpp>
#include <Configuration/ConfigurationRegistrar.hpp>
#include <Renderer/Vulkan/AssetViews/ImageView.hpp>
#include <Renderer/Vulkan/AssetViews/MeshView.hpp>
#include <Renderer/Vulkan/DeviceEnumerator.hpp>
#include <Assets/AssetViewFactoryRegistrar.hpp>
#include <Renderer/Vulkan/ObjectBuilders.hpp>
#include <Renderer/Vulkan/EngineService.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Renderer/Vulkan/WorldService.hpp>
#include <Core/ServiceBlackboard.hpp>
#include <Core/Engine.hpp>
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
using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::Eldritch2::Core;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace {

	static ETInlineHint VulkanResult<VkDebugReportCallbackEXT> RegisterDebugCallback( const UniquePointer<VkInstance>& instance, const VkAllocationCallbacks* allocationCallbacks, Log& log ) {
	//	Query the Vulkan instance for debug callback handler support. We will receive an installation callback if the driver supports reporting functionality, or a null pointer if the functionality is unavailable.
		const auto	createDebugCallbackFunc( PFN_vkCreateDebugReportCallbackEXT( vkGetInstanceProcAddr( instance.Get(), "vkCreateDebugReportCallbackEXT" ) ) );
		if( nullptr == createDebugCallbackFunc ) {
			return VkResult::VK_ERROR_FEATURE_NOT_PRESENT;
		}

	//	At this point we know the driver supports debug output capture; build and submit the callback.
		VkDebugReportCallbackCreateInfoEXT	callbackCreateInfo;
		VkDebugReportCallbackEXT			callback;

		callbackCreateInfo.sType		= VkStructureType::VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		callbackCreateInfo.pNext		= nullptr;
		callbackCreateInfo.flags		= VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_WARNING_BIT_EXT | VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		callbackCreateInfo.pUserData	= &log;
		callbackCreateInfo.pfnCallback	= [] ( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*objectType*/, uint64_t /*object*/, size_t /*location*/, int32_t /*messageCode*/, const char* layerPrefix, const char* message, void* log ) -> VkBool32 {
			const MessageSeverity	severity( flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT ? MessageSeverity::Error : MessageSeverity::Warning );

			(*static_cast<Log*>(log))( severity, "Vulkan: {} {}" ET_UTF8_NEWLINE_LITERAL, layerPrefix, message );
			return VK_FALSE;
		};

	//	Install the callback and go!
		const VkResult	installCallbackResult( createDebugCallbackFunc( instance.Get(), &callbackCreateInfo, allocationCallbacks, &callback ) );
		if( VkResult::VK_SUCCESS != installCallbackResult ) {
			return installCallbackResult;
		}

		return Vulkan::UniquePointer<VkDebugReportCallbackEXT>( callback, { instance.Get(), allocationCallbacks } );
	}

// ---------------------------------------------------

	static ETInlineHint void DescribeDevice( VkPhysicalDevice device, Log& log ) {
		const PhysicalDeviceProperties	deviceProperties( device );

		log( MessageSeverity::Message, "Using Vulkan device '{}' on driver version {}." ET_UTF8_NEWLINE_LITERAL, deviceProperties.properties.deviceName, deviceProperties.properties.driverVersion );
	}

// ---------------------------------------------------

	static ETInlineHint Result<Eldritch2::UniquePointer<Device>> CreateDeviceContext( VkInstance instance, Log& log, const Utf8Char* preferredAdapterName, HostAllocator& allocator ) {
		DeviceEnumerator		enumerator( instance, allocator );
		LogicalDeviceBuilder	deviceBuilder( allocator );

		log( MessageSeverity::Message, "Creating Vulkan logical device." ET_UTF8_NEWLINE_LITERAL );
	
		if( *preferredAdapterName != '\0' ) {
		//	Filter out all devices not matching the preferred adapter name.
			enumerator.FilterUnless( [preferredAdapterName] ( const PhysicalDeviceProperties& deviceProperties ) {
				return deviceProperties.DescribesNamedDevice( preferredAdapterName );
			} );
		}

	//	Select the most suitable Vulkan physical device according to the criteria we have just specified.
		const VkPhysicalDevice	physicalDevice( enumerator.GetTopDevice() );
		if( VK_NULL_HANDLE == physicalDevice ) {
			log( MessageSeverity::Error, "No suitable Vulkan physical devices found!" ET_UTF8_NEWLINE_LITERAL );
			return Error::UnsupportedOperation;
		}

		DescribeDevice( physicalDevice, log );

	//	Enable the device extensions we need.
		SurfaceBuilder::EnableExtensions( deviceBuilder );
		SwapChainBuilder::EnableExtensions( deviceBuilder );

		VulkanResult<VkDevice> createDeviceResult( deviceBuilder.Create( physicalDevice, allocator ) );
		if( !createDeviceResult ) {
			log( MessageSeverity::Error, "Unable to allocate Vulkan logical device!" ET_UTF8_NEWLINE_LITERAL );
			return Error::UnsupportedOperation;
		}

		auto	result( MakeUnique<Device>( allocator, physicalDevice, eastl::move( *createDeviceResult ), allocator ) );
		if( !result ) {
			log( MessageSeverity::Error, "Unable to allocate Vulkan device context object!" ET_UTF8_NEWLINE_LITERAL );
			return Error::UnsupportedOperation;
		}

		log( MessageSeverity::Message, "Vulkan device created successfully." ET_UTF8_NEWLINE_LITERAL );

		return eastl::move( result );
	}

}	// anonymous namespace

	EngineService::EngineService( const Engine& engine ) : Core::EngineService( engine.GetServiceBlackboard() ),
														   _allocator( engine.GetAllocator(), "Vulkan Root Allocator" ),
														   _log( engine.GetLog() ),
														   _instanceLayers( { _allocator, "Vulkan Enabled Instance Layers Collection Allocator" } ),
														   _deviceLayers( { _allocator, "Vulkan Enabled Device Layers Collection Allocator" } ),
														   _preferredAdapterName( { _allocator, "Vulkan Preferred Adapter Name Allocator" } ),
														   _allowAfrMultiGpu( false ),
														   _installDebugMessageHook( false ),
														   _vulkan( nullptr ),
														   _device( nullptr ),
														   _debugCallback( nullptr ),
														   _imagePools( { _allocator, "Vulkan Renderer Image Pool Bucket Allocator" } ),
														   _meshPools( { _allocator, "Vulkan Renderer Mesh Pool Bucket Allocator" } ),
														   _sparseImageManager( _allocator ) {}

// ---------------------------------------------------

	Utf8Literal EngineService::GetName() const {
		return "Vulkan Renderer";
	}

// ---------------------------------------------------

	Result<Eldritch2::UniquePointer<Core::WorldService>> EngineService::CreateWorldService( Allocator& allocator, const World& world ) {
		auto result( MakeUnique<Vulkan::WorldService>( allocator, world ) );
		if( !result ) {
			return Error::OutOfMemory;
		}

		return eastl::move( result );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const ConfigurationBroadcastVisitor ) {
		InstanceBuilder	instanceBuilder( _allocator );

	//	Enable the instance extensions we need.
		SurfaceBuilder::EnableExtensions( instanceBuilder );
		SwapChainBuilder::EnableExtensions( instanceBuilder );

		if( _installDebugMessageHook ) {
			instanceBuilder.EnableExtension( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
		}

	//	Enable the instance layers we need.
		for( const Utf8String<>& layer : _instanceLayers ) {
			instanceBuilder.EnableLayer( layer );
		}

		_log( MessageSeverity::Message, "Creating Vulkan core objects." ET_UTF8_NEWLINE_LITERAL );

	//	Create the core Vulkan instance.
		VulkanResult<VkInstance> createVulkanResult( instanceBuilder.Create( _allocator ) );
		if( !createVulkanResult ) {
			_log( MessageSeverity::Error, "Unable to create Vulkan instance! ({})" ET_UTF8_NEWLINE_LITERAL, static_cast<VkResult>(createVulkanResult) );
			return;
		}

	//	Create support objects for the device.
		VulkanResult<VkDebugReportCallbackEXT>	createCallbackResult( _installDebugMessageHook ? RegisterDebugCallback( createVulkanResult->Get(), _allocator, _log ) : VulkanResult<VkDebugReportCallbackEXT>( UniquePointer<VkDebugReportCallbackEXT>(nullptr) ) );
		if( !createCallbackResult ) {
			_log( MessageSeverity::Warning, "Unable to create Vulkan debug callback, reporting services will be unavailable. ({})" ET_UTF8_NEWLINE_LITERAL );
		}

		Result<Eldritch2::UniquePointer<Device>>	createDeviceResult( CreateDeviceContext( createVulkanResult->Get(), _log, _preferredAdapterName, _allocator ) );
		if( !createDeviceResult ) {
			_log( MessageSeverity::Error, "Unable to create Vulkan device context! ({})" ET_UTF8_NEWLINE_LITERAL, GetErrorString( createDeviceResult ) );
			return;
		}

		_log( MessageSeverity::Message, "Vulkan core objects created successfully." ET_UTF8_NEWLINE_LITERAL );

	//	Commit the changes to the engine service.
		_vulkan			= eastl::move( *createVulkanResult );
		_device			= eastl::move( *createDeviceResult );
		_debugCallback	= eastl::move( *createCallbackResult );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ApiRegistrar& visitor ) {
		WorldService::RegisterScriptApi( visitor );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ConfigurationRegistrar& visitor ) {
		visitor.BeginSection( "Vulkan" )
			.Register( "PreferredSingleGpuAdapterName", [this] ( Range<const Utf8Char*> value ) { _preferredAdapterName.Assign( value.Begin(), value.End() ); } )
			.Register( "AllowAfrMultiGpu", _allowAfrMultiGpu )
			.Register( "InstallDebugMessageHook", _installDebugMessageHook );

	//	Debug/validation layers.
		visitor.BeginSection( "Vulkan.InstanceLayers" )
			.RegisterDynamicKeySetter( [this] ( Utf8Literal name, Range<const Utf8Char*> /*value*/ ) {
				Utf8String<>	layerName( name, { _allocator, "Vulkan Instance Layer Name Allocator" } );
			
				_instanceLayers.Insert( eastl::move( layerName ) );
			} );
		visitor.BeginSection( "Vulkan.DeviceLayers" )
			.RegisterDynamicKeySetter( [this] ( Utf8Literal name, Range<const Utf8Char*> /*value*/ ) {
				Utf8String<>	layerName( name, { _allocator, "Vulkan Device Layer Name Allocator" } );
			
				_deviceLayers.Insert( eastl::move( layerName ) );
			} );

	//	Device memory pools.
		visitor.BeginSection( "Vulkan.ImagePools" )
			.RegisterDynamicKeySetter( [this] ( Utf8Literal name, Range<const Utf8Char*> /*value*/ ) {
				Utf8String<>	poolName( name, { _allocator, "Vulkan Image Pool Name Allocator" } );
			
				// _imagePools.Emplace( eastl::move( poolName ), 0 );
			} );
		visitor.BeginSection( "Vulkan.MeshPools" )
			.RegisterDynamicKeySetter( [this] ( Utf8Literal name, Range<const Utf8Char*> /*value*/ ) {
				Utf8String<>	poolName( name, { _allocator, "Vulkan Mesh Pool Name Allocator" } );
			
				// _meshPools.Emplace( eastl::move( poolName ), 0 );
			} );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( AssetViewFactoryRegistrar& visitor ) {
		visitor.Publish( AssetViews::ImageView::GetExtension(), [this] ( Allocator& allocator, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) -> Result<Eldritch2::UniquePointer<AssetView>> {
			const auto poolCandidate( _imagePools.Find( "default", Eldritch2::Hash<const Utf8Char*>(), eastl::equal_to_2<const Utf8String<>, const Utf8Char*>() ) );

			if( poolCandidate == _imagePools.End() ) {
				return Error::OutOfMemory;
			}

			return AssetViews::ImageView::CreateView( allocator, *_device, poolCandidate->second, library, name, rawBytes );
		} )
		.Publish( AssetViews::MeshView::GetExtension(), [this] ( Allocator& allocator, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) -> Result<Eldritch2::UniquePointer<AssetView>> {
			const auto poolCandidate( _meshPools.Find( "default", Eldritch2::Hash<const Utf8Char*>(), eastl::equal_to_2<const Utf8String<>, const Utf8Char*>() ) );

			if( poolCandidate == _meshPools.End() ) {
				return Error::OutOfMemory;
			}

			return AssetViews::MeshView::CreateView( allocator, *_device, poolCandidate->second, library, name, rawBytes );
		} )
		.Publish( AssetViews::SpirVShaderPipelineView::GetExtension(), [this] ( Allocator& allocator, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
			return AssetViews::SpirVShaderPipelineView::CreateView( allocator, *_device, library, name, rawBytes );
		} );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ServiceBlackboard& blackboard ) {
		blackboard.Publish( _sparseImageManager );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const ServiceTickVisitor ) {
		enum : uint64 {
			FenceAcquisitionTimeoutInNs		= 60000000u,
			PresentAcquisitionTimeoutInNs	= FenceAcquisitionTimeoutInNs
		};

	// ---

		Device&	device( GetDeviceForCurrentFrame() );

#if 0
		{	MICROPROFILE_SCOPEI( GetName(), "Flip swap chains", 0x0FAFAF );
		//	Loop through all active swap chains, performing a buffer flip/recreate operation on each.
			for( const auto swapChain : _swapChains ) {
				swapChain->OnFrameBegin( deviceContext, VK_NULL_HANDLE, PresentAcquisitionTimeoutInNs, GetVulkanAllocator() );
			}
		}
#endif
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

