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

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	ETInlineHint Gpu::CommandQueue::CommandQueue() {
		lock.clear( std::memory_order_relaxed );
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

	ETInlineHint Gpu::operator VkDevice() {
		return _device;
	}

// ---------------------------------------------------

	ETInlineHint uint32_t Gpu::GetQueueFamilyByConcept( QueueConcept concept ) const {
		return _queues[concept].familyIndex;
	}

// ---------------------------------------------------

	ETInlineHint bool Gpu::SharesQueues( QueueConcept first, QueueConcept second ) const {
		return _queues[first].familyIndex == _queues[second].familyIndex;
	}

// ---------------------------------------------------

	ETInlineHint VkResult Gpu::SubmitAsync( QueueConcept target, VkFence commandsConsumed, uint32_t submitCount, const VkSubmitInfo* begin ) {
		CommandQueue&	queue( _queues[target] );

		while (queue.lock.test_and_set( std::memory_order_acquire )) {
		//	Busy wait.
		}

		VkResult result( vkQueueSubmit( queue.queue, submitCount, begin, commandsConsumed ) );

		queue.lock.clear( std::memory_order_release );

		return result;
	}

// ---------------------------------------------------

	template <uint32_t submitCount>
	ETInlineHint VkResult Gpu::SubmitAsync( QueueConcept target, VkFence commandsConsumed, const VkSubmitInfo (&submits)[submitCount] ) {
		return Gpu::SubmitAsync( target, commandsConsumed, submitCount, submits );
	}

// ---------------------------------------------------

	ETInlineHint VkResult Gpu::BindAsync( QueueConcept target, VkFence commandsConsumed, uint32_t bindCount, const VkBindSparseInfo* begin ) {
		CommandQueue&	queue( _queues[target] );

		while (queue.lock.test_and_set( std::memory_order_acquire )) {
		//	Busy wait.
		}

		VkResult result( vkQueueBindSparse( queue.queue, bindCount, begin, commandsConsumed ) );

		queue.lock.clear( std::memory_order_release );

		return result;
	}

// ---------------------------------------------------

	template <uint32_t bindCount>
	ETInlineHint VkResult Gpu::BindAsync( QueueConcept target, VkFence commandsConsumed, const VkBindSparseInfo (&binds)[bindCount] ) {
		return Gpu::BindAsync( target, commandsConsumed, bindCount, binds );
	}

// ---------------------------------------------------

#if VK_KHR_swapchain
	ETInlineHint VkResult Gpu::PresentAsync( QueueConcept target, const VkPresentInfoKHR& submit ) {
		CommandQueue&	queue( _queues[target] );

		while (queue.lock.test_and_set( std::memory_order_acquire )) {
		//	Busy wait.
		}

		VkResult result( vkQueuePresentKHR( queue.queue, &submit ) );

		queue.lock.clear( std::memory_order_release );

		return result;
	}
#endif	// VK_KHR_swapchain

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2