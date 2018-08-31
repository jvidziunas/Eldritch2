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
#include <Build.hpp>
//------------------------------------------------------------------//
#if ET_PLATFORM_WINDOWS
#	include <vulkan/vulkan_win32.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;

	namespace {

		ETInlineHint ETForceInlineHint ETPureFunctionHint Severity AsSeverity(VkDebugReportFlagsEXT flags) {
			return flags & VK_DEBUG_REPORT_ERROR_BIT_EXT ? Severity::Error : Severity::Warning;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT& createInfo, const VkAllocationCallbacks* allocator, VkDebugReportCallbackEXT* callback) {
			const auto implementation(PFN_vkCreateDebugReportCallbackEXT(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT")));
			if (!implementation) {
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}

			return implementation(instance, ETAddressOf(createInfo), allocator, callback);
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* allocator) {
			const auto implementation(PFN_vkDestroyDebugReportCallbackEXT(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT")));
			if (!implementation) {
				return;
			}

			implementation(instance, callback, allocator);
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint VkResult CreateLogCallback(VkInstance instance, Vulkan& vulkan, const VkAllocationCallbacks* allocator, VkDebugReportCallbackEXT* callback) {
			const auto Callback([](VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*source*/, uint64_t /*object*/, size_t /*location*/, int32_t /*code*/, const char* /*layer*/, const char* message, void* vulkan) -> VkBool32 {
				static_cast<Vulkan*>(vulkan)->WriteLog(AsSeverity(flags), "{}" ET_NEWLINE, message);
				if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
					ET_TRIGGER_DEBUGBREAK();
				}

				return VK_FALSE;
			});

			const VkDebugReportCallbackCreateInfoEXT createInfo {
				VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
				/*pNext =*/nullptr,
				VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
				Callback,
				/*pUserData =*/ETAddressOf(vulkan)
			};

			return CreateDebugReportCallbackEXT(instance, createInfo, allocator, callback);
		}

	} // anonymous namespace

	VkResult Vulkan::Device::BindResources(VkInstance vulkan, VkPhysicalDevice device, VkDeviceSize heapBlockSize, VkDeviceSize transferBufferSize) {
		ET_ABORT_UNLESS(Gpu::BindResources(vulkan, device, heapBlockSize, 2u));
		ET_ABORT_UNLESS(ResourceBus::BindResources(*this, transferBufferSize, /*vertexCacheSize =*/256u * 1024u * 1024u, /*indexCacheSize =*/64u * 1024u * 1024u));
		ET_ABORT_UNLESS(DisplayBus::BindResources(*this));

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Vulkan::Device::FreeResources() {
		DisplayBus::FreeResources(*this);
		ResourceBus::FreeResources(*this);
		Gpu::FreeResources();
	}

	// ---------------------------------------------------

	Vulkan::Vulkan(Log& log) :
		_allocator("Vulkan Root Allocator"),
		_log(log),
		_vulkan(nullptr),
		_debugCallback(nullptr) {
	}

	// ---------------------------------------------------

	Vulkan::~Vulkan() {
		ET_ASSERT(_vulkan == nullptr, "Leaking Vulkan instance!");
		ET_ASSERT(_debugCallback == nullptr, "Leaking Vulkan log callback!");
	}

	// ---------------------------------------------------

	VkResult Vulkan::BindResources() {
		using ::Eldritch2::Swap;

		ArrayList<const char*> layers(MallocAllocator("Vulkan Instance Layers List Allocator"), {
#if ET_DEBUG_BUILD
			"VK_LAYER_LUNARG_standard_validation"
#endif
		});

		ArrayList<const char*> extensions(MallocAllocator("Vulkan Instance Extensions List Allocator"), {
#if VK_KHR_surface
			VK_KHR_SURFACE_EXTENSION_NAME,
#endif
#if VK_EXT_debug_report
				VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif
#if VK_KHR_win32_surface
				VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
		});

		const VkApplicationInfo applicationInfo {
			VK_STRUCTURE_TYPE_APPLICATION_INFO,
			/*pNext =*/nullptr,
			/*applicationName =*/PROJECT_NAME,
			/*applicationVersion =*/1,
			/*pEngineName =*/"Eldritch2",
			/*engineVersion =*/1,
			VK_MAKE_VERSION(1, 1, 0)
		};

		VkInstance                 vulkan;
		const VkInstanceCreateInfo vulkanInfo {
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			ETAddressOf(applicationInfo),
			uint32_t(layers.GetSize()),
			layers.GetData(),
			uint32_t(extensions.GetSize()),
			extensions.GetData()
		};

		const VkResult result(vkCreateInstance(ETAddressOf(vulkanInfo), _allocator, ETAddressOf(vulkan)));
		if (Failed(result)) {
			_log.Write(Severity::Error, "Unable to create Vulkan instance! ({})" ET_NEWLINE, result);
			return VK_ERROR_INITIALIZATION_FAILED;
		}
		ET_AT_SCOPE_EXIT(vkDestroyInstance(vulkan, _allocator));

		VkDebugReportCallbackEXT debugCallback;
		if (Failed(CreateLogCallback(vulkan, *this, _allocator, ETAddressOf(debugCallback)))) {
			_log.Write(Severity::Warning, "Unable to create Vulkan log callback, debug output will be unavailable." ET_NEWLINE);
		}
		ET_AT_SCOPE_EXIT(DestroyDebugReportCallbackEXT(vulkan, debugCallback, _allocator));

		//	First, get the number of installed physical devices in the system.
		uint32 gpuCount(0u);
		if (Failed(vkEnumeratePhysicalDevices(vulkan, ETAddressOf(gpuCount), nullptr))) {
			_log.Write(Severity::Error, "Unable to enumerate Vulkan physical devices!" ET_NEWLINE);
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		//	Allocate an array large enough to hold all the physical device handles.
		VkPhysicalDevice* const devices(ETStackAlloc(VkPhysicalDevice, gpuCount));
		//	Fill the array with physical device handles.
		if (Failed(vkEnumeratePhysicalDevices(vulkan, ETAddressOf(gpuCount), devices))) {
			_log.Write(Severity::Error, "Unable to retrieve Vulkan physical device handles!" ET_NEWLINE);
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		VkPhysicalDevice* selectedDevice(devices);
		if (Failed(_device.BindResources(vulkan, *selectedDevice, Device::HeapBlockSize, Device::TransferBufferSize))) {
			_log.Write(Severity::Error, "Error binding GPU!" ET_NEWLINE);
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		//	MicroProfileGpuInitVulkan()
		//	Commit the changes to the object.
		Swap(_vulkan, vulkan);
		Swap(_debugCallback, debugCallback);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Vulkan::FreeResources() {
		_device.FreeResources();

		DestroyDebugReportCallbackEXT(_vulkan, eastl::exchange(_debugCallback, nullptr), _allocator);
		vkDestroyInstance(eastl::exchange(_vulkan, nullptr), _allocator);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
