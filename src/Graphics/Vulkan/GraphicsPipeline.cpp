/*==================================================================*\
  GraphicsPipeline.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.hpp>
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		ETConstexpr ETInlineHint VkAttachmentDescription AsVkAttachmentDescription(uint32 passIndex, const PipelinePassDescription& pass, const PipelineAttachmentDescription& attachment) {
			return VkAttachmentDescription {
				/*flags=*/0u,
				attachment.format,
				GetSampleCountFlags(pass.framebuffer.samplesPerPixel),
				attachment.GetLoadOp(passIndex),
				attachment.GetStoreOp(passIndex),
				/*stencilLoadOp =*/attachment.GetLoadOp(passIndex),
				/*stencilStoreOp =*/attachment.GetStoreOp(passIndex),
				layout,
				layout
			};
		}

	} // anonymous namespace

#if 0
	VkImageViewCreateInfo GraphicsPipeline::ImageDescription::GetViewCreateInfo(VkImage image, const PipelineAttachmentReference& context, void* extensions) const ETNoexceptHint {
		return VkImageViewCreateInfo {
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			/*pNext =*/extensions,
			/*flags =*/viewFlags,
			image,
			viewType,
			format,
			{ VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY },
			VkImageSubresourceRange {
				GetAspectsByUsage(usages),
				context.mipLevel,
				/*levelCount =*/1u, // Vulkan requires all image views bound as attachments reference exactly 1 MIP level.
				context.firstLayer,
				context.layers }
		};
#endif

	// ---------------------------------------------------

	GraphicsPipeline::Pass::~Pass() {
		ET_ASSERT(renderPass == nullptr, "Leaking Vulkan render pass {}!", fmt::ptr(renderPass));
	}

	// ---------------------------------------------------

	VkResult GraphicsPipeline::Pass::BindResources(Gpu& gpu, const PipelinePassDescription& pass, const PipelineAttachmentDescription globalAttachments[]) {
		using ::Eldritch2::Swap;

		/*	The mapping from source/virtual attachment index to physical attachment index is a little more complicated than a direct 1:1 assignment.
		 *	Pipeline builders reference a global index (that is, the index of the attachment in the list of all attachments assigned to *any* pass)
		 *	but individual render passes (and thus Vulkan) only ever see the index within the *current* pass.*/
		VkAttachmentReference    inputs[ETCountOf(pass.framebuffer.inputAttachments)];
		VkAttachmentReference    outputs[ETCountOf(pass.framebuffer.colorAttachments)];
		VkAttachmentReference    depthStencil;
		VkAttachmentDescription  attachments[ETCountOf(inputs) + ETCountOf(outputs) + /*ETCountOf(depthStencil) =*/1];
		VkAttachmentDescription* attachmentsEnd(attachments);

		const auto AsVkAttachmentReference([&](const PipelineAttachmentReference& reference, VkImageLayout layout) -> VkAttachmentReference {
			if (reference.globalIndex == VK_ATTACHMENT_UNUSED) {
				return VkAttachmentReference { VK_ATTACHMENT_UNUSED, VK_IMAGE_LAYOUT_UNDEFINED };
			}

			*attachmentsEnd = AsVkAttachmentDescription(0u, pass, globalAttachments[reference.globalIndex]);
			return VkAttachmentReference { uint32(eastl::distance(attachments, attachmentsEnd++)), layout };
		});

		const VkSubpassDescription subpasses[] = {
			VkSubpassDescription { /*flags =*/0u,
								   VK_PIPELINE_BIND_POINT_GRAPHICS,
								   uint32(eastl::distance(inputs, Transform(pass.framebuffer.inputAttachments, End(pass.framebuffer.inputAttachments), inputs, AsVkAttachmentReference))), inputs,
								   uint32(eastl::distance(outputs, Transform(pass.framebuffer.colorAttachments, End(pass.framebuffer.colorAttachments), outputs, AsVkAttachmentReference))), outputs,
								   /*pResolveAttachments =*/nullptr,
								   ETAddressOf(depthStencil = AsVkAttachmentReference(pass.framebuffer.depthAttachment, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)),
								   /*preserveAttachmentCount =*/0u, /*pPreserveAttachments =*/nullptr }
		};

		const VkSubpassDependency subpassDependencies[] = {
			VkSubpassDependency {}
		};

		VkRenderPass                 renderPass;
		const VkRenderPassCreateInfo renderPassInfo {
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			uint32(eastl::distance(attachments, attachmentsEnd)), attachments,
			ETCountOf(subpasses), subpasses,
			/*dependencyCount =*/0u, subpassDependencies
		};
		ET_ABORT_UNLESS(vkCreateRenderPass(gpu, ETAddressOf(renderPassInfo), gpu.GetAllocationCallbacks(), ETAddressOf(renderPass)));
		ET_AT_SCOPE_EXIT(vkDestroyRenderPass(gpu, renderPass, gpu.GetAllocationCallbacks()));

		Swap(this->renderPass, renderPass);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GraphicsPipeline::Pass::FreeResources(Gpu& gpu) {
		vkDestroyRenderPass(gpu, eastl::exchange(renderPass, nullptr), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	GraphicsPipeline::GraphicsPipeline() {}

	// ---------------------------------------------------

	GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& pipeline) :
		GraphicsPipeline() {
		Swap(*this, pipeline);
	}

	// ---------------------------------------------------

	VkResult GraphicsPipeline::BindResources(Gpu& gpu, const GraphicsPipelineBuilder& pipeline, size_t commandPoolCount) {
		using ::Eldritch2::Swap;

		DescriptorTable shaderResources;
		ET_ABORT_UNLESS(shaderResources.BindResources(gpu,
													  /*pools =*/ { VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 512u } },
													  /*bindings =*/ { VkDescriptorSetLayoutBinding { /*binding =*/0u, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
																									  /*descriptorCount =*/512u,
																									  /*stageFlags =*/VK_SHADER_STAGE_ALL,
																									  /*pImmutableSamplers =*/nullptr } }));
		ET_AT_SCOPE_EXIT(shaderResources.FreeResources(gpu));

		BatchCoordinator batches;
		ET_ABORT_UNLESS(batches.BindResources(gpu, pipeline, commandPoolCount));

		Swap(_shaderResources, shaderResources);
		Swap(_batches, batches);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GraphicsPipeline::FreeResources(Gpu& gpu) {
		_batches.FreeResources(gpu);
		_shaderResources.FreeResources(gpu);
	}

	// ---------------------------------------------------

	void Swap(GraphicsPipeline& lhs, GraphicsPipeline& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._shaderResources, rhs._shaderResources);
		Swap(lhs._batches, rhs._batches);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
