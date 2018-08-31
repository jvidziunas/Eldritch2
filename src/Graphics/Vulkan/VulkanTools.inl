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

	ETCpp14Constexpr ETPureFunctionHint VkComponentMapping MakeIdentitySwizzle() {
		return VkComponentMapping { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	}

	// ---------------------------------------------------

	template <uint32_t count, uint32_t commandCount, uint32_t signalCount>
	ETCpp14Constexpr ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkSemaphore (&waits)[count], const VkPipelineStageFlags (&waitStages)[count], const VkCommandBuffer (&commands)[commandCount], const VkSemaphore (&signals)[signalCount]) {
		return AsSubmitInfo(count, waits, waitStages, commandCount, commands, signalCount, signals);
	}

	// ---------------------------------------------------

	template <uint32_t commandCount, uint32_t signalCount>
	ETCpp14Constexpr ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkCommandBuffer (&commands)[commandCount], const VkSemaphore (&signals)[signalCount]) {
		return AsSubmitInfo(0u, nullptr, nullptr, commandCount, commands, signalCount, signals);
	}

	// ---------------------------------------------------

	template <uint32_t count, uint32_t commandCount>
	ETCpp14Constexpr ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkSemaphore (&waits)[count], const VkPipelineStageFlags (&waitStages)[count], const VkCommandBuffer (&commands)[commandCount]) {
		return AsSubmitInfo(count, waits, waitStages, commandCount, commands, 0u, nullptr);
	}

	// ---------------------------------------------------

	template <uint32_t count, uint32_t signalCount>
	ETCpp14Constexpr ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkSemaphore (&waits)[count], const VkPipelineStageFlags (&waitStages)[count], const VkSemaphore (&signals)[signalCount]) {
		return AsSubmitInfo(count, waits, waitStages, 0u, nullptr, signalCount, signals);
	}

	// ---------------------------------------------------

	template <uint32_t commandCount>
	ETCpp14Constexpr ETInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkCommandBuffer (&commands)[commandCount]) {
		return AsSubmitInfo(0u, nullptr, nullptr, commandCount, commands, 0u, nullptr);
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint VkSubmitInfo AsSubmitInfo(
		uint32_t waitCount, const VkSemaphore* waits,
		const VkPipelineStageFlags* stages,
		uint32_t                    commandCount,
		const VkCommandBuffer*      commands,
		uint32_t                    signalCount,
		const VkSemaphore*          signals) {
		return VkSubmitInfo {
			VK_STRUCTURE_TYPE_SUBMIT_INFO,
			nullptr, // No extensions.
			waitCount,
			waits,
			stages,
			commandCount,
			commands,
			signalCount,
			signals
		};
	}

	// ---------------------------------------------------

	template <uint32_t count, uint32_t signalCount>
	ETCpp14Constexpr ETInlineHint ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
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
	ETCpp14Constexpr ETInlineHint ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
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

	ETCpp14Constexpr ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
		uint32_t waitCount, const VkSemaphore* waits,
		uint32_t bufferBindCount, const VkSparseBufferMemoryBindInfo* bufferBinds,
		uint32_t opaqueImageBindCount, const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t imageBindCount, const VkSparseImageMemoryBindInfo* imageBinds,
		uint32_t signalCount, const VkSemaphore* signals) {
		return VkBindSparseInfo {
			VK_STRUCTURE_TYPE_BIND_SPARSE_INFO,
			nullptr, // No extension data.
			waitCount,
			waits,
			bufferBindCount,
			bufferBinds,
			opaqueImageBindCount,
			opaqueImageBinds,
			imageBindCount,
			imageBinds,
			signalCount,
			signals
		};
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint bool Succeeded(VkResult result) {
		return result == VK_SUCCESS;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETPureFunctionHint bool Failed(VkResult result) {
		return result < VK_SUCCESS;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint VkImageType GetImageType(VkExtent3D extent) {
		return extent.depth > 1u ? VK_IMAGE_TYPE_3D : extent.height > 1u ? VK_IMAGE_TYPE_2D : VK_IMAGE_TYPE_1D;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint bool IsDepthStencilFormat(VkFormat format) {
		return (VK_FORMAT_D16_UNORM <= format) & (format <= VK_FORMAT_D32_SFLOAT_S8_UINT);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint VkViewport GetViewport(VkRect2D renderArea, bool invertDepth) {
		return VkViewport {
			/*x =*/AsFloat(renderArea.offset.x),
			/*y =*/AsFloat(renderArea.offset.y),
			/*width =*/AsFloat(renderArea.extent.width),
			/*height =*/AsFloat(renderArea.extent.height),
			/*minDepth =*/invertDepth ? 1.0f : 0.0f,
			/*maxDepth =*/invertDepth ? 0.0f : 1.0f
		};
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint VkSampleCountFlags GetSampleCountFlags(uint32_t sampleCount) {
		sampleCount = Clamp<uint32_t>(sampleCount, VK_SAMPLE_COUNT_1_BIT, VK_SAMPLE_COUNT_64_BIT);

		return VkSampleCountFlags(sampleCount | (sampleCount - 1u));
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint VkImageAspectFlags GetAspectsByUsage(VkImageUsageFlags usages) {
		const bool hasColorAspect(usages & VK_IMAGE_USAGE_SAMPLED_BIT | usages & VK_IMAGE_USAGE_STORAGE_BIT | usages & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
		const bool hasDepthStencilAspect(usages & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

		return (hasColorAspect ? VK_IMAGE_ASPECT_COLOR_BIT : 0u) | (hasDepthStencilAspect ? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT : 0u);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
