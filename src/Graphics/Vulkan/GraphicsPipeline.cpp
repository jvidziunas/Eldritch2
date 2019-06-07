/*==================================================================*\
  GraphicsPipeline.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
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
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	GraphicsPipeline::Stage::~Stage() {
		ETAssert(renderPass == NullVulkanHandle, "Leaking Vulkan render pass {}!", fmt::ptr(renderPass));
	}

	// ---------------------------------------------------

	VkResult GraphicsPipeline::Stage::BindResources(Gpu& gpu, const StageBuilder& stage, const VkAttachmentDescription attachments_[]) {
		using ::Eldritch2::Swap;

		const auto AsAttachmentDescription([&](const StageAttachment& attachment, VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) ETNoexceptHint -> VkAttachmentDescription {
			return VkAttachmentDescription{
				/*flags=*/0u,
				attachments_[attachment.globalIndex].format,
				GetSampleCountFlags(stage.samplesPerPixel),
				attachment.shouldLoad ? VK_ATTACHMENT_LOAD_OP_LOAD : VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				attachment.shouldStore ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE,
				/*stencilLoadOp =*/VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				/*stencilStoreOp =*/VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				/*initialLayout =*/attachment.shouldLoad ? layout : VK_IMAGE_LAYOUT_UNDEFINED,
				/*finalLayout =*/layout
			};
		});

		// ---

		/*	The mapping from source/virtual attachment index to physical attachment index is a little more complicated than a direct 1:1 assignment.
		 *	Pipeline builders reference a global index (that is, the index of the attachment in the list of all attachments assigned to *any* pass)
		 *	but individual render passes (and thus Vulkan) only ever see the index within the *current* pass.*/
		VkAttachmentDescription attachments[ETCountOf(stage.inputAttachments) + ETCountOf(stage.colorAttachments) + 1];
		VkAttachmentReference   references[ETCountOf(attachments)];
		const auto              firstInputAttachment(attachments);
		const auto              firstColorAttachment(Transform(stage.inputAttachments, End(stage.inputAttachments), firstInputAttachment, AsAttachmentDescription));
		const auto              lastColorAttachment(Transform(stage.colorAttachments, End(stage.colorAttachments), firstColorAttachment, AsAttachmentDescription));
		const auto              depthAttachment(lastColorAttachment);

		*depthAttachment = AsAttachmentDescription(stage.depthAttachment, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

		const VkSubpassDescription subpasses[] = {
			VkSubpassDescription{
				/*flags =*/0u,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				uint32(eastl::distance(firstInputAttachment, firstColorAttachment)), firstInputAttachment,
				uint32(eastl::distance(firstColorAttachment, lastColorAttachment)), firstColorAttachment,
				/*pResolveAttachments =*/nullptr, depthAttachment,
				/*preserveAttachmentCount =*/0u, /*pPreserveAttachments =*/nullptr },
		};
		const VkSubpassDependency dependencies[] = {
			VkSubpassDependency{
				/*srcSubpass =*/VK_SUBPASS_EXTERNAL,
				/*dstSubpass =*/0u,
				/*srcStageMask =*/VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
				/*dstStageMask =*/VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
				/*srcAccessMask =*/VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
				/*dstAccessMask =*/VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
				/*dependencyFlags =*/0u },
		};

		VkRenderPass                 renderPass;
		const VkRenderPassCreateInfo renderPassInfo{
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			uint32(eastl::distance(firstInputAttachment, depthAttachment + 1)), attachments,
			ETCountOf(subpasses), subpasses,
			/*dependencyCount =*/0u, dependencies
		};
		ET_ABORT_UNLESS(vkCreateRenderPass(gpu, ETAddressOf(renderPassInfo), gpu.GetAllocationCallbacks(), ETAddressOf(renderPass)));
		ET_AT_SCOPE_EXIT(vkDestroyRenderPass(gpu, renderPass, gpu.GetAllocationCallbacks()));

		Swap(this->renderPass, renderPass);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GraphicsPipeline::Stage::FreeResources(Gpu& gpu) ETNoexceptHint {
		vkDestroyRenderPass(gpu, Exchange(renderPass, NullVulkanHandle), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& pipeline) ETNoexceptHint : GraphicsPipeline() {
		Swap(*this, pipeline);
	}

	// ---------------------------------------------------

	VkResult GraphicsPipeline::BindResourceViews(Gpu& gpu) {
		return _shaderResources.BindResourceViews(gpu, /*begin =*/0u, /*end =*/0u);
	}

	// ---------------------------------------------------

	VkResult GraphicsPipeline::PushToGpu(Gpu& gpu) {
		return _shaderResources.PushToGpu(gpu, /*begin =*/0u, /*end =*/0u);
	}

	// ---------------------------------------------------

	VkResult GraphicsPipeline::BindResources(Gpu& gpu, const PipelineBuilder& pipeline, PipelineBatcher::CommandPoolList::SizeType poolCount) {
		using ::Eldritch2::Swap;

		PipelineBatcher batches;
		ET_ABORT_UNLESS(batches.BindResources(gpu, Span<const byte*>(), pipeline, poolCount));

		DescriptorTable shaderResources;
		ET_ABORT_UNLESS(shaderResources.BindResources(gpu, /*layout =*/NullVulkanHandle, /*pools =*/{ VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 512u } }));
		ET_AT_SCOPE_EXIT(shaderResources.FreeResources(gpu));

		Swap(_batches, batches);
		Swap(_shaderResources, shaderResources);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GraphicsPipeline::FreeResources(Gpu& gpu) ETNoexceptHint {
		_shaderResources.FreeResources(gpu);
		_batches.FreeResources(gpu);
	}

	// ---------------------------------------------------

	void Swap(GraphicsPipeline& lhs, GraphicsPipeline& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._batches, rhs._batches);
		Swap(lhs._shaderResources, rhs._shaderResources);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
