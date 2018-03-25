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

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	ETPureFunctionHint VkComponentMapping	MakeIdentitySwizzle();

// ---------------------------------------------------

	ETPureFunctionHint VkSubmitInfo AsSubmitInfo( uint32_t waitCount, const VkSemaphore* waits, const VkPipelineStageFlags* waitStages, uint32_t commandCount, const VkCommandBuffer* commands, uint32_t signalCount, const VkSemaphore* signals );
	template <uint32_t waitCount, uint32_t commandCount, uint32_t signalCount>
	ETPureFunctionHint VkSubmitInfo AsSubmitInfo( const VkSemaphore (&waits)[waitCount], const VkPipelineStageFlags (&waitStages)[waitCount], const VkCommandBuffer (&commands)[commandCount], const VkSemaphore (&signals)[signalCount] );
	template <uint32_t commandCount, uint32_t signalCount>
	ETPureFunctionHint VkSubmitInfo AsSubmitInfo( const VkCommandBuffer (&commands)[commandCount], const VkSemaphore (&signals)[signalCount] );
	template <uint32_t waitCount, uint32_t commandCount>
	ETPureFunctionHint VkSubmitInfo AsSubmitInfo( const VkSemaphore (&waits)[waitCount], const VkPipelineStageFlags (&waitStages)[waitCount], const VkCommandBuffer (&commands)[commandCount] );
	template <uint32_t waitCount, uint32_t signalCount>
	ETPureFunctionHint VkSubmitInfo AsSubmitInfo( const VkSemaphore (&waits)[waitCount], const VkPipelineStageFlags (&waitStages)[waitCount], const VkSemaphore (&signals)[signalCount] );
	template <uint32_t commandCount>
	ETPureFunctionHint VkSubmitInfo AsSubmitInfo( const VkCommandBuffer (&commands)[commandCount] );

// ---------------------------------------------------

	ETPureFunctionHint VkBindSparseInfo	AsBindSparseInfo(
		uint32_t waitCount,
		const VkSemaphore* waits,
		uint32_t bufferBindCount,
		const VkSparseBufferMemoryBindInfo* bufferBinds,
		uint32_t opaqueImageBindCount,
		const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t imageBindCount,
		const VkSparseImageMemoryBindInfo* imageBinds,
		uint32_t signalCount,
		const VkSemaphore* signals
	);
	template <uint32_t waitCount, uint32_t signalCount>
	ETPureFunctionHint VkBindSparseInfo	AsBindSparseInfo(
		const VkSemaphore (&waits)[waitCount],
		uint32_t bufferBindCount,
		const VkSparseBufferMemoryBindInfo* bufferBinds,
		uint32_t opaqueImageBindCount,
		const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t imageBindCount,
		const VkSparseImageMemoryBindInfo* imageBinds,
		const VkSemaphore (&signals)[signalCount]
	);
	template <uint32_t signalCount>
	ETPureFunctionHint VkBindSparseInfo	AsBindSparseInfo(
		uint32_t bufferBindCount,
		const VkSparseBufferMemoryBindInfo* bufferBinds,
		uint32_t opaqueImageBindCount,
		const VkSparseImageOpaqueMemoryBindInfo* opaqueImageBinds,
		uint32_t imageBindCount,
		const VkSparseImageMemoryBindInfo* imageBinds,
		const VkSemaphore (&signals)[signalCount]
	);

// ---------------------------------------------------

	template <typename InputIterator, size_t preferredFlagsCount>
	uint32_t	FindQueueFamilyByFlags( InputIterator families, InputIterator familiesEnd, const VkQueueFlagBits (&flags)[preferredFlagsCount] );

// ---------------------------------------------------

	ETPureFunctionHint bool	Succeeded( VkResult result );

	ETPureFunctionHint bool	Failed( VkResult result );

#if !defined( ET_FAIL_UNLESS )
#	define ET_FAIL_UNLESS(condition) { const auto tempVal( condition ); if (ET_UNLIKELY( Failed( tempVal ) )) { return tempVal; } }
#endif

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.inl>
//------------------------------------------------------------------//