/*==================================================================*\
  Gpu.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
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
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		template <typename InputIterator, size_t count>
		ETInlineHint ETPureFunctionHint VkResult FindQueueFamily(uint32& outFamily, InputIterator begin, InputIterator end, const VkQueueFlags (&flags)[count]) ETNoexceptHint {
			for (VkQueueFlags desiredFlags : flags) {
				//	Try to find a dedicated match.
				const auto candidate(FindIf(begin, end, [desiredFlags](const VkQueueFamilyProperties& family) ETNoexceptHint {
					return family.queueFlags == desiredFlags;
				}));
				if (candidate != end) {
					outFamily = uint32(eastl::distance(begin, candidate));
					return VK_SUCCESS;
				}
			}

			for (VkQueueFlags desiredFlags : flags) {
				const auto candidate(FindIf(begin, end, [desiredFlags](const VkQueueFamilyProperties& family) ETNoexceptHint {
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

		ETInlineHint ETForceInlineHint VkResult FindQueueFamilies(uint32 (&families)[QueueClasses], VkPhysicalDevice device) ETNoexceptHint {
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

		ETInlineHint VkResult BuildQueueCreateInfos(VkDeviceQueueCreateInfo (&out)[QueueClasses], uint32& outQueueCount, VkPhysicalDevice device) ETNoexceptHint {
			uint32 indices[QueueClasses];

			ET_ABORT_UNLESS(FindQueueFamilies(indices, device));
			Sort(indices, End(indices));
			const auto lastUnique(Unique(indices, End(indices)));

			outQueueCount = uint32(eastl::distance(indices, lastUnique));
			Transform(indices, lastUnique, out, [](uint32 family) ETNoexceptHint {
				static ETConstexpr float32 priorities[] = { 1.0f };
				return VkDeviceQueueCreateInfo{
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
		vkGetDeviceQueue(device, familyIndex, /*queueIndex =*/0u, ETAddressOf(_queue));
		_family = familyIndex;
	}

	// ---------------------------------------------------

	Gpu::Gpu() ETNoexceptHint : _allocator("GPU Root Allocator"),
								_physicalDevice(NullVulkanHandle),
								_device(NullVulkanHandle),
								_gpuAllocator(NullVulkanHandle),
								_finalizerArena("GPU Finalizer Arena Allocator") {}

	// ---------------------------------------------------

	Gpu::~Gpu() {
		ETAssert(_device == NullVulkanHandle, "Leaking Vulkan device {}!", fmt::ptr(_device));
		ETAssert(_gpuAllocator == NullVulkanHandle, "Leaking Vulkan GPU allocator {}!", fmt::ptr(_gpuAllocator));
	}

	// ---------------------------------------------------

	void Gpu::DestroyGarbage() ETNoexceptHint {
		{
			ET_LOCK_SCOPE(_gpuAllocatorMutex);
			_allocationFinalizers.ClearAndDispose([this](Finalizer<Gpu&, VmaAllocator>& finalizer) ETNoexceptHint {
				finalizer(*this, _gpuAllocator);
			});
			// End of lock scope.
		}

		_finalizers.ClearAndDispose([this](Finalizer<Gpu&>& finalizer) ETNoexceptHint {
			finalizer(*this);
		});

		_finalizerArena.Restore(_frameStartCheckpoint);
	}

	// ---------------------------------------------------

	VkResult Gpu::BindResources(VkInstance vulkan, VkPhysicalDevice physicalDevice, VkDeviceSize heapBlockSize, uint32 frameUseCount) {
		using ::Eldritch2::Swap;

		const InitializerList<const char*> extensions {
#if VK_KHR_swapchain
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#endif
		};

		uint32                  queueInfoCount(0u);
		VkDeviceQueueCreateInfo queueInfos[QueueClasses];
		ET_ABORT_UNLESS(BuildQueueCreateInfos(queueInfos, queueInfoCount, physicalDevice));

		VkDevice                 device;
		const VkDeviceCreateInfo deviceInfo{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			/*queueCreateInfoCount =*/queueInfoCount, Begin(queueInfos),
			/*enabledLayerCount =*/0u, /*ppEnabledLayerNames =*/nullptr,
			/*enabledExtensionCount =*/uint32(extensions.size()), Begin(extensions),
			/*pEnabledFeatures =*/nullptr
		};
		ET_ABORT_UNLESS(vkCreateDevice(physicalDevice, ETAddressOf(deviceInfo), _allocator, ETAddressOf(device)));
		ET_AT_SCOPE_EXIT(vkDestroyDevice(device, _allocator));

		uint32 families[QueueClasses];
		ET_ABORT_UNLESS(FindQueueFamilies(families, physicalDevice));
		for (uint32 concept(0u); concept < ETCountOf(_queues); ++concept) {
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
		ET_ABORT_UNLESS(vmaCreateAllocator(ETAddressOf(gpuAllocatorInfo), ETAddressOf(gpuAllocator)));
		ET_AT_SCOPE_EXIT(vmaDestroyAllocator(gpuAllocator));

		if (Failed(_finalizerArena.BindResources(_allocator, /*byteSize =*/512u * 1024u, AllocationDuration::Permanent))) {
			return VK_ERROR_OUT_OF_HOST_MEMORY;
		}

		_frameStartCheckpoint = _finalizerArena.GetHead();

		Swap(_vulkan, vulkan);
		Swap(_physicalDevice, physicalDevice);
		Swap(_device, device);
		Swap(_gpuAllocator, gpuAllocator);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Gpu::FreeResources() ETNoexceptHint {
		//	Ensure all rendering operations are complete before destroying the device.
		vkDeviceWaitIdle(_device);
		DestroyGarbage();

		vmaDestroyAllocator(Exchange(_gpuAllocator, NullVulkanHandle));
		vkDestroyDevice(Exchange(_device, NullVulkanHandle), _allocator);
		_physicalDevice = NullVulkanHandle;
		_vulkan         = NullVulkanHandle;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
