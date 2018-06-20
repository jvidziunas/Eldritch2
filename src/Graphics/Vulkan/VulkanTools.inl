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

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	template <uint32_t count, uint32_t commandCount, uint32_t signalCount>
	ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkSemaphore (&waits)[count], const VkPipelineStageFlags (&waitStages)[count], const VkCommandBuffer (&commands)[commandCount], const VkSemaphore (&signals)[signalCount]) {
		return AsSubmitInfo(count, waits, waitStages, commandCount, commands, signalCount, signals);
	}

	// ---------------------------------------------------

	template <uint32_t commandCount, uint32_t signalCount>
	ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkCommandBuffer (&commands)[commandCount], const VkSemaphore (&signals)[signalCount]) {
		return AsSubmitInfo(0u, nullptr, nullptr, commandCount, commands, signalCount, signals);
	}

	// ---------------------------------------------------

	template <uint32_t count, uint32_t commandCount>
	ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkSemaphore (&waits)[count], const VkPipelineStageFlags (&waitStages)[count], const VkCommandBuffer (&commands)[commandCount]) {
		return AsSubmitInfo(count, waits, waitStages, commandCount, commands, 0u, nullptr);
	}

	// ---------------------------------------------------

	template <uint32_t count, uint32_t signalCount>
	ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkSemaphore (&waits)[count], const VkPipelineStageFlags (&waitStages)[count], const VkSemaphore (&signals)[signalCount]) {
		return AsSubmitInfo(count, waits, waitStages, 0u, nullptr, signalCount, signals);
	}

	// ---------------------------------------------------

	template <uint32_t commandCount>
	ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkCommandBuffer (&commands)[commandCount]) {
		return AsSubmitInfo(0u, nullptr, nullptr, commandCount, commands, 0u, nullptr);
	}

	// ---------------------------------------------------

	template <uint32_t count, uint32_t signalCount>
	ETInlineHint ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
		const VkSemaphore (&waits)[count],
		uint32_t                                 bufferBindCount,
		const VkSparseBufferMemoryBindInfo*      bufferBinds,
		uint32_t                                 opaqueImageBindCount,
		const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t                                 imageBindCount,
		const VkSparseImageMemoryBindInfo*       imageBinds,
		const VkSemaphore (&signals)[signalCount]) {
		return AsBindSparseInfo(count, waits, bufferBindCount, bufferBinds, opaqueImageBindCount, opaqueImageBinds, imageBindCount, imageBinds, signalCount, signals);
	}

	// ---------------------------------------------------

	template <uint32_t signalCount>
	ETInlineHint ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
		uint32_t                                 bufferBindCount,
		const VkSparseBufferMemoryBindInfo*      bufferBinds,
		uint32_t                                 opaqueImageBindCount,
		const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t                                 imageBindCount,
		const VkSparseImageMemoryBindInfo*       imageBinds,
		const VkSemaphore (&signals)[signalCount]) {
		return AsBindSparseInfo(0u, nullptr, bufferBindCount, bufferBinds, opaqueImageBindCount, opaqueImageBinds, imageBindCount, imageBinds, signalCount, signals);
	}

	// ---------------------------------------------------

	template <typename InputIterator, size_t count>
	ETInlineHint ETPureFunctionHint uint32_t FindQueueFamilyByFlags(InputIterator families, InputIterator familiesEnd, const VkQueueFlags (&flags)[count]) {
		enum : uint32_t { InvalidQueue = static_cast<uint32_t>(-1) };

		register uint32_t match(InvalidQueue);

		//	Try to find a dedicated match.
		for (VkQueueFlags desired : flags) {
			for (InputIterator family(families); family != familiesEnd; ++family) {
				if (family->queueFlags == desired) {
					return static_cast<uint32_t>(family - families);
				}

				if ((family->queueFlags & desired) == desired && match == InvalidQueue) {
					//	If there are no dedicated queues, then select the first appropriate multipurpose queue.
					match = static_cast<uint32_t>(family - families);
				}
			}
		}

		return match;
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool Succeeded(VkResult result) {
		return result == VK_SUCCESS;
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool Failed(VkResult result) {
		return result < VK_SUCCESS;
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint VkImageType GetImageType(VkExtent3D extent) {
		return extent.depth > 1u ? VK_IMAGE_TYPE_3D : extent.height > 1u ? VK_IMAGE_TYPE_2D : VK_IMAGE_TYPE_1D;
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool IsDepthStencilFormat(VkFormat format) {
		return (VK_FORMAT_D16_UNORM <= format) & (format <= VK_FORMAT_D32_SFLOAT_S8_UINT);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint VkViewport GetViewport(VkRect2D renderArea, bool invertDepth) {
		return VkViewport{
			/*x =*/AsFloat(renderArea.offset.x),
			/*y =*/AsFloat(renderArea.offset.y),
			/*width =*/AsFloat(renderArea.extent.width),
			/*height =*/AsFloat(renderArea.extent.height),
			/*minDepth =*/invertDepth ? 1.0f : 0.0f,
			/*maxDepth =*/invertDepth ? 0.0f : 1.0f
		};
	}

}}} // namespace Eldritch2::Graphics::Vulkan
