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
#include <Graphics/GpuFormats.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	struct FormatDescriptor {
		VkFormat           format;
		VkComponentMapping swizzle;
	};

	ETPureFunctionHint const FormatDescriptor& DescribeFormat(GpuFormat format) ETNoexceptHint;

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint VkSubmitInfo AsSubmitInfo(uint32_t waitCount, const VkSemaphore* waits, const VkPipelineStageFlags* waitStages, uint32_t commandCount, const VkCommandBuffer* commands, uint32_t signalCount, const VkSemaphore* signals);
	template <uint32_t count, uint32_t commandCount, uint32_t signalCount>
	ETCpp14Constexpr ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkSemaphore (&waits)[count], const VkPipelineStageFlags (&stages)[count], const VkCommandBuffer (&commands)[commandCount], const VkSemaphore (&signals)[signalCount]);
	template <uint32_t commandCount, uint32_t signalCount>
	ETCpp14Constexpr ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkCommandBuffer (&commands)[commandCount], const VkSemaphore (&signals)[signalCount]);
	template <uint32_t count, uint32_t commandCount>
	ETCpp14Constexpr ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkSemaphore (&waits)[count], const VkPipelineStageFlags (&stages)[count], const VkCommandBuffer (&commands)[commandCount]);
	template <uint32_t count, uint32_t signalCount>
	ETCpp14Constexpr ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkSemaphore (&waits)[count], const VkPipelineStageFlags (&stages)[count], const VkSemaphore (&signals)[signalCount]);
	template <uint32_t count>
	ETCpp14Constexpr ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const VkCommandBuffer (&commands)[count]);

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
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
	ETCpp14Constexpr ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
		const VkSemaphore (&waits)[count],
		uint32_t                                 bufferBindCount,
		const VkSparseBufferMemoryBindInfo*      bufferBinds,
		uint32_t                                 opaqueImageBindCount,
		const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t                                 imageBindCount,
		const VkSparseImageMemoryBindInfo*       imageBinds,
		const VkSemaphore (&signals)[signalCount]);
	template <uint32_t signalCount>
	ETCpp14Constexpr ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(
		uint32_t                                 bufferBindCount,
		const VkSparseBufferMemoryBindInfo*      bufferBinds,
		uint32_t                                 opaqueImageBindCount,
		const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t                                 imageBindCount,
		const VkSparseImageMemoryBindInfo*       imageBinds,
		const VkSemaphore (&signals)[signalCount]);

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint VkSampleCountFlags GetSampleCountFlags(uint32_t sampleCount);

	ETCpp14Constexpr ETPureFunctionHint VkImageAspectFlags GetAspectsByUsage(VkImageUsageFlags usages);

	ETConstexpr ETPureFunctionHint VkImageType GetImageType(VkExtent3D extent);

	ETConstexpr ETPureFunctionHint bool IsDepthStencilFormat(VkFormat format);

	// ---------------------------------------------------

	ETConstexpr ETPureFunctionHint bool Succeeded(VkResult result);

	ETConstexpr ETPureFunctionHint bool Failed(VkResult result);

	// ---------------------------------------------------

	ETPureFunctionHint VkViewport GetViewport(VkRect2D renderArea, bool invertDepth = true);

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.inl>
//------------------------------------------------------------------//
