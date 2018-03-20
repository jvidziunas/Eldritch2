/*==================================================================*\
  VulkanTools.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "vulkan-1.lib" )
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	ETPureFunctionHint VkComponentMapping MakeIdentitySwizzle() {
		return VkComponentMapping{
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY
		};
	}

// ---------------------------------------------------

	ETPureFunctionHint VkSubmitInfo AsSubmitInfo(
		uint32_t waitCount, const VkSemaphore* waits,
		const VkPipelineStageFlags* waitStages,
		uint32_t commandCount,
		const VkCommandBuffer* commands,
		uint32_t signalCount,
		const VkSemaphore* signals ) {
		return VkSubmitInfo{
			VK_STRUCTURE_TYPE_SUBMIT_INFO,
			nullptr,	// No extensions.
			waitCount, waits, waitStages,
			commandCount, commands,
			signalCount, signals
		};
	}

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
	) {
		return VkBindSparseInfo{
			VK_STRUCTURE_TYPE_BIND_SPARSE_INFO,
			nullptr,	// No extension data.
			waitCount,				waits,
			bufferBindCount,		bufferBinds,
			opaqueImageBindCount,	opaqueImageBinds,
			imageBindCount,			imageBinds,
			signalCount,			signals
		};
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2