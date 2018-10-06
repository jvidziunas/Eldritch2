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
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/Vulkan/CommandList.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		ETInlineHint ETForceInlineHint void Dispatch(VkCommandBuffer commands, VkEvent halfComplete, VkEvent complete, uint32 x, uint32 y, uint32 z) ETNoexceptHint {
			//	We can increase GPU utilization % by optionally kicking off two smaller dispatches in place of the single large operation requested by the user.
			if (halfComplete == VK_NULL_HANDLE) {
				vkCmdDispatch(commands, x, y, z);
			} else {
				vkCmdDispatch(commands, x / 2u, y, z);
				vkCmdSetEvent(commands, halfComplete, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
				vkCmdWaitEvents(commands, /*eventCount =*/1u, ETAddressOf(halfComplete), VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0u, nullptr, 0u, nullptr, 0u, nullptr);
				vkCmdDispatch(commands, x / 2u, y, z);
			}

			if (complete != VK_NULL_HANDLE) {
				vkCmdSetEvent(commands, complete, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
			}
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint void ExecuteGraphicsPass(VkCommandBuffer commands, uint32 batchCount, const VkCommandBuffer* batches, const View& view, const VkRenderPassBeginInfo& passInfo) ETNoexceptHint {
			vkCmdBeginRenderPass(commands, ETAddressOf(passInfo), VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
			vkCmdSetViewport(commands, /*firstViewport =*/0u, /*viewportCount =*/1u, ETAddressOf(view.affineViewport));
			vkCmdExecuteCommands(commands, batchCount, batches);
			vkCmdEndRenderPass(commands);
		}

		// ---------------------------------------------------

		void ExecuteGraphicsPass(VkCommandBuffer commands, uint32 batchCount, const VkCommandBuffer* batches, const ArrayList<View>& views, const VkRenderPassBeginInfo& passInfo) ETNoexceptHint {
			static ETConstexpr uint32 AllBits(~0u);
			uint32                    stencilReference(0u);

			vkCmdSetStencilCompareMask(commands, VK_STENCIL_FRONT_AND_BACK, AllBits);
			vkCmdSetStencilWriteMask(commands, VK_STENCIL_FRONT_AND_BACK, AllBits);

			vkCmdBeginRenderPass(commands, ETAddressOf(passInfo), VK_SUBPASS_CONTENTS_INLINE);
			for (const View& view : views) {
				vkCmdSetViewport(commands, /*firstViewport =*/0u, /*viewportCount =*/1u, ETAddressOf(view.affineViewport));
				// >> Draw stencil masks <<
			}

			vkCmdNextSubpass(commands, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
			for (const View& view : views) {
				vkCmdSetStencilReference(commands, VK_STENCIL_FRONT_AND_BACK, stencilReference++);
				vkCmdSetViewport(commands, /*firstViewport =*/0u, /*viewportCount =*/1u, ETAddressOf(view.affineViewport));
				vkCmdExecuteCommands(commands, batchCount, batches);
			}
			vkCmdEndRenderPass(commands);
		}

	} // anonymous namespace

	CommandList::CommandList() ETNoexceptHint : _pool(nullptr) {
		Fill(_commands, End(_commands), nullptr);
	}

	// ---------------------------------------------------

	CommandList::CommandList(CommandList&& list) ETNoexceptHint : CommandList() {
		Swap(*this, list);
	}

	// ---------------------------------------------------

	CommandList::~CommandList() {
		ET_ASSERT(_pool == nullptr, "Leaking Vulkan command pool {}!", fmt::ptr(_pool));
	}

	// ---------------------------------------------------

	void CommandList::ExecutePipeline(const Framebuffer& framebuffer, const GraphicsPipeline& pipeline, const ArrayList<View>& views) {
		const auto& descriptorSets(pipeline.GetShaderResources().GetDescriptorSets());
		const auto& batches(pipeline.GetBatches());
		const auto  self(_commands[0]);
		uint32      timer(0u);

		vkCmdBindDescriptorSets(self, VK_PIPELINE_BIND_POINT_GRAPHICS, VK_NULL_HANDLE, /*firstSet =*/0u, ETCountOf(descriptorSets), descriptorSets, /*dynamicOffsetCount =*/0u, /*pDynamicOffsets =*/nullptr);
		vkCmdBindDescriptorSets(self, VK_PIPELINE_BIND_POINT_COMPUTE, VK_NULL_HANDLE, /*firstSet =*/0u, ETCountOf(descriptorSets), descriptorSets, /*dynamicOffsetCount =*/0u, /*pDynamicOffsets =*/nullptr);
		vkCmdResetQueryPool(self, framebuffer.GetTimingPool(), /*firstQuery =*/0u, /*queryCount =*/uint32(2u * framebuffer.GetPasses().GetSize()));
		for (const Framebuffer::Pass& pass : framebuffer.GetPasses()) {
			vkCmdWriteTimestamp(self, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, framebuffer.GetTimingPool(), timer++);
			ExecuteGraphicsPass(self, 0u, nullptr, views, VkRenderPassBeginInfo { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
																				  /*pNext =*/nullptr, pass.renderPass, pass.framebuffer, VkRect2D { { 0, 0 }, pass.GetRenderExtent() },
																				  /*clearValueCount =*/0u,
																				  /*pClearValues =*/nullptr });
			vkCmdWriteTimestamp(self, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, framebuffer.GetTimingPool(), timer++);
		}
	}

	// ---------------------------------------------------

	void CommandList::CompositeImage(const Framebuffer& /*target*/, const Framebuffer& /*source*/) {
		//	vkCmdBlitImage(_commands, )
	}

	// ---------------------------------------------------

	VkResult CommandList::BeginRecording(Gpu& gpu, VkCommandPoolResetFlags resetFlags, VkCommandBufferUsageFlags usages) ETNoexceptHint {
		const VkCommandBufferBeginInfo beginInfo {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			/*pNext =*/nullptr,
			usages,
			/*pInheritanceInfo =*/nullptr
		};

		ET_ABORT_UNLESS(vkResetCommandPool(gpu, _pool, resetFlags));
		ET_ABORT_UNLESS(vkBeginCommandBuffer(_commands[0], ETAddressOf(beginInfo)));

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult CommandList::FinishRecording() ETNoexceptHint {
		return vkEndCommandBuffer(_commands[0]);
	}

	// ---------------------------------------------------

	VkResult CommandList::BindResources(Gpu& gpu, QueueConcept queue, VkCommandPoolCreateFlags poolFlags) {
		using ::Eldritch2::Swap;

		VkCommandPool                 pool;
		const VkCommandPoolCreateInfo poolInfo {
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			poolFlags,
			gpu.GetQueueFamilyByConcept(queue)
		};

		ET_ABORT_UNLESS(vkCreateCommandPool(gpu, ETAddressOf(poolInfo), gpu.GetAllocationCallbacks(), ETAddressOf(pool)));
		ET_AT_SCOPE_EXIT(vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks()));

		VkCommandBuffer                   commands[ETCountOf(_commands)];
		const VkCommandBufferAllocateInfo commandsInfo {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			/*pNext =*/nullptr,
			pool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			uint32(ETCountOf(commands))
		};

		ET_ABORT_UNLESS(vkAllocateCommandBuffers(gpu, ETAddressOf(commandsInfo), commands));
		ET_AT_SCOPE_EXIT(if (pool != nullptr) vkFreeCommandBuffers(gpu, pool, uint32(ETCountOf(commands)), commands));

		Swap(_pool, pool);
		Swap(_commands, commands);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void CommandList::FreeResources(Gpu& gpu) {
		if (VkCommandPool pool = eastl::exchange(_pool, nullptr)) {
			vkFreeCommandBuffers(gpu, pool, ETCountOf(_commands), _commands);
			vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks());
		}
	}

}}} // namespace Eldritch2::Graphics::Vulkan
