/*==================================================================*\
  Gpu.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint ETForceInlineHint VkResult Gpu::CommandQueue::Submit(VkFence consumed, uint32_t submitCount, const VkSubmitInfo submits[]) ETNoexceptHint {
		ET_LOCK_SCOPE(_mutex);
		return vkQueueSubmit(_queue, submitCount, submits, consumed);
		// End of lock scope.
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkResult Gpu::CommandQueue::Bind(VkFence consumed, uint32_t bindCount, const VkBindSparseInfo binds[]) ETNoexceptHint {
		ET_LOCK_SCOPE(_mutex);
		return vkQueueBindSparse(_queue, bindCount, binds, consumed);
		// End of lock scope.
	}

	// ---------------------------------------------------

#if VK_KHR_swapchain
	ETInlineHint ETForceInlineHint VkResult Gpu::CommandQueue::Present(const VkPresentInfoKHR& present) ETNoexceptHint {
		ET_LOCK_SCOPE(_mutex);
		return vkQueuePresentKHR(_queue, ETAddressOf(present));
		// End of lock scope.
	}
#endif // VK_KHR_swapchain

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Gpu::CommandQueue::CommandQueue() ETNoexceptHint : _family(~uint32(0u)), _queue(NullVulkanHandle) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint const Gpu::CommandQueue& Gpu::GetQueue(QueueClass queue) const ETNoexceptHint {
		return _queues[_indices.byConcept[queue]];
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint Gpu::CommandQueue& Gpu::GetQueue(QueueClass queue) ETNoexceptHint {
		return _queues[_indices.byConcept[queue]];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 Gpu::GetFrameDeviceMask() const ETNoexceptHint {
		return 0u;
	}

	// ---------------------------------------------------

	template <typename HostType>
	ETInlineHint ETForceInlineHint VkResult Gpu::Map(HostType*& outBase, VmaAllocation memory) ETNoexceptHint {
		return vmaMapMemory(_gpuAllocator, memory, reinterpret_cast<void**>(ETAddressOf(outBase)));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Gpu::Unmap(VmaAllocation memory) ETNoexceptHint {
		return vmaUnmapMemory(_gpuAllocator, memory);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkResult Gpu::AllocateMemory(VmaAllocation& allocation, const VmaAllocationCreateInfo& createInfo, VkBuffer buffer) {
		return vmaAllocateMemoryForBuffer(_gpuAllocator, buffer, ETAddressOf(createInfo), ETAddressOf(allocation), nullptr);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkResult Gpu::AllocateMemory(VmaAllocation& allocation, const VmaAllocationCreateInfo& createInfo, VkImage image) {
		return vmaAllocateMemoryForImage(_gpuAllocator, image, ETAddressOf(createInfo), ETAddressOf(allocation), nullptr);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Gpu::DeallocateMemory(VmaAllocation allocation) {
		vmaFreeMemory(_gpuAllocator, allocation);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkResult Gpu::GetTemporarySemaphore(VkSemaphore& outSemaphore) {
		VkSemaphore                 semaphore;
		const VkSemaphoreCreateInfo semaphoreInfo {
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u
		};

		ET_ABORT_UNLESS(vkCreateSemaphore(_device, ETAddressOf(semaphoreInfo), GetAllocationCallbacks(), ETAddressOf(semaphore)));
		Gpu::Finalize([=](Gpu& self) ETNoexceptHint{
			vkDestroySemaphore(self, semaphore, self.GetAllocationCallbacks());
		});

		outSemaphore = semaphore;
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	template <typename BinaryPredicate>
	ETInlineHint ETForceInlineHint void Gpu::Finalize(PoolClass pool, BinaryPredicate finalizer) ETNoexceptHint {
		struct Shim final : public Finalizer<Gpu&, VmaAllocator> {
			ETConstexpr ETForceInlineHint Shim(BinaryPredicate&& finalizer) : finalizer(Move(finalizer)) {}

			ETInlineHint void operator()(Gpu& self, VmaAllocator allocator) ETNoexceptHint override {
				finalizer(self, allocator);
				eastl::destruct(this);
			}

			BinaryPredicate finalizer;
		};

		_allocationFinalizers.Push(new(_finalizerArena.Allocate(sizeof(Shim))) Shim(Move(finalizer)));
	}

	// ---------------------------------------------------

	template <typename UnaryPredicate>
	ETInlineHint ETForceInlineHint void Gpu::Finalize(UnaryPredicate finalizer) ETNoexceptHint {
		struct Shim final : public Finalizer<Gpu&> {
			ETConstexpr ETForceInlineHint Shim(UnaryPredicate&& finalizer) : finalizer(Move(finalizer)) {}

			ETInlineHint void operator()(Gpu& self) ETNoexceptHint override {
				finalizer(self);
				eastl::destruct(this);
			}

			UnaryPredicate finalizer;
		};

		_finalizers.Push(*new(_finalizerArena.Allocate(sizeof(Shim))) Shim(Move(finalizer)));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint const VkAllocationCallbacks* Gpu::GetAllocationCallbacks() const ETNoexceptHint {
		return _allocator;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint Gpu::operator VkPhysicalDevice() ETNoexceptHint {
		return _physicalDevice;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint Gpu::operator VkInstance() ETNoexceptHint {
		return _vulkan;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint Gpu::operator VkDevice() ETNoexceptHint {
		return _device;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
