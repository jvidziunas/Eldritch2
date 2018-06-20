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

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint Gpu::CommandQueue::CommandQueue() {
		lock.clear(std::memory_order_relaxed);
	}

	// ---------------------------------------------------

	ETInlineHint void Gpu::AddGarbage(VkBuffer buffer, VmaAllocation backing) {
		Lock _(_garbageMutex);

		_bufferGarbage.Append(buffer);
		_allocationGarbage.Append(backing);
	}

	// ---------------------------------------------------

	ETInlineHint void Gpu::AddGarbage(VkImage image, VmaAllocation backing) {
		Lock _(_garbageMutex);

		_imageGarbage.Append(image);
		_allocationGarbage.Append(backing);
	}

	// ---------------------------------------------------

	ETInlineHint void Gpu::AddGarbage(VmaAllocation garbage) {
		Lock _(_garbageMutex);

		_allocationGarbage.Append(garbage);
	}

	// ---------------------------------------------------

	ETInlineHint void Gpu::AddGarbage(VkBuffer buffer) {
		Lock _(_garbageMutex);

		_bufferGarbage.Append(buffer);
	}

	// ---------------------------------------------------

	ETInlineHint void Gpu::AddGarbage(VkImage image) {
		Lock _(_garbageMutex);

		_imageGarbage.Append(image);
	}

	// ---------------------------------------------------

	ETInlineHint const VkAllocationCallbacks* Gpu::GetAllocationCallbacks() const {
		return _allocator;
	}

	// ---------------------------------------------------

	ETInlineHint VkPipelineCache Gpu::GetPipelineCache() {
		return _pipelineCache;
	}

	// ---------------------------------------------------

	ETInlineHint Gpu::operator VkPhysicalDevice() {
		return _physicalDevice;
	}

	// ---------------------------------------------------

	ETInlineHint Gpu::operator VmaAllocator() {
		return _gpuAllocator;
	}

	// ---------------------------------------------------

	ETInlineHint Gpu::operator VkDevice() {
		return _device;
	}

	// ---------------------------------------------------

	ETInlineHint uint32_t Gpu::GetQueueFamilyByConcept(QueueConcept concept) const {
		return _queues[concept].family;
	}

	// ---------------------------------------------------

	ETInlineHint bool Gpu::SharesQueues(QueueConcept first, QueueConcept second) const {
		return _queues[first].family == _queues[second].family;
	}

	// ---------------------------------------------------

	ETInlineHint VkResult Gpu::SubmitAsync(QueueConcept target, VkFence commandsConsumed, uint32_t submitCount, const VkSubmitInfo* begin) {
		CommandQueue& queue(_queues[target]);

		while (queue.lock.test_and_set(std::memory_order_acquire)) {
			//	Busy wait.
		}

		VkResult result(vkQueueSubmit(queue.queue, submitCount, begin, commandsConsumed));

		queue.lock.clear(std::memory_order_release);

		return result;
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint VkResult Gpu::SubmitAsync(QueueConcept target, VkFence commandsConsumed, const VkSubmitInfo (&submits)[count]) {
		return Gpu::SubmitAsync(target, commandsConsumed, count, submits);
	}

	// ---------------------------------------------------

	ETInlineHint VkResult Gpu::BindAsync(QueueConcept target, VkFence commandsConsumed, uint32_t bindCount, const VkBindSparseInfo* begin) {
		CommandQueue& queue(_queues[target]);

		while (queue.lock.test_and_set(std::memory_order_acquire)) {
			//	Busy wait.
		}

		VkResult result(vkQueueBindSparse(queue.queue, bindCount, begin, commandsConsumed));

		queue.lock.clear(std::memory_order_release);

		return result;
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint VkResult Gpu::BindAsync(QueueConcept target, VkFence commandsConsumed, const VkBindSparseInfo (&binds)[count]) {
		return Gpu::BindAsync(target, commandsConsumed, count, binds);
	}

	// ---------------------------------------------------

#if VK_KHR_swapchain
	ETInlineHint VkResult Gpu::PresentAsync(QueueConcept target, const VkPresentInfoKHR& submit) {
		CommandQueue& queue(_queues[target]);

		while (queue.lock.test_and_set(std::memory_order_acquire)) {
			//	Busy wait.
		}

		VkResult result(vkQueuePresentKHR(queue.queue, &submit));

		queue.lock.clear(std::memory_order_release);

		return result;
	}
#endif // VK_KHR_swapchain

}}} // namespace Eldritch2::Graphics::Vulkan
