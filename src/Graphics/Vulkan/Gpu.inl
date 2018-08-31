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

	ETInlineHint ETForceInlineHint Gpu::CommandQueue::CommandQueue() :
		family(~uint32_t(0)),
		queue(VK_NULL_HANDLE) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 Gpu::GetFrameAfrDeviceMask() const ETNoexceptHint {
		return 0u;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Gpu::AddGarbage(VkBuffer buffer, VmaAllocation backing) {
		Lock _(_garbageMutex);

		_bufferGarbage.Append(buffer);
		_allocationGarbage.Append(backing);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Gpu::AddGarbage(VkImage image, VmaAllocation backing) {
		Lock _(_garbageMutex);

		_imageGarbage.Append(image);
		_allocationGarbage.Append(backing);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Gpu::AddGarbage(VmaAllocation garbage) {
		Lock _(_garbageMutex);

		_allocationGarbage.Append(garbage);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Gpu::AddGarbage(VkBuffer buffer) {
		Lock _(_garbageMutex);

		_bufferGarbage.Append(buffer);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Gpu::AddGarbage(VkImage image) {
		Lock _(_garbageMutex);

		_imageGarbage.Append(image);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const VkAllocationCallbacks* Gpu::GetAllocationCallbacks() const ETNoexceptHint {
		return _allocator;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkPipelineCache Gpu::GetPipelineCache() ETNoexceptHint {
		return _pipelineCache;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Gpu::operator VkPhysicalDevice() ETNoexceptHint {
		return _physicalDevice;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Gpu::operator VmaAllocator() ETNoexceptHint {
		return _gpuAllocator;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Gpu::operator VkDevice() ETNoexceptHint {
		return _device;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32_t Gpu::GetQueueFamilyByConcept(QueueConcept concept) const ETNoexceptHint {
		return _queues[concept].family;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Gpu::SharesQueues(QueueConcept first, QueueConcept second) const ETNoexceptHint {
		return _queues[first].family == _queues[second].family;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkResult Gpu::SubmitAsync(QueueConcept target, VkFence commandsConsumed, uint32_t submitCount, const VkSubmitInfo* begin) ETNoexceptHint {
		CommandQueue& queue(_queues[target]);
		Lock          _(queue.mutex);

		return vkQueueSubmit(queue.queue, submitCount, begin, commandsConsumed);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint ETForceInlineHint VkResult Gpu::SubmitAsync(QueueConcept target, VkFence commandsConsumed, const VkSubmitInfo (&submits)[count]) ETNoexceptHint {
		return Gpu::SubmitAsync(target, commandsConsumed, count, submits);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkResult Gpu::BindAsync(QueueConcept target, VkFence commandsConsumed, uint32_t bindCount, const VkBindSparseInfo* begin) ETNoexceptHint {
		CommandQueue& queue(_queues[target]);
		Lock          _(queue.mutex);

		return vkQueueBindSparse(queue.queue, bindCount, begin, commandsConsumed);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint ETForceInlineHint VkResult Gpu::BindAsync(QueueConcept target, VkFence commandsConsumed, const VkBindSparseInfo (&binds)[count]) ETNoexceptHint {
		return Gpu::BindAsync(target, commandsConsumed, count, binds);
	}

	// ---------------------------------------------------

#if VK_KHR_swapchain
	ETInlineHint ETForceInlineHint VkResult Gpu::PresentAsync(QueueConcept target, const VkPresentInfoKHR& submit) ETNoexceptHint {
		CommandQueue& queue(_queues[target]);
		Lock          _(queue.mutex);

		return vkQueuePresentKHR(queue.queue, ETAddressOf(submit));
	}
#endif // VK_KHR_swapchain

}}} // namespace Eldritch2::Graphics::Vulkan
