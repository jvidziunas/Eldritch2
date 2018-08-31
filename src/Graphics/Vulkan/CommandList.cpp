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
		_commands(nullptr) {}

	// ---------------------------------------------------

	CommandList::~CommandList() {
		ET_ASSERT(_pool == nullptr, "Leaking Vulkan command pool!");
	}

	// ---------------------------------------------------

	void CommandList::ExecutePipeline(const Framebuffer& target, const GraphicsPipeline& pipeline, const DescriptorTable& descriptors, const ArrayList<View>& views) {
		const Framebuffer::Pass* const passes(target.GetPasses());
		const VkDescriptorSet          sets[] = { descriptors.GetDescriptorSet() };

		vkCmdResetQueryPool(_commands, target.GetTimingPool(), /*firstQuery =*/0u, /*queryCount =*/2u * pipeline.GetPassCount());
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

			vkCmdBindDescriptorSets(_commands, VK_PIPELINE_BIND_POINT_GRAPHICS, VK_NULL_HANDLE, /*firstSet =*/0u, ETCountOf(sets), sets, /*dynamicOffsetCount =*/0u, /*pDynamicOffsets =*/nullptr);
			vkCmdWriteTimestamp(_commands, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, target.GetTimingPool(), 2u * pass);
			vkCmdBeginRenderPass(_commands, ETAddressOf(passInfo), VK_SUBPASS_CONTENTS_INLINE);

			for (const View& view : views) {
				vkCmdSetViewport(_commands, /*firstViewport =*/0u, /*viewportCount =*/1u, ETAddressOf(view.affineViewport));
			}

			vkCmdEndRenderPass(_commands);
			vkCmdWriteTimestamp(_commands, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, target.GetTimingPool(), 2u * pass + 1u);
		}
	}

	// ---------------------------------------------------

	void CommandList::CompositeImage(const Framebuffer& /*target*/, const Framebuffer& /*source*/) {
		//	vkCmdBlitImage(_commands, )
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

		ET_ABORT_UNLESS(vkCreateCommandPool(gpu, ETAddressOf(poolInfo), gpu.GetAllocationCallbacks(), ETAddressOf(pool)));
		ET_AT_SCOPE_EXIT(vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks()));

		VkCommandBuffer                   commands(nullptr);
		const VkCommandBufferAllocateInfo commandsInfo {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			/*pNext =*/nullptr,
			pool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			/*commandBufferCount =*/1u
		};

		ET_ABORT_UNLESS(vkAllocateCommandBuffers(gpu, ETAddressOf(commandsInfo), ETAddressOf(commands)));
		ET_AT_SCOPE_EXIT(if (commands != nullptr) vkFreeCommandBuffers(gpu, pool, commandsInfo.commandBufferCount, ETAddressOf(commands)););

		Swap(_pool, pool);
		Swap(_commands, commands);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void CommandList::FreeResources(Gpu& gpu) {
		if (VkCommandBuffer commands = eastl::exchange(_commands, nullptr)) {
			vkFreeCommandBuffers(gpu, _pool, /*commandBufferCount =*/1u, ETAddressOf(commands));
		}

		vkDestroyCommandPool(gpu, eastl::exchange(_pool, nullptr), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	VkResult CommandList::BeginRecording(Gpu& gpu, VkCommandPoolResetFlags resetFlags, VkCommandBufferUsageFlags usages) ETNoexceptHint {
		ET_ABORT_UNLESS(vkResetCommandPool(gpu, _pool, resetFlags));

		const VkCommandBufferBeginInfo beginInfo {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			/*pNext =*/nullptr,
			usages,
			/*pInheritanceInfo =*/nullptr
		};

		return vkBeginCommandBuffer(_commands, &beginInfo);
	}

	// ---------------------------------------------------

	VkResult CommandList::FinishRecording() ETNoexceptHint {
		return vkEndCommandBuffer(_commands);
	}

	// ---------------------------------------------------

	void CommandList::Dispatch(VkEvent halfComplete, VkEvent complete, uint32_t x, uint32_t y, uint32_t z) ETNoexceptHint {
		const bool useAtomicDispatch(halfComplete == VK_NULL_HANDLE);

		if (useAtomicDispatch) {
			vkCmdDispatch(_commands, x, y, z);
		} else {
			//	We can increase GPU utilization % by optionally kicking off two smaller dispatches in place of the single large operation requested by the user.
			vkCmdDispatch(_commands, x / 2u, y, z);
			vkCmdSetEvent(_commands, halfComplete, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
			vkCmdWaitEvents(_commands, 1u, ETAddressOf(halfComplete), VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0u, nullptr, 0u, nullptr, 0u, nullptr);
			vkCmdDispatch(_commands, x / 2u, y, z);
		}

		if (complete != VK_NULL_HANDLE) {
			vkCmdSetEvent(_commands, complete, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
		}
	}

}}} // namespace Eldritch2::Graphics::Vulkan
