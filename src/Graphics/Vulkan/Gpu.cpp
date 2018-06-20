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
#include <eastl/sort.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("vulkan-1.lib")
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	namespace {

		ETInlineHint void FindQueueFamilies(VkPhysicalDevice device, uint32_t (&families)[QueueConcept::COUNT]) {
			uint32_t familyCount(0u);

			//	Determine the number of queue families exposed by the device.
			vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
			const auto properties(static_cast<VkQueueFamilyProperties*>(_alloca(familyCount * sizeof(VkQueueFamilyProperties))));
			const auto end(properties + familyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, properties);

			families[QueueConcept::Drawing]         = FindQueueFamilyByFlags(properties, end, { VkQueueFlags(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT) });
			families[QueueConcept::Presentation]    = FindQueueFamilyByFlags(properties, end, { VkQueueFlags(VK_QUEUE_COMPUTE_BIT) });
			families[QueueConcept::SparseBinding]   = FindQueueFamilyByFlags(properties, end, { VkQueueFlags(VK_QUEUE_SPARSE_BINDING_BIT | VK_QUEUE_TRANSFER_BIT), VkQueueFlags(VK_QUEUE_SPARSE_BINDING_BIT) });
			families[QueueConcept::Transfer]        = FindQueueFamilyByFlags(properties, end, { VkQueueFlags(VK_QUEUE_SPARSE_BINDING_BIT | VK_QUEUE_TRANSFER_BIT), VkQueueFlags(VK_QUEUE_TRANSFER_BIT) });
			families[QueueConcept::BulkComputation] = FindQueueFamilyByFlags(properties, end, { VkQueueFlags(VK_QUEUE_COMPUTE_BIT) });
		}

		// ---------------------------------------------------

		ETInlineHint uint32_t BuildQueueCreateInfos(VkPhysicalDevice device, VkDeviceQueueCreateInfo (&out)[QueueConcept::COUNT]) {
			static const float priorities[] = { 1.0f };
			uint32_t           indices[QueueConcept::COUNT];

			FindQueueFamilies(device, indices);

			Sort(eastl::begin(indices), eastl::end(indices), LessThan<uint32_t>());
			const auto lastUnique(eastl::unique(eastl::begin(indices), eastl::end(indices)));

			return static_cast<uint32_t>(eastl::distance(eastl::begin(out), Transform(eastl::begin(indices), lastUnique, out, [](uint32_t familyIndex) {
															 return VkDeviceQueueCreateInfo{
																 VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
																 nullptr, // No extension data.
																 0u,      // No create flags.
																 familyIndex,
																 _countof(priorities),
																 priorities
															 };
														 })));
		}

	} // anonymous namespace

	void Gpu::CommandQueue::BindResources(VkDevice device, uint32_t family) {
		vkGetDeviceQueue(device, family, 0u, &queue);
		this->family = family;
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
		ET_ASSERT(_device == nullptr, "Leaking Vulkan device!");
		ET_ASSERT(_pipelineCache == nullptr, "Leaking Vulkan pipeline cache!");
		ET_ASSERT(_gpuAllocator == nullptr, "Leaking Vulkan GPU allocator!");
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

		{
			Lock _(_gpuAllocatorMutex);
			for (VmaAllocation garbage : allocationGarbage) {
				vmaFreeMemory(_gpuAllocator, garbage);
			}
		} // End of lock scope.
	}

	// ---------------------------------------------------

	VkResult Gpu::BindResources(VkPhysicalDevice physicalDevice, VkDeviceSize heapBlockSize, uint32 frameUseCount) {
		using ::Eldritch2::Swap;

		static const char* const enabledExtensions[] = {
#if VK_KHR_swapchain
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#endif
			nullptr
		};

		VkDeviceQueueCreateInfo  queueInfos[QueueConcept::COUNT];
		VkDevice                 device;
		const VkDeviceCreateInfo deviceInfo{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			/*queueCreateInfoCount =*/BuildQueueCreateInfos(physicalDevice, queueInfos), queueInfos,
			/*enabledLayerCount =*/0u,
			/*ppEnabledLayerNames =*/nullptr,
			/*enabledExtensionCount =*/uint32_t(_countof(enabledExtensions) - 1u), enabledExtensions,
			/*pEnabledFeatures =*/nullptr
		};
		ET_FAIL_UNLESS(vkCreateDevice(physicalDevice, &deviceInfo, _allocator, &device));
		ET_AT_SCOPE_EXIT(vkDestroyDevice(device, _allocator));

		VkPipelineCache                 pipelineCache;
		const VkPipelineCacheCreateInfo pipelineCacheInfo{
			VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			/*initialDataSize =*/0u,
			/*pInitialData =*/nullptr,
		};
		ET_FAIL_UNLESS(vkCreatePipelineCache(device, &pipelineCacheInfo, _allocator, &pipelineCache));
		ET_AT_SCOPE_EXIT(if (device) vkDestroyPipelineCache(device, pipelineCache, _allocator););

		uint32_t families[QueueConcept::COUNT];
		FindQueueFamilies(physicalDevice, families);

		for (uint32_t concept(0u); concept < _countof(_queues); ++concept) {
			_queues[concept].BindResources(device, families[concept]);
		}
		VmaAllocator                 gpuAllocator;
		const VmaAllocatorCreateInfo gpuAllocatorInfo{
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
		ET_FAIL_UNLESS(vmaCreateAllocator(&gpuAllocatorInfo, &gpuAllocator));
		ET_AT_SCOPE_EXIT(vmaDestroyAllocator(gpuAllocator));

		//	Commit resources to the object.
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
		_physicalDevice = nullptr;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
