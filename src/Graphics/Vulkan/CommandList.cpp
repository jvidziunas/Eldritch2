/*==================================================================*\
  CommandList.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/Vulkan/CommandList.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/GraphicsScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		enum : VkDeviceSize { TightPack = 0u };

	} // anonymous namespace

	void GraphicsCommandList::ExecutePipeline(Framebuffer& target, const GraphicsPipeline& pipeline, Span<const RenderView*> views) ETNoexceptHint {
		const auto SetViewports([commands = _commands](InitializerList<VkViewport> viewports) ETNoexceptHint {
			vkCmdSetViewport(commands, 0u, uint32(viewports.size()), Begin(viewports));
		});

		const auto SetLayout([commands = _commands](VkPipelineBindPoint target, VkPipelineLayout layout, InitializerList<VkDescriptorSet> descriptorSets) ETNoexceptHint {
			vkCmdBindDescriptorSets(commands, target, layout, /*firstSet =*/0u, uint32(descriptorSets.size()), Begin(descriptorSets), /*dynamicOffsetCount =*/0u, /*pDynamicOffsets =*/nullptr);
			vkCmdPushConstants(commands, layout, target == VK_PIPELINE_BIND_POINT_GRAPHICS ? VK_SHADER_STAGE_ALL_GRAPHICS : VK_SHADER_STAGE_ALL, /*offset =*/0u, /*size =*/0u, nullptr);
		});

		const auto Dispatch([commands = _commands](VkEvent halfComplete, VkEvent complete, uint32 x, uint32 y, uint32 z) ETNoexceptHint {
			//	We can increase GPU utilization % by optionally kicking off two smaller dispatches in place of the single large operation requested by the user.
			if (ET_LIKELY(halfComplete == VK_NULL_HANDLE)) {
				vkCmdDispatch(commands, x, y, z);
			} else {
				vkCmdDispatch(commands, x / 2u, y, z);
				vkCmdSetEvent(commands, halfComplete, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
				vkCmdWaitEvents(commands, /*eventCount =*/1u, ETAddressOf(halfComplete), VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0u, nullptr, 0u, nullptr, 0u, nullptr);
				vkCmdDispatch(commands, x / 2u, y, z);
			}

			if (ET_UNLIKELY(complete != VK_NULL_HANDLE)) {
				vkCmdSetEvent(commands, complete, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
			}
		});

		// ---

		for (const RenderView& view : views) {
			const VkQueryPool viewTimings(target.GetTimingPool());
			VkPipelineLayout  currentLayout(NullVulkanHandle);
			uint32            timerIndex(0u);

			if (viewTimings != NullVulkanHandle) {
				vkCmdResetQueryPool(_commands, viewTimings, /*firstQuery =*/0u, /*queryCount =*/uint32(2u * target.GetStages().GetSize()));
			}

			SetViewports({ VkViewport{ view.viewport.x, view.viewport.y, view.viewport.width, view.viewport.height, /*minDepth =*/0.0f, /*maxDepth =*/1.0f } });

			for (const Framebuffer::StageFramebuffer& stage : target.GetStages()) {
				const VkPipelineBindPoint stageTarget(VK_PIPELINE_BIND_POINT_GRAPHICS);
				// Activate a new pipeline layout as necessary.
				if (Exchange(currentLayout, NullVulkanHandle) != NullVulkanHandle) {
					SetLayout(stageTarget, currentLayout, { target.GetShaderResources().GetDescriptorSet(), pipeline.GetShaderResources().GetDescriptorSet() });
				}

				const VkRenderPassBeginInfo beginInfo{
					VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
					/*pNext =*/nullptr,
					/*renderPass =*/stage.renderPass,
					/*framebuffer =*/stage.framebuffer,
					/*renderArea =*/VkRect2D{ ScreenOrigin, stage.GetRenderExtent() },
					/*clearValueCount =*/0u,
					/*pClearValues =*/nullptr
				};

				if (viewTimings != NullVulkanHandle) {
					vkCmdWriteTimestamp(_commands, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, viewTimings, timerIndex++);
				}
				vkCmdBeginRenderPass(_commands, ETAddressOf(beginInfo), VK_SUBPASS_CONTENTS_INLINE);

				vkCmdEndRenderPass(_commands);
				if (viewTimings != NullVulkanHandle) {
					vkCmdWriteTimestamp(_commands, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, viewTimings, timerIndex++);
				}
			}
		}
	}

	// ---------------------------------------------------

	TransferCommandList::TransferCommandList() ETNoexceptHint : _bufferBinds(BufferBindList::AllocatorType("Buffer Bind List Allocator")),
																_imageBinds(ImageBindList::AllocatorType("Image Bind List Allocator")),
																_imageOpaqueBinds(ImageOpaqueBindList::AllocatorType("Image Opaque Bind List Allocator")) {}

	// ---------------------------------------------------

	void TransferCommandList::QueuePush(VkImage target, VkBuffer source, VkOffset3D offset, VkExtent3D extent, VkImageSubresourceLayers subresource) {
		const VkDeviceSize size(extent.width * extent.height * extent.depth * subresource.layerCount);
		VkBufferImageCopy  copies[] = { VkBufferImageCopy{
            /*bufferOffset =*/0u, // Offset in staging buffer will be filled in below.
            TightPack,
            TightPack,
            subresource,
            offset,
            extent,
        } };
		vkCmdCopyBufferToImage(_commands, source, target, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, ETCountOf(copies), copies);
	}

	// ---------------------------------------------------

	void TransferCommandList::QueuePush(VkBuffer target, VkBuffer source, VkDeviceSize offset, VkDeviceSize extent) {
		VkBufferCopy copies[] = { VkBufferCopy{ /*srcOffset =*/0u, // Offset in staging buffer will be filled in below.
												offset,
												extent } };
		vkCmdCopyBuffer(_commands, source, target, ETCountOf(copies), copies);
	}

	// ---------------------------------------------------

	void TransferCommandList::QueuePull(VkBuffer target, VkImage source, VkOffset3D offset, VkExtent3D extent, VkImageSubresourceLayers subresource) {
		const VkDeviceSize size(extent.width * extent.height * extent.depth * subresource.layerCount);
		VkBufferImageCopy  copies[] = { VkBufferImageCopy{
            /*bufferOffset =*/0u, // Offset in staging buffer will be filled in below.
            TightPack,
            TightPack,
            subresource,
            offset,
            extent,
        } };
		vkCmdCopyImageToBuffer(_commands, source, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, target, ETCountOf(copies), copies);
	}

	// ---------------------------------------------------

	void TransferCommandList::QueuePull(VkBuffer target, VkBuffer source, VkDeviceSize offset, VkDeviceSize extent) {
		VkBufferCopy copies[] = { VkBufferCopy{ offset, /*dstOffset =*/0u, /* Offset in staging buffer will be filled in below. */ extent } };
		vkCmdCopyBuffer(_commands, source, target, ETCountOf(copies), copies);
	}

	// ---------------------------------------------------

	void TransferCommandList::QueueBind(VkBuffer buffer, const VkSparseMemoryBind& bind) {
		for (BufferBindList::Reference bindCommand : _bufferBinds) {
			if (Get<BindInfos>(bindCommand).buffer == buffer) {
				Get<BindLists>(bindCommand).EmplaceBack(bind);
				return;
			}
		}

		auto newBindInfo(_bufferBinds.EmplaceBack(CreateBindInfo(buffer), MemoryBindList(MemoryBindList::AllocatorType(""))));
		Get<BindLists>(newBindInfo).EmplaceBack(bind);
	}

	// ---------------------------------------------------

	void TransferCommandList::QueueBind(VkImage image, const VkSparseImageMemoryBind& bind) {
		for (ImageBindList::Reference bindCommand : _imageBinds) {
			if (Get<BindInfos>(bindCommand).image == image) {
				Get<BindLists>(bindCommand).EmplaceBack(bind);
				return;
			}
		}

		auto newBindInfo(_imageBinds.EmplaceBack(CreateBindInfo(image), ImageMemoryBindList(ImageMemoryBindList::AllocatorType(""))));
		Get<BindLists>(newBindInfo).EmplaceBack(bind);
	}

	// ---------------------------------------------------

	void TransferCommandList::QueueBind(VkImage image, const VkSparseMemoryBind& bind) {
		for (ImageOpaqueBindList::Reference bindCommand : _imageOpaqueBinds) {
			if (Get<BindInfos>(bindCommand).image == image) {
				Get<BindLists>(bindCommand).EmplaceBack(bind);
				return;
			}
		}

		auto newBindInfo(_imageOpaqueBinds.EmplaceBack(CreateOpaqueBindInfo(image), MemoryBindList(MemoryBindList::AllocatorType(""))));
		Get<BindLists>(newBindInfo).EmplaceBack(bind);
	}

	// ---------------------------------------------------

	VkResult TransferCommandList::Submit(Gpu& gpu, VkFence consumed, uint32 waitCount, const VkSemaphore waits[], const VkPipelineStageFlags stages[]) ETNoexceptHint {
		static ETConstexpr auto AttachBinds([](auto command) ETNoexceptHint {
			auto& bindInfo(Get<BindInfos>(command));
			auto& binds(Get<BindLists>(command));

			bindInfo.bindCount = uint32_t(binds.GetSize());
			bindInfo.pBinds    = binds.GetData();
		});

		// ---

		ForEach(_bufferBinds.Begin(), _bufferBinds.End(), AttachBinds);
		ForEach(_imageBinds.Begin(), _imageBinds.End(), AttachBinds);
		ForEach(_imageOpaqueBinds.Begin(), _imageOpaqueBinds.End(), AttachBinds);

		VkSemaphore          transferWaits[] = { NullVulkanHandle };
		VkPipelineStageFlags transferWaitStages[ETCountOf(transferWaits)];
		uint32               transferWaitCount(0u);

		if (_bufferBinds || _imageBinds || _imageOpaqueBinds) {
			const VkBindSparseInfo binds[] = {
				VkBindSparseInfo{ // clang-format off
				VK_STRUCTURE_TYPE_BIND_SPARSE_INFO,
				/*pNext =*/nullptr,
				/*waitSemaphoreCount =*/0u,
				/*pWaitSemaphores =*/nullptr,
				uint32_t(_bufferBinds.GetSize()),
				_bufferBinds.GetData<VkSparseBufferMemoryBindInfo>(),
				uint32_t(_imageOpaqueBinds.GetSize()),
				_imageOpaqueBinds.GetData<VkSparseImageOpaqueMemoryBindInfo>(),
				uint32_t(_imageBinds.GetSize()),
				_imageBinds.GetData<VkSparseImageMemoryBindInfo>(),
				/*signalSemaphoreCount =*/ETCountOf(transferWaits),
				/*pSignalSemaphores =*/transferWaits } }; // clang-format on

			ET_ABORT_UNLESS(gpu.GetTemporarySemaphore(transferWaits[transferWaitCount]));
			transferWaitStages[transferWaitCount++] = VK_PIPELINE_STAGE_TRANSFER_BIT;

			ET_ABORT_UNLESS(gpu.GetQueue(SparseBinding).Bind(NullVulkanHandle, ETCountOf(binds), binds));
		}

		const VkSubmitInfo submits[] = {
			VkSubmitInfo{ // clang-format off
			VK_STRUCTURE_TYPE_SUBMIT_INFO,
			/*pNext =*/nullptr,
			/*waitSemaphoreCount =*/ETCountOf(transferWaits),
			/*pWaitSemaphores =*/transferWaits,
			transferWaitStages,
			/*commandBufferCount =*/0u,
			/*pCommandBuffers =*/nullptr,
			/*signalSemaphoreCount =*/0u,
			/*pSignalSemaphores =*/nullptr				   
		} }; // clang-format on
		return AbstractCommandList::Submit(gpu, consumed, ETCountOf(submits), submits);
	}

	// ---------------------------------------------------

	VkResult TransferCommandList::Submit(Gpu& gpu, VkFence consumed) ETNoexceptHint {
		return Submit(gpu, consumed, /*waitCount*/ 0u, /*waits =*/nullptr, /*stages =*/nullptr);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
