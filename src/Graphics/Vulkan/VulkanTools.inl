/*==================================================================*\
  VulkanTools.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	template <uint32_t waitCount, uint32_t commandCount, uint32_t signalCount>
	ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo( const VkSemaphore (&waits)[waitCount], const VkPipelineStageFlags (&waitStages)[waitCount], const VkCommandBuffer (&commands)[commandCount], const VkSemaphore (&signals)[signalCount] ) {
		return AsSubmitInfo( waitCount, waits, waitStages, commandCount, commands, signalCount, signals );
	}

// ---------------------------------------------------

	template <uint32_t commandCount, uint32_t signalCount>
	ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo( const VkCommandBuffer (&commands)[commandCount], const VkSemaphore (&signals)[signalCount] ) {
		return AsSubmitInfo( 0u, nullptr, nullptr, commandCount, commands, signalCount, signals );
	}

// ---------------------------------------------------

	template <uint32_t waitCount, uint32_t commandCount>
	ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo( const VkSemaphore (&waits)[waitCount], const VkPipelineStageFlags (&waitStages)[waitCount], const VkCommandBuffer (&commands)[commandCount] ) {
		return AsSubmitInfo( waitCount, waits, waitStages, commandCount, commands, 0u, nullptr );
	}

// ---------------------------------------------------

	template <uint32_t waitCount, uint32_t signalCount>
	ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo( const VkSemaphore (&waits)[waitCount], const VkPipelineStageFlags (&waitStages)[waitCount], const VkSemaphore (&signals)[signalCount] ) {
		return AsSubmitInfo( waitCount, waits, waitStages, 0u, nullptr, signalCount, signals );
	}

// ---------------------------------------------------

	template <uint32_t commandCount>
	ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo( const VkCommandBuffer (&commands)[commandCount] ) {
		return AsSubmitInfo( 0u, nullptr, nullptr, commandCount, commands, 0u, nullptr );
	}

// ---------------------------------------------------

	template <uint32_t waitCount, uint32_t signalCount>
	ETInlineHint ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
		const VkSemaphore (&waits)[waitCount],
		uint32_t bufferBindCount,
		const VkSparseBufferMemoryBindInfo* bufferBinds,
		uint32_t opaqueImageBindCount,
		const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t imageBindCount,
		const VkSparseImageMemoryBindInfo* imageBinds,
		const VkSemaphore (&signals)[signalCount]
	) {
		return AsBindSparseInfo( waitCount, waits, bufferBindCount, bufferBinds, opaqueImageBindCount, opaqueImageBinds, imageBindCount, imageBinds, signalCount, signals );
	}

// ---------------------------------------------------

	template <uint32_t signalCount>
	ETInlineHint ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
		uint32_t bufferBindCount,
		const VkSparseBufferMemoryBindInfo* bufferBinds,
		uint32_t opaqueImageBindCount,
		const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t imageBindCount,
		const VkSparseImageMemoryBindInfo* imageBinds,
		const VkSemaphore (&signals)[signalCount]
	) {
		return AsBindSparseInfo( 0u, nullptr, bufferBindCount, bufferBinds, opaqueImageBindCount, opaqueImageBinds, imageBindCount, imageBinds, signalCount, signals );
	}

// ---------------------------------------------------

	template <typename InputIterator, size_t preferredFlagsCount>
	ETInlineHint uint32_t FindQueueFamilyByFlags( InputIterator families, InputIterator familiesEnd, const VkQueueFlagBits (&flags)[preferredFlagsCount] ) {
	//	Try to find a dedicated match.
		for (VkQueueFlags desiredFlags : flags) {
			for (InputIterator family( families ); family != familiesEnd; ++family) {
				if (family->queueFlags == desiredFlags) {
					return static_cast<uint32_t>(family - families);
				}
			}
		}

	//	If there are no dedicated queues, then select the first appropriate multipurpose queue.
		for (VkQueueFlags desiredFlags : flags) {
			for (InputIterator family( families ); family != familiesEnd; ++family) {
				if (( family->queueFlags & desiredFlags ) == desiredFlags) {
					return static_cast<uint32_t>(family - families);
				}
			}
		}

		return static_cast<uint32_t>(-1);
	}

	
// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool Succeeded( VkResult result ) {
		return result == VK_SUCCESS;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool Failed( VkResult result ) {
		return result < VK_SUCCESS;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint VkImageType GetImageType( VkExtent3D extent ) {
		return extent.depth > 1u ? VK_IMAGE_TYPE_3D : extent.height > 1u ? VK_IMAGE_TYPE_2D : VK_IMAGE_TYPE_1D;
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2