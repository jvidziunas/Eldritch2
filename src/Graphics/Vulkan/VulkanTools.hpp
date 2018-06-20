/*==================================================================*\
  VulkanTools.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETPureFunctionHint VkComponentMapping MakeIdentitySwizzle();

	// ---------------------------------------------------

	ETPureFunctionHint VkSubmitInfo AsSubmitInfo(uint32_t waitCount, const VkSemaphore* waits, const VkPipelineStageFlags* waitStages, uint32_t commandCount, const VkCommandBuffer* commands, uint32_t signalCount, const VkSemaphore* signals);
	template <uint32_t count, uint32_t commandCount, uint32_t signalCount>
	ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkSemaphore (&waits)[count], const VkPipelineStageFlags (&stages)[count], const VkCommandBuffer (&commands)[commandCount], const VkSemaphore (&signals)[signalCount]);
	template <uint32_t commandCount, uint32_t signalCount>
	ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkCommandBuffer (&commands)[commandCount], const VkSemaphore (&signals)[signalCount]);
	template <uint32_t count, uint32_t commandCount>
	ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkSemaphore (&waits)[count], const VkPipelineStageFlags (&stages)[count], const VkCommandBuffer (&commands)[commandCount]);
	template <uint32_t count, uint32_t signalCount>
	ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkSemaphore (&waits)[count], const VkPipelineStageFlags (&stages)[count], const VkSemaphore (&signals)[signalCount]);
	template <uint32_t count>
	ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkCommandBuffer (&commands)[count]);

	// ---------------------------------------------------

	ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
		uint32_t                                 waitCount,
		const VkSemaphore*                       waits,
		uint32_t                                 bufferBindCount,
		const VkSparseBufferMemoryBindInfo*      bufferBinds,
		uint32_t                                 opaqueImageBindCount,
		const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t                                 imageBindCount,
		const VkSparseImageMemoryBindInfo*       imageBinds,
		uint32_t                                 signalCount,
		const VkSemaphore*                       signals);
	template <uint32_t count, uint32_t signalCount>
	ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
		const VkSemaphore (&waits)[count],
		uint32_t                                 bufferBindCount,
		const VkSparseBufferMemoryBindInfo*      bufferBinds,
		uint32_t                                 opaqueImageBindCount,
		const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t                                 imageBindCount,
		const VkSparseImageMemoryBindInfo*       imageBinds,
		const VkSemaphore (&signals)[signalCount]);
	template <uint32_t signalCount>
	ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
		uint32_t                                 bufferBindCount,
		const VkSparseBufferMemoryBindInfo*      bufferBinds,
		uint32_t                                 opaqueImageBindCount,
		const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t                                 imageBindCount,
		const VkSparseImageMemoryBindInfo*       imageBinds,
		const VkSemaphore (&signals)[signalCount]);

	// ---------------------------------------------------

	template <typename InputIterator, size_t count>
	ETPureFunctionHint uint32_t FindQueueFamilyByFlags(InputIterator families, InputIterator familiesEnd, const VkQueueFlags (&flags)[count]);

	// ---------------------------------------------------

	ETPureFunctionHint VkSampleCountFlags GetSampleCountFlags(uint32_t sampleCount);

	ETPureFunctionHint VkImageType GetImageType(VkExtent3D extent);

	ETPureFunctionHint bool IsDepthStencilFormat(VkFormat format);

	// ---------------------------------------------------

	ETPureFunctionHint bool Succeeded(VkResult result);

	ETPureFunctionHint bool Failed(VkResult result);

	// ---------------------------------------------------

	ETPureFunctionHint VkViewport GetViewport(VkRect2D renderArea, bool invertDepth = true);

#if !defined(ET_FAIL_UNLESS)
#	define ET_FAIL_UNLESS(condition)          \
		{                                      \
			const auto result(condition);      \
			if (ET_UNLIKELY(Failed(result))) { \
				return result;                 \
			}                                  \
		}
#endif

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.inl>
//------------------------------------------------------------------//
