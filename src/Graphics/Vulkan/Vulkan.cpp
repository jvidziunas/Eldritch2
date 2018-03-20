/*==================================================================*\
  Vulkan.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Scheduling/JobExecutor.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "vulkan-1.lib" )
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace {

	using namespace ::Eldritch2::Logging;

	static ETInlineHint ETPureFunctionHint MessageType GetMessageType( VkDebugReportFlagsEXT flags ) {
		return flags & VK_DEBUG_REPORT_ERROR_BIT_EXT ? MessageType::Error : MessageType::Warning;
	}

// ---------------------------------------------------

	static ETInlineHint VkResult CreateDebugReportCallbackEXT( VkInstance instance, const VkDebugReportCallbackCreateInfoEXT& createInfo, const VkAllocationCallbacks* allocator, VkDebugReportCallbackEXT* callback ) {
		const auto	implementation( reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr( instance, "vkCreateDebugReportCallbackEXT" )) );
		if (!implementation) {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}

		return implementation( instance, &createInfo, allocator, callback );
	}

// ---------------------------------------------------

	static ETInlineHint void DestroyDebugReportCallbackEXT( VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* allocator ) {
		const auto destroyCallback( reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr( instance, "vkDestroyDebugReportCallbackEXT" )) );

		if (!destroyCallback) {
			return;
		}

		destroyCallback( instance, callback, allocator );
	}

// ---------------------------------------------------

	static ETInlineHint VkResult CreateLogCallback( VkInstance instance, Vulkan& vulkan, const VkAllocationCallbacks* allocator, VkDebugReportCallbackEXT* callback ) {
		const VkDebugReportCallbackCreateInfoEXT	createInfo{
			VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
			nullptr,
			VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
			[] ( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*objectType*/, uint64_t /*object*/, size_t /*location*/, int32_t /*messageCode*/, const char* layer, const char* message, void* vulkan ) -> VkBool32 {	
				static_cast<Vulkan*>(vulkan)->WriteLog( GetMessageType( flags ), "{} [{}]" UTF8_NEWLINE, message, layer );

				ET_TRIGGER_DEBUGBREAK();

				return VK_FALSE;
			},
			&vulkan
		};

		return CreateDebugReportCallbackEXT( instance, createInfo, allocator, callback );
	}

}	// anonymous namespace

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;

	VkResult Vulkan::Device::BindResources( Vulkan& /*vulkan*/, VkPhysicalDevice device, VkDeviceSize transferBufferSize ) {
		ET_FAIL_UNLESS( Gpu::BindResources( device ) );
		ET_FAIL_UNLESS( ResidencyCoordinator::BindResources( *this, transferBufferSize ) );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void Vulkan::Device::FreeResources() {
		ResidencyCoordinator::FreeResources( *this );
		Gpu::FreeResources();
	}

// ---------------------------------------------------

	Vulkan::Vulkan(
		Log& log
	) : _allocator( "Vulkan Root Allocator" ),
		_log( log ),
		_vulkan( nullptr ),
		_debugCallback( nullptr ),
		_deviceCount( 0u ) {
	}

// ---------------------------------------------------

	Vulkan::~Vulkan() {
		ET_ASSERT( _vulkan == nullptr,        "Leaking Vulkan instance!" );
		ET_ASSERT( _debugCallback == nullptr, "Leaking Vulkan log callback!" );
		ET_ASSERT( _deviceCount == 0,         "Leaking Vulkan GPUs!" );
	}

// ---------------------------------------------------

	void Vulkan::BeginFrame( JobExecutor& executor ) {
		executor.ForEach<1u>( _devices, ( _devices + _deviceCount ), [] ( JobExecutor& executor, Device& device ) {
			executor.AwaitWork(
				[&] ( JobExecutor&/* executor*/ ) {
					device.SubmitFrameIo( executor, device );
				},
				[&] ( JobExecutor&/* executor*/ ) {
					device.Present( device );
				}
			);
		} );
	}

// ---------------------------------------------------

	VkResult Vulkan::BindResources( JobExecutor& executor, const VkApplicationInfo& applicationInfo ) {
		using ::Eldritch2::Swap;

		const std::initializer_list<const char*> layers = {
			"VK_LAYER_LUNARG_standard_validation"
		};

		const std::initializer_list<const char*> extensions = {
			VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
			VK_KHR_SURFACE_EXTENSION_NAME,
	#	if ET_PLATFORM_WINDOWS
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	#	endif
		};

		VkInstance	vulkan;
		{	const VkInstanceCreateInfo	instanceInfo{
				VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				nullptr,
				0,
				&applicationInfo,
				static_cast<uint32_t>(layers.size()), layers.begin(),
				static_cast<uint32_t>(extensions.size()), extensions.begin()
			};
		
			const VkResult	result( vkCreateInstance( &instanceInfo, _allocator, &vulkan ) );
			if (Failed( result )) {
				WriteLog( MessageType::Error, "Unable to create Vulkan instance! ({})" UTF8_NEWLINE, result );
				return VK_ERROR_INITIALIZATION_FAILED;
			}
		}
		ET_AT_SCOPE_EXIT( vkDestroyInstance( vulkan, _allocator ) );

		VkDebugReportCallbackEXT debugCallback;
		if (Failed( CreateLogCallback( vulkan, *this, _allocator, &debugCallback ) )) {
			WriteLog( MessageType::Warning, "Unable to create Vulkan log callback, debug output will be unavailable." UTF8_NEWLINE );
		}
		ET_AT_SCOPE_EXIT( DestroyDebugReportCallbackEXT( vulkan, debugCallback, _allocator ) );

	//	First, get the number of installed physical devices in the system.
		uint32	gpuCount( 0u );
		if (Failed( vkEnumeratePhysicalDevices( vulkan, &gpuCount, nullptr ) ) || gpuCount == 0u) {
			WriteLog( MessageType::Error, "Unable to enumerate Vulkan physical devices!" UTF8_NEWLINE );
			return VK_ERROR_INITIALIZATION_FAILED;
		}

	//	Allocate an array large enough to hold all the physical device handles.
		VkPhysicalDevice* const	devices( static_cast<VkPhysicalDevice*>(_alloca( gpuCount * sizeof(VkPhysicalDevice) )) );
	//	Fill the array with physical device handles.
		if (Failed( vkEnumeratePhysicalDevices( vulkan, &gpuCount, devices ) )) {
			WriteLog( MessageType::Error, "Unable to query Vulkan physical devices!" UTF8_NEWLINE );
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		gpuCount = Min<uint32>( gpuCount, _countof(_devices) );

		executor.ForEach<1u>( _devices, ( _devices + gpuCount ), devices, [this] ( JobExecutor& /*executor*/, Device& gpu, VkPhysicalDevice physicalDevice ) {
			if (Failed( gpu.BindResources( *this, physicalDevice, Device::TransferBufferSize ) )) {
				WriteLog( MessageType::Error, "Error binding GPU!" UTF8_NEWLINE );
			}
		} );

	//	Commit the changes to the object.
		Swap( _vulkan,			vulkan );
		Swap( _debugCallback,	debugCallback );

		_deviceCount = gpuCount;

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void Vulkan::FreeResources( JobExecutor& executor ) {
		executor.ForEach<1u>( _devices, ( _devices + eastl::exchange( _deviceCount, 0u ) ), [] ( JobExecutor& /*executor*/, Device& gpu ) {
			gpu.FreeResources();
		} );

		DestroyDebugReportCallbackEXT( _vulkan, eastl::exchange( _debugCallback, nullptr ), _allocator );
		vkDestroyInstance( eastl::exchange( _vulkan, nullptr ), _allocator );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2