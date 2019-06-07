/*==================================================================*\
  Vulkan.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Logging/Log.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#if ET_PLATFORM_WINDOWS
#	include <vulkan/vulkan_win32.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Logging;

	namespace {

		ETForceInlineHint void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* allocator) ETNoexceptHint {
			if (const auto implementation = PFN_vkDestroyDebugReportCallbackEXT(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"))) {
				implementation(instance, callback, allocator);
			}
		}

	} // anonymous namespace

	VkResult Device::BindResources(VkInstance vulkan, VkPhysicalDevice device, VkDeviceSize heapBlockSize) {
		ET_ABORT_UNLESS(Gpu::BindResources(vulkan, device, heapBlockSize, 2u));
		ET_ABORT_UNLESS(ResourceBus::BindResources(*this, /*vertexCacheSize =*/256u * 1024u * 1024u, /*indexCacheSize =*/64u * 1024u * 1024u));
		ET_ABORT_UNLESS(DisplayBus::BindResources(*this));

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Device::FreeResources() {
		DisplayBus::FreeResources(*this);
		ResourceBus::FreeResources(*this);
		Gpu::FreeResources();
	}

	// ---------------------------------------------------

	Vulkan::Vulkan() ETNoexceptHint : _allocator("Vulkan Root Allocator"), _vulkan(NullVulkanHandle), _debugCallback(NullVulkanHandle) {}

	// ---------------------------------------------------

	Vulkan::~Vulkan() {
		ETAssert(_vulkan == NullVulkanHandle, "Leaking Vulkan instance {}!", fmt::ptr(_vulkan));
		ETAssert(_debugCallback == NullVulkanHandle, "Leaking Vulkan log callback {}!", fmt::ptr(_debugCallback));
	}

	// ---------------------------------------------------

	VkResult Vulkan::BindResources(Log& log, uint32 apiVersion, Span<const char* const*> layers) {
		static ETConstexpr auto Callback([](VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*source*/, uint64_t /*object*/, size_t /*location*/, int32_t /*code*/, const char* /*layer*/, const char* message, void* log) ETNoexceptHint -> VkBool32 {
			static_cast<Log*>(log)->Write(flags & VK_DEBUG_REPORT_ERROR_BIT_EXT ? Severity::Error : Severity::Warning, "{}" ET_NEWLINE, message);
			if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
				ET_TRIGGER_DEBUGBREAK();
			}

			return VK_FALSE;
		});

		const auto CreateLogCallback([&](VkInstance instance, VkDebugReportCallbackEXT& callback, const VkAllocationCallbacks* allocator) ETNoexceptHint -> VkResult {
			const VkDebugReportCallbackCreateInfoEXT createInfo{
				VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
				/*pNext =*/nullptr,
				VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
				Callback,
				/*pUserData =*/ETAddressOf(log)
			};

			const auto implementation(PFN_vkCreateDebugReportCallbackEXT(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT")));
			return implementation ? implementation(instance, ETAddressOf(createInfo), allocator, ETAddressOf(callback)) : VK_ERROR_EXTENSION_NOT_PRESENT;
		});

		// ---

		using ::Eldritch2::Swap;

		static InitializerList<const char*> InstanceExtensions = {
#if VK_KHR_surface
			VK_KHR_SURFACE_EXTENSION_NAME,
#endif
#if VK_EXT_debug_report
			VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif
#if VK_KHR_win32_surface
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
		};

		const VkApplicationInfo applicationInfo{
			VK_STRUCTURE_TYPE_APPLICATION_INFO,
			/*pNext =*/nullptr,
			/*applicationName =*/PROJECT_NAME,
			/*applicationVersion =*/1,
			/*pEngineName =*/"Eldritch2",
			/*engineVersion =*/1,
			apiVersion
		};

		VkInstance                 vulkan;
		const VkInstanceCreateInfo vulkanInfo{
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			ETAddressOf(applicationInfo),
			uint32_t(layers.GetSize()),
			layers.Begin(),
			uint32_t(InstanceExtensions.size()),
			Begin(InstanceExtensions)
		};

		ET_ABORT_UNLESS(vkCreateInstance(ETAddressOf(vulkanInfo), _allocator, ETAddressOf(vulkan)), log.Write(Error, "Unable to create Vulkan instance! ({})" ET_NEWLINE, FUNC_RESULT));
		ET_AT_SCOPE_EXIT(vkDestroyInstance(vulkan, _allocator));

		VkDebugReportCallbackEXT debugCallback;
		if (Failed(CreateLogCallback(vulkan, debugCallback, _allocator))) {
			log.Write(Severity::Warning, "Unable to create Vulkan log callback, debug output will be unavailable." ET_NEWLINE);
		}
		ET_AT_SCOPE_EXIT(DestroyDebugReportCallbackEXT(vulkan, debugCallback, _allocator));

		//	First, get the number of installed physical devices in the system.
		uint32 gpuCount(0u);
		ET_ABORT_UNLESS(vkEnumeratePhysicalDevices(vulkan, ETAddressOf(gpuCount), nullptr), log.Write(Error, "Unable to enumerate Vulkan physical devices!" ET_NEWLINE));

		//	Enumerate all attached physical devices.
		VkPhysicalDevice* const devices(ETStackAlloc(VkPhysicalDevice, gpuCount));
		ET_ABORT_UNLESS(vkEnumeratePhysicalDevices(vulkan, ETAddressOf(gpuCount), devices), log.Write(Error, "Unable to retrieve Vulkan physical device handles!" ET_NEWLINE));
		ET_ABORT_UNLESS(_device.BindResources(vulkan, devices[0], Device::HeapBlockSize), log.Write(Error, "Error binding GPU!" ET_NEWLINE));

		//	MicroProfileGpuInitVulkan()
		Swap(_vulkan, vulkan);
		Swap(_debugCallback, debugCallback);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Vulkan::FreeResources() {
		_device.FreeResources();

		DestroyDebugReportCallbackEXT(_vulkan, Exchange(_debugCallback, NullVulkanHandle), _allocator);
		vkDestroyInstance(Exchange(_vulkan, NullVulkanHandle), _allocator);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
