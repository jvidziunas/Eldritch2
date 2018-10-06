/*==================================================================*\
  Gpu.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		template <typename InputIterator, size_t count>
		ETInlineHint ETPureFunctionHint VkResult FindQueueFamily(uint32& outFamily, InputIterator begin, InputIterator end, const VkQueueFlags (&flags)[count]) {
			for (VkQueueFlags desiredFlags : flags) {
				//	Try to find a dedicated match.
				const auto candidate(FindIf(begin, end, [desiredFlags](const VkQueueFamilyProperties& family) {
					return family.queueFlags == desiredFlags;
				}));
				if (candidate != end) {
					outFamily = uint32(eastl::distance(begin, candidate));
					return VK_SUCCESS;
				}
			}

			for (VkQueueFlags desiredFlags : flags) {
				const auto candidate(FindIf(begin, end, [desiredFlags](const VkQueueFamilyProperties& family) {
					return (family.queueFlags & desiredFlags) == desiredFlags;
				}));
				if (candidate != end) {
					outFamily = uint32(eastl::distance(begin, candidate));
					return VK_SUCCESS;
				}
			}

			return VK_ERROR_FEATURE_NOT_PRESENT;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint VkResult FindQueueFamilies(uint32 (&families)[QueueConcepts], VkPhysicalDevice device) {
			uint32 count(0u);

			vkGetPhysicalDeviceQueueFamilyProperties(device, ETAddressOf(count), nullptr);
			VkQueueFamilyProperties* const begin(ETStackAlloc(VkQueueFamilyProperties, count));
			VkQueueFamilyProperties* const end(begin + count);
			vkGetPhysicalDeviceQueueFamilyProperties(device, ETAddressOf(count), begin);

			ET_ABORT_UNLESS(FindQueueFamily(families[Drawing], begin, end, { VkQueueFlags(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT) }));
			ET_ABORT_UNLESS(FindQueueFamily(families[Presentation], begin, end, { VkQueueFlags(VK_QUEUE_COMPUTE_BIT), VkQueueFlags(VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT) }));
			ET_ABORT_UNLESS(FindQueueFamily(families[SparseBinding], begin, end, { VkQueueFlags(VK_QUEUE_SPARSE_BINDING_BIT | VK_QUEUE_TRANSFER_BIT), VkQueueFlags(VK_QUEUE_SPARSE_BINDING_BIT) }));
			ET_ABORT_UNLESS(FindQueueFamily(families[Transfer], begin, end, { VkQueueFlags(VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT), VkQueueFlags(VK_QUEUE_TRANSFER_BIT) }));
			ET_ABORT_UNLESS(FindQueueFamily(families[BulkComputation], begin, end, { VkQueueFlags(VK_QUEUE_COMPUTE_BIT) }));

			return VK_SUCCESS;
		}

		// ---------------------------------------------------

		ETInlineHint VkResult BuildQueueCreateInfos(VkDeviceQueueCreateInfo (&out)[QueueConcepts], uint32& outQueueCount, VkPhysicalDevice device) {
			uint32 indices[QueueConcepts];

			ET_ABORT_UNLESS(FindQueueFamilies(indices, device));
			Sort(indices, End(indices));
			const auto lastUnique(Unique(indices, End(indices)));

			outQueueCount = uint32(eastl::distance(indices, lastUnique));
			Transform(indices, lastUnique, out, [](uint32 family) {
				static ETConstexpr float32 priorities[] = { 1.0f };
				return VkDeviceQueueCreateInfo {
					VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					/*pNext =*/nullptr,
					/*flags =*/0u,
					family,
					ETCountOf(priorities), priorities
				};
			});

			return VK_SUCCESS;
		}

	} // anonymous namespace

	void Gpu::CommandQueue::BindResources(VkDevice device, uint32_t familyIndex) {
		vkGetDeviceQueue(device, familyIndex, 0u, ETAddressOf(queue));
		family = familyIndex;
	}

	// ---------------------------------------------------

	Gpu::Gpu() :
		_allocator("Gpu Root Allocator"),
		_physicalDevice(nullptr),
		_device(nullptr),
		_pipelineCache(nullptr),
		_gpuAllocator(nullptr),
		_imageGarbage(MallocAllocator("Vulkan GPU Image Garbage List Allocator")),
		_bufferGarbage(MallocAllocator("Vulkan GPU Buffer Garbage List Allocator")),
		_allocationGarbage(MallocAllocator("Vulkan GPU Allocation Garbage List Allocator")) {
	}

	// ---------------------------------------------------

	Gpu::~Gpu() {
		ET_ASSERT(_device == nullptr, "Leaking Vulkan device {}!", fmt::ptr(_device));
		ET_ASSERT(_pipelineCache == nullptr, "Leaking Vulkan pipeline cache {}!", fmt::ptr(_pipelineCache));
		ET_ASSERT(_gpuAllocator == nullptr, "Leaking Vulkan GPU allocator {}!", fmt::ptr(_gpuAllocator));
	}

	// ---------------------------------------------------

	void Gpu::DestroyGarbage() {
		ArrayList<VkImage>       imageGarbage(_imageGarbage.GetAllocator());
		ArrayList<VkBuffer>      bufferGarbage(_bufferGarbage.GetAllocator());
		ArrayList<VmaAllocation> allocationGarbage(_allocationGarbage.GetAllocator());

		{
			Lock _(_garbageMutex);
			Swap(_imageGarbage, imageGarbage);
			Swap(_bufferGarbage, bufferGarbage);
			Swap(_allocationGarbage, allocationGarbage);
		} // End of lock scope.

		for (VkImage garbage : imageGarbage) {
			vkDestroyImage(_device, garbage, GetAllocationCallbacks());
		}

		for (VkBuffer garbage : bufferGarbage) {
			vkDestroyBuffer(_device, garbage, GetAllocationCallbacks());
		}

		for (VmaAllocation garbage : allocationGarbage) {
			// To consider: Can we hoist the scope of this lock out of the loop?
			Lock _(_gpuAllocatorMutex);
			vmaFreeMemory(_gpuAllocator, garbage);
		} // End of lock scope.
	}

	// ---------------------------------------------------

	VkResult Gpu::BindResources(VkInstance vulkan, VkPhysicalDevice physicalDevice, VkDeviceSize heapBlockSize, uint32 frameUseCount) {
		using ::Eldritch2::Swap;

		ArrayList<const char*> enabledExtensions;
#if VK_KHR_swapchain
		enabledExtensions.Append(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
#endif

		uint32                  queueInfoCount(0u);
		VkDeviceQueueCreateInfo queueInfos[QueueConcepts];
		ET_ABORT_UNLESS(BuildQueueCreateInfos(queueInfos, queueInfoCount, physicalDevice));

		VkDevice                 device;
		const VkDeviceCreateInfo deviceInfo {
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			/*queueCreateInfoCount =*/queueInfoCount, queueInfos,
			/*enabledLayerCount =*/0u, /*ppEnabledLayerNames =*/nullptr,
			/*enabledExtensionCount =*/uint32(enabledExtensions.GetSize()), enabledExtensions.GetData(),
			/*pEnabledFeatures =*/nullptr
		};
		ET_ABORT_UNLESS(vkCreateDevice(physicalDevice, ETAddressOf(deviceInfo), _allocator, ETAddressOf(device)));
		ET_AT_SCOPE_EXIT(vkDestroyDevice(device, _allocator));

		VkPipelineCache                 pipelineCache;
		const VkPipelineCacheCreateInfo pipelineCacheInfo {
			VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			/*initialDataSize =*/0u, /*pInitialData =*/nullptr
		};
		ET_ABORT_UNLESS(vkCreatePipelineCache(device, ETAddressOf(pipelineCacheInfo), _allocator, ETAddressOf(pipelineCache)));
		ET_AT_SCOPE_EXIT(if (device) vkDestroyPipelineCache(device, pipelineCache, _allocator););

		uint32 families[QueueConcepts];
		ET_ABORT_UNLESS(FindQueueFamilies(families, physicalDevice));
		for (uint32 concept(0u); concept < ETCountOf(_queues); ++concept) {
			_queues[concept].BindResources(device, families[concept]);
		}

		VmaAllocator                 gpuAllocator;
		const VmaAllocatorCreateInfo gpuAllocatorInfo {
			VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT,
			physicalDevice,
			device,
			heapBlockSize,
			GetAllocationCallbacks(),
			/*pDeviceMemoryCallbacks =*/nullptr,
			frameUseCount,
			/*pHeapSizeLimit =*/nullptr,
			/*pVulkanFunctions =*/nullptr
		};
		ET_ABORT_UNLESS(vmaCreateAllocator(ETAddressOf(gpuAllocatorInfo), ETAddressOf(gpuAllocator)));
		ET_AT_SCOPE_EXIT(vmaDestroyAllocator(gpuAllocator));

		Swap(_vulkan, vulkan);
		Swap(_physicalDevice, physicalDevice);
		Swap(_device, device);
		Swap(_pipelineCache, pipelineCache);
		Swap(_gpuAllocator, gpuAllocator);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Gpu::FreeResources() {
		//	Ensure all rendering operations are complete before destroying the device.
		vkDeviceWaitIdle(_device);
		DestroyGarbage();

		vmaDestroyAllocator(eastl::exchange(_gpuAllocator, nullptr));
		vkDestroyPipelineCache(_device, eastl::exchange(_pipelineCache, nullptr), _allocator);
		vkDestroyDevice(eastl::exchange(_device, nullptr), _allocator);
		_physicalDevice = VK_NULL_HANDLE;
		_vulkan         = VK_NULL_HANDLE;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
