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

	GraphicsPipeline::AttachmentDescription::AttachmentDescription(const GraphicsPipelineBuilder::AttachmentDescription& attachment) ETNoexceptHint : _format(attachment.format),
																																					  _flags(attachment.flags),
																																					  _usages(attachment.usages),
																																					  _sampleRates(attachment.sampleRates),
																																					  _arraySize(1u),
																																					  _staticResolution(attachment.staticResolution != 0u) {
		if (_staticResolution) {
			_resolution = attachment.resolution;
		} else {
			_scale = attachment.scale;
		}
	}

	// ---------------------------------------------------

	VkImageViewCreateInfo GraphicsPipeline::AttachmentDescription::GetImageViewCreateInfo(VkImage image, uint32 mip, void* extensions) const ETNoexceptHint {
		return VkImageViewCreateInfo {
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			image,
			GetViewType(),
			_format,
			MakeIdentitySwizzle(),
			VkImageSubresourceRange {
				GetAspectsByUsage(attachment.usages),
				/*baseMipLevel =*/mip,
				/*levelCount =*/1u, // Vulkan requires all image views bound as attachments have exactly 1 MIP level.
				/*baseArrayLayer =*/0u,
				VK_REMAINING_ARRAY_LAYERS }
		};
	}

	// ---------------------------------------------------

	VkImageCreateInfo GraphicsPipeline::AttachmentDescription::GetImageCreateInfo(VkExtent2D baseResolution, uint32 layers, void* extensions) const ETNoexceptHint {
		return VkImageCreateInfo {
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			/*pNext =*/extensions,
			_flags,
			GetType(),
			_format,
			GetResolution(baseResolution),
			/*mipLevels =*/1u,
			GetLayers(layers),
			VkSampleCountFlagBits(_sampleRates),
			VK_IMAGE_TILING_OPTIMAL,
			_usages,
			VK_SHARING_MODE_EXCLUSIVE,
			/*queueFamilyIndexCount =*/0u, /*pQueueFamilyIndices =*/nullptr,
			VK_IMAGE_LAYOUT_UNDEFINED
		};
	}

	// ---------------------------------------------------

	GraphicsPipeline::PassDescription::PassDescription(GeometryType source) ETNoexceptHint : source(source),
																							 renderPass(nullptr),
																							 maxScale(1.0f),
																							 minScale(1.0f),
																							 depthStencilAttachment(VK_ATTACHMENT_UNUSED) {
		Fill(colorAttachments, End(colorAttachments), VK_ATTACHMENT_UNUSED);
		Fill(inputAttachments, End(inputAttachments), VK_ATTACHMENT_UNUSED);
	}

	// ---------------------------------------------------

	GraphicsPipeline::PassDescription::~PassDescription() {
		ET_ASSERT(renderPass == nullptr, "Leaking Vulkan render pass!");
	}

	// ---------------------------------------------------

	VkResult GraphicsPipeline::PassDescription::BindResources(Gpu& gpu, const GraphicsPipelineBuilder& pipeline, uint32 passIndex) {
		using ::Eldritch2::Swap;

		using AttachmentReference = GraphicsPipelineBuilder::AttachmentReference;
		using PipelineAttachment  = GraphicsPipelineBuilder::AttachmentDescription;

		const bool isFinalPass(passIndex == pipeline.GetPassCount() - 1);
		const auto AsAttachmentDescription([&](const AttachmentReference& reference) -> VkAttachmentDescription {
			const PipelineAttachment& attachment(pipeline.GetAttachment(reference.globalIndex));
			// We can optimize out the first load/last store unless the attachment contents are marked as persisting across frames.
			const bool containsData(attachment.isPersistent || attachment.firstWrite < passIndex);
			const bool mayBeRead(isFinalPass || attachment.isPersistent || attachment.lastRead < passIndex);
			return VkAttachmentDescription {
				/*flags=*/0u,
				attachment.format,
				VkSampleCountFlagBits(attachment.sampleRates),
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
		 *	but individual render passes (and thus Vulkan) only ever see the index within the *current* pass.*/
		VkAttachmentDescription attachments[ETCountOf(inputAttachments) + ETCountOf(colorAttachments) + 1];
		uint32                  currentAttachment(0u);
		VkAttachmentReference   inputs[ETCountOf(PassDescription::inputAttachments)];
		uint32                  inputCount(0u);
		for (const AttachmentReference& reference : pipeline[passIndex].inputAttachments) {
			if (reference.globalIndex == VK_ATTACHMENT_UNUSED) {
				continue;
			}
			inputs[inputCount++]             = VkAttachmentReference { currentAttachment, reference.layout };
			attachments[currentAttachment++] = AsAttachmentDescription(reference);
		}

		VkAttachmentReference outputs[ETCountOf(PassDescription::colorAttachments)];
		uint32                outputCount(0u);
		for (const AttachmentReference& reference : pipeline[passIndex].colorAttachments) {
			if (reference.globalIndex == VK_ATTACHMENT_UNUSED) {
				continue;
			}
			outputs[outputCount++]           = VkAttachmentReference { currentAttachment, reference.layout };
			attachments[currentAttachment++] = AsAttachmentDescription(reference);
		}

		VkAttachmentReference depthStencil { VK_ATTACHMENT_UNUSED, VK_IMAGE_LAYOUT_UNDEFINED };
		if (pipeline[passIndex].depthStencilAttachment.globalIndex != VK_ATTACHMENT_UNUSED) {
			depthStencil                     = VkAttachmentReference { currentAttachment, pipeline[passIndex].depthStencilAttachment.layout };
			attachments[currentAttachment++] = AsAttachmentDescription(pipeline[passIndex].depthStencilAttachment);
		}

		const VkSubpassDescription passDescription {
			/*flags =*/0u,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			inputCount, inputs,
			outputCount, outputs,
			/*pResolveAttachments =*/nullptr,
			ETAddressOf(depthStencil),
			/*preserveAttachmentCount =*/0u, /*pPreserveAttachments =*/nullptr
		};

		VkRenderPass                 renderPass;
		const VkRenderPassCreateInfo renderPassInfo {
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			currentAttachment,
			attachments,
			/*subpassCount =*/1u, /*pSubpasses =*/ETAddressOf(passDescription),
			/*dependencyCount =*/0u, /*pDependencies =*/nullptr
		};
		ET_ABORT_UNLESS(vkCreateRenderPass(gpu, ETAddressOf(renderPassInfo), gpu.GetAllocationCallbacks(), ETAddressOf(renderPass)));
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
		_attachments(MallocAllocator("Vulkan Render Pipeline Framebuffer Attachment Description List Allocator")),
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

		static ETConstexpr VkDescriptorSetLayoutBinding descriptorBindings[] = {
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
			uint32_t(ETCountOf(descriptorBindings)), descriptorBindings
		};
		ET_ABORT_UNLESS(vkCreateDescriptorSetLayout(gpu, ETAddressOf(descriptorLayoutInfo), gpu.GetAllocationCallbacks(), ETAddressOf(descriptorLayout)));
		ET_AT_SCOPE_EXIT(vkDestroyDescriptorSetLayout(gpu, descriptorLayout, gpu.GetAllocationCallbacks()));

		ArrayList<AttachmentDescription> attachments(_attachments.GetAllocator(), pipeline.GetAttachmentCount());
		for (uint32 attachment(0u); attachment < pipeline.GetAttachmentCount(); ++attachment) {
			attachments.EmplaceBack(pipeline.GetAttachment(attachment));
		}

		ArrayList<PassDescription> passes(_passes.GetAllocator());
		ET_AT_SCOPE_EXIT(for (PassDescription& pass
							  : passes) pass.FreeResources(gpu));

		passes.Resize(pipeline.GetPassCount());
		for (uint32 pass(0u); pass < pipeline.GetPassCount(); ++pass) {
			ET_ABORT_UNLESS(passes[pass].BindResources(gpu, pipeline, pass));
		}

		Swap(_descriptorLayout, descriptorLayout);
		Swap(_attachments, attachments);
		Swap(_passes, passes);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GraphicsPipeline::FreeResources(Gpu& gpu) {
		for (PassDescription& pass : _passes) {
			pass.FreeResources(gpu);
		}
		_passes.Clear(ReleaseMemorySemantics());
		_attachments.Clear(ReleaseMemorySemantics());
		vkDestroyDescriptorSetLayout(gpu, eastl::exchange(_descriptorLayout, nullptr), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	void Swap(GraphicsPipeline& lhs, GraphicsPipeline& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._descriptorLayout, rhs._descriptorLayout);
		Swap(lhs._attachments, rhs._attachments);
		Swap(lhs._passes, rhs._passes);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
