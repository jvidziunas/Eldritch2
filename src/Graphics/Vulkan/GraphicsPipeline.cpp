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

	VkExtent3D GraphicsPipeline::AttachmentDescription::GetDimensions(VkExtent2D base, uint32 layers) const {
		if (dynamicResolution) {
			return VkExtent3D { uint32(base.width * scale[0]), uint32(base.height * scale[1]), 1u };
		}

		return dimensions;
	}

	// ---------------------------------------------------

	GraphicsPipeline::PassDescription::PassDescription(GeometryType source) :
		source(source),
		renderPass(nullptr),
		maxScale(1.0f),
		minScale(1.0f),
		colorAttachments { VK_ATTACHMENT_UNUSED, VK_ATTACHMENT_UNUSED, VK_ATTACHMENT_UNUSED, VK_ATTACHMENT_UNUSED },
		inputAttachments { VK_ATTACHMENT_UNUSED, VK_ATTACHMENT_UNUSED, VK_ATTACHMENT_UNUSED, VK_ATTACHMENT_UNUSED },
		depthStencilAttachment(VK_ATTACHMENT_UNUSED) {}

	// ---------------------------------------------------

	GraphicsPipeline::PassDescription::~PassDescription() {
		ET_ASSERT(renderPass == nullptr, "Leaking Vulkan render pass!");
	}

	// ---------------------------------------------------

	VkResult GraphicsPipeline::PassDescription::BindResources(Gpu& gpu, const GraphicsPipelineBuilder& pipeline, uint32 passIndex) {
		using AttachmentReference = GraphicsPipelineBuilder::AttachmentReference;
		using PipelineAttachment  = GraphicsPipelineBuilder::AttachmentDescription;
		using ::Eldritch2::Swap;

		const bool isFinalPass(passIndex == pipeline.GetPassCount() - 1);
		const auto AsAttachmentDescription([&](const AttachmentReference& reference) -> VkAttachmentDescription {
			const PipelineAttachment& attachment(pipeline.GetAttachment(reference.globalIndex));
			const bool                containsData(attachment.isPersistent || attachment.firstWrite < passIndex);
			const bool                mayBeRead(isFinalPass || attachment.isPersistent || attachment.lastRead < passIndex);

			// We can optimize out the first load/last store unless the attachment contents are marked as persisting across frames.
			return VkAttachmentDescription {
				/*flags=*/0u,
				attachment.format,
				VkSampleCountFlagBits(attachment.msaaQuality),
				/*loadOp =*/containsData ? VK_ATTACHMENT_LOAD_OP_LOAD : VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				/*storeOp =*/mayBeRead ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE,
				/*stencilLoadOp =*/containsData ? VK_ATTACHMENT_LOAD_OP_LOAD : VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				/*stencilStoreOp =*/mayBeRead ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE,
				reference.layout,
				reference.layout
			};
		});

		/*	The mapping from source/virtual attachment index to physical attachment index is a little more complicated than a direct 1:1 assignment.
		 *	Pipeline builders reference a global index (that is, the index of the attachment in the list of all attachments assigned to *any* pass)
		 *	but the Vulkan render passes only ever see the index within the *current* pass.*/
		VkAttachmentDescription attachments[_countof(inputAttachments) + _countof(colorAttachments) + 1];
		uint32                  attachmentCount(0u);
		VkAttachmentReference   inputAttachments[_countof(GraphicsPipelineBuilder::PassDescription::inputAttachments)];
		uint32                  inputAttachmentCount(0u);
		for (const AttachmentReference& reference : pipeline[passIndex].inputAttachments) {
			if (reference.globalIndex == VK_ATTACHMENT_UNUSED) {
				continue;
			}
			inputAttachments[inputAttachmentCount++] = VkAttachmentReference { attachmentCount, reference.layout };
			attachments[attachmentCount++]           = AsAttachmentDescription(reference);
		}

		VkAttachmentReference colorAttachments[_countof(GraphicsPipelineBuilder::PassDescription::colorAttachments)];
		uint32                colorAttachmentCount(0u);
		for (const AttachmentReference& reference : pipeline[passIndex].colorAttachments) {
			if (reference.globalIndex == VK_ATTACHMENT_UNUSED) {
				continue;
			}
			colorAttachments[colorAttachmentCount++] = VkAttachmentReference { attachmentCount, reference.layout };
			attachments[attachmentCount++]           = AsAttachmentDescription(reference);
		}

		VkAttachmentReference depthStencilAttachment { VK_ATTACHMENT_UNUSED, VK_IMAGE_LAYOUT_UNDEFINED };
		if (pipeline[passIndex].depthStencilAttachment.globalIndex != VK_ATTACHMENT_UNUSED) {
			depthStencilAttachment         = VkAttachmentReference { attachmentCount, pipeline[passIndex].depthStencilAttachment.layout };
			attachments[attachmentCount++] = AsAttachmentDescription(pipeline[passIndex].depthStencilAttachment);
		}

		const VkSubpassDescription passDescription {
			/*flags =*/0u,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			inputAttachmentCount,
			inputAttachments,
			colorAttachmentCount,
			colorAttachments,
			/*pResolveAttachments =*/nullptr,
			&depthStencilAttachment,
			/*preserveAttachmentCount =*/0u,
			/*pPreserveAttachments =*/nullptr
		};

		VkRenderPass                 renderPass;
		const VkRenderPassCreateInfo renderPassInfo {
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			attachmentCount,
			attachments,
			/*subpassCount =*/1u,
			/*pSubpasses =*/&passDescription,
			/*dependencyCount =*/0u,
			/*pDependencies =*/nullptr
		};
		ET_FAIL_UNLESS(vkCreateRenderPass(gpu, &renderPassInfo, gpu.GetAllocationCallbacks(), &renderPass));
		ET_AT_SCOPE_EXIT(vkDestroyRenderPass(gpu, renderPass, gpu.GetAllocationCallbacks()));

		Swap(this->renderPass, renderPass);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GraphicsPipeline::PassDescription::FreeResources(Gpu& gpu) {
		vkDestroyRenderPass(gpu, eastl::exchange(renderPass, nullptr), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	GraphicsPipeline::GraphicsPipeline() :
		_descriptorLayout(nullptr),
		_framebufferAttachments(MallocAllocator("Vulkan Render Pipeline Framebuffer Attachment Description List Allocator")),
		_passes(MallocAllocator("Vulkan Render Pipeline Pass List Allocator")) {}

	// ---------------------------------------------------

	GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& pipeline) :
		GraphicsPipeline() {
		Swap(*this, pipeline);
	}

	// ---------------------------------------------------

	GraphicsPipeline::~GraphicsPipeline() {
		ET_ASSERT(_descriptorLayout == nullptr, "Leaking Vulkan descriptor set layout!");
	}

	// ---------------------------------------------------

	VkResult GraphicsPipeline::BindResources(Gpu& gpu, const GraphicsPipelineBuilder& pipeline) {
		using ::Eldritch2::Swap;

		static const VkDescriptorSetLayoutBinding descriptorBindings[] = {
			VkDescriptorSetLayoutBinding {
				/*binding =*/0u,
				VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				/*descriptorCount =*/512u,
				VK_SHADER_STAGE_ALL,
				/*pImmutableSamplers =*/nullptr }
		};

		VkDescriptorSetLayout                 descriptorLayout;
		const VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo {
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			uint32_t(_countof(descriptorBindings)), descriptorBindings
		};
		ET_FAIL_UNLESS(vkCreateDescriptorSetLayout(gpu, &descriptorLayoutInfo, gpu.GetAllocationCallbacks(), &descriptorLayout));
		ET_AT_SCOPE_EXIT(vkDestroyDescriptorSetLayout(gpu, descriptorLayout, gpu.GetAllocationCallbacks()));

		ArrayList<PassDescription> passes(_passes.GetAllocator());
		ET_AT_SCOPE_EXIT(for (PassDescription& pass
							  : passes) pass.FreeResources(gpu));

		passes.Resize(pipeline.GetPassCount());
		for (uint32 pass(0u); pass < pipeline.GetPassCount(); ++pass) {
			ET_FAIL_UNLESS(passes[pass].BindResources(gpu, pipeline, pass));
		}

		Swap(_descriptorLayout, descriptorLayout);
		Swap(_passes, passes);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GraphicsPipeline::FreeResources(Gpu& gpu) {
		for (PassDescription& pass : _passes) {
			pass.FreeResources(gpu);
		}
		vkDestroyDescriptorSetLayout(gpu, eastl::exchange(_descriptorLayout, nullptr), gpu.GetAllocationCallbacks());

		_passes.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	void Swap(GraphicsPipeline& lhs, GraphicsPipeline& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._descriptorLayout, rhs._descriptorLayout);
		Swap(lhs._passes, rhs._passes);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
