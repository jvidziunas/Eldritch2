/*==================================================================*\
  CommandList.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/DescriptorTable.hpp>
#include <Graphics/Vulkan/CommandList.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	CommandList::CommandList(CommandList&& list) :
		CommandList() {
		Swap(*this, list);
	}

	// ---------------------------------------------------

	CommandList::CommandList() :
		_pool(nullptr),
		_commandBuffer(nullptr) {}

	// ---------------------------------------------------

	CommandList::~CommandList() {
		ET_ASSERT(_pool == nullptr, "Leaking Vulkan command pool!");
	}

	// ---------------------------------------------------

	VkResult CommandList::ResetPool(Gpu& gpu, VkCommandPoolResetFlags flags) {
		return vkResetCommandPool(gpu, _pool, flags);
	}

	// ---------------------------------------------------

	void CommandList::FreeResources(Gpu& gpu) {
		if (VkCommandBuffer commandBuffer = eastl::exchange(_commandBuffer, nullptr)) {
			vkFreeCommandBuffers(gpu, _pool, 1u, &commandBuffer);
		}

		vkDestroyCommandPool(gpu, eastl::exchange(_pool, nullptr), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	VkResult CommandList::BeginRecording(VkCommandBufferUsageFlags flags) {
		const VkCommandBufferBeginInfo beginInfo {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			/*pNext =*/nullptr,
			flags,
			/*pInheritanceInfo =*/nullptr
		};

		return vkBeginCommandBuffer(_commandBuffer, &beginInfo);
	}

	// ---------------------------------------------------

	void CommandList::ExecutePipeline(const GraphicsPipeline& pipeline, const DescriptorTable& resources, const Framebuffer& target) {
		const Framebuffer::Pass* const passes(target.GetPasses());
		const VkDescriptorSet          resourceSets[] = { resources.GetDescriptorSet() };

		vkCmdBindDescriptorSets(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VK_NULL_HANDLE, /*firstSet =*/0u, _countof(resourceSets), resourceSets, /*dynamicOffsetCount =*/0u, /*pDynamicOffsets =*/nullptr);
		vkCmdResetQueryPool(_commandBuffer, target.GetTimingPool(), /*firstQuery =*/0u, /*queryCount =*/2u * pipeline.GetPassCount());
		for (uint32 pass(0u); pass < pipeline.GetPassCount(); ++pass) {
			const VkRenderPassBeginInfo passInfo {
				VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
				/*pNext =*/nullptr,
				passes[pass].nativeRenderPass,
				passes[pass].nativeFramebuffer,
				passes[pass].GetRenderArea(),
				/*clearValueCount =*/0u,
				/*pClearValues =*/nullptr
			};

			vkCmdWriteTimestamp(_commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, target.GetTimingPool(), 2u * pass);
			vkCmdBeginRenderPass(_commandBuffer, &passInfo, VK_SUBPASS_CONTENTS_INLINE);
			// Insert cool draw commands here.
			vkCmdEndRenderPass(_commandBuffer);
			vkCmdWriteTimestamp(_commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, target.GetTimingPool(), 2u * pass + 1u);
		}
	}

	// ---------------------------------------------------

	VkResult CommandList::BindResources(Gpu& gpu, QueueConcept queue, VkCommandPoolCreateFlags flags) {
		using ::Eldritch2::Swap;

		VkCommandPool                 pool(nullptr);
		const VkCommandPoolCreateInfo poolInfo {
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			flags,
			gpu.GetQueueFamilyByConcept(queue)
		};

		ET_FAIL_UNLESS(vkCreateCommandPool(gpu, &poolInfo, gpu.GetAllocationCallbacks(), &pool));
		ET_AT_SCOPE_EXIT(vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks()));

		VkCommandBuffer                   commandBuffer(nullptr);
		const VkCommandBufferAllocateInfo commandBufferInfo {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			/*pNext =*/nullptr,
			pool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			/*commandBufferCount =*/1u
		};

		ET_FAIL_UNLESS(vkAllocateCommandBuffers(gpu, &commandBufferInfo, &commandBuffer));
		ET_AT_SCOPE_EXIT(if (commandBuffer != nullptr) vkFreeCommandBuffers(gpu, pool, commandBufferInfo.commandBufferCount, &commandBuffer););

		Swap(_pool, pool);
		Swap(_commandBuffer, commandBuffer);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult CommandList::FinishRecording() {
		return vkEndCommandBuffer(_commandBuffer);
	}

	// ---------------------------------------------------

	void CommandList::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
		vkCmdDraw(_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	// ---------------------------------------------------

	void CommandList::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
		vkCmdDrawIndexed(_commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	// ---------------------------------------------------

	void CommandList::DrawIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t drawStrideInBytes) {
		vkCmdDrawIndirect(_commandBuffer, buffer, offset, drawCount, drawStrideInBytes);
	}

	// ---------------------------------------------------

	void CommandList::DrawIndexedIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t drawStrideInBytes) {
		vkCmdDrawIndexedIndirect(_commandBuffer, buffer, offset, drawCount, drawStrideInBytes);
	}

	// ---------------------------------------------------

	void CommandList::Dispatch(VkEvent halfComplete, VkEvent fullComplete, uint32_t x, uint32_t y, uint32_t z) {
		const bool shouldSplitDispatch(halfComplete != VK_NULL_HANDLE);

		if (shouldSplitDispatch) {
			//	We can increase GPU utilization % by optionally kicking off two smaller dispatches in place of the single large operation requested by the user.
			vkCmdDispatch(_commandBuffer, x / 2, y, z);
			vkCmdSetEvent(_commandBuffer, halfComplete, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
			vkCmdWaitEvents(_commandBuffer, 1u, &halfComplete, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0u, nullptr, 0u, nullptr, 0u, nullptr);
			vkCmdDispatch(_commandBuffer, x / 2, y, z);
		} else {
			vkCmdDispatch(_commandBuffer, x, y, z);
		}

		if (fullComplete != VK_NULL_HANDLE) {
			vkCmdSetEvent(_commandBuffer, fullComplete, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
		}
	}

}}} // namespace Eldritch2::Graphics::Vulkan
