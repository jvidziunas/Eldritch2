/*==================================================================*\
  GraphicsPipelineBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	namespace {

		constexpr GraphicsPipelineBuilder::AttachmentReference InvalidAttachment{ VK_ATTACHMENT_UNUSED, VK_IMAGE_LAYOUT_UNDEFINED };

	} // anonymous namespace

	GraphicsPipelineBuilder::Attachment::Attachment(VkFormat format, VkSampleCountFlags quality, float32 widthScale, float32 heightScale, float32 depthScale) :
		format(format),
		flags(0u),
		usages(0u),
		firstPass(~0u),
		lastPass(0u),
		msaaQuality(quality),
		staticResolution(0u) {
		scale[0] = widthScale;
		scale[1] = heightScale;
		scale[2] = depthScale;
	}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::Attachment::Attachment(VkFormat format, VkSampleCountFlags quality, VkExtent3D dimensions) :
		format(format),
		flags(0u),
		usages(0u),
		firstPass(~0u),
		lastPass(0u),
		msaaQuality(quality),
		staticResolution(1u) {
		this->dimensions = dimensions;
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::Attachment::MarkUsed(uint32 pass, VkImageUsageFlags usage) {
		usages |= usage;

		firstPass = Min(pass, firstPass);
		lastPass  = Max(pass, lastPass);
	}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::Pass::Pass(const Utf8Char* const name) :
		inputAttachmentCount(0u),
		attachmentCount(0u),
		inputAttachments{ InvalidAttachment },
		attachments{ InvalidAttachment },
		depthStencilAttachment(InvalidAttachment) {
		CopyString(this->name, name);
	}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::GraphicsPipelineBuilder() :
		_attachments(MallocAllocator("Vulkan Graphics Pipeline Attachment List Allocator")),
		_passes(MallocAllocator("Vulkan Graphics Pipeline Stage List Allocator")) {}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::GraphicsPipelineBuilder(GraphicsPipelineBuilder&& builder) :
		GraphicsPipelineBuilder() {
		Swap(*this, builder);
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::Begin(VkPipelineBindPoint bindPoint, const Utf8Char* name) {
		_attachments.Clear();
		_passes.Clear();

		BeginPass(bindPoint, name);
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::BeginPass(VkPipelineBindPoint bindPoint, const Utf8Char* name) {
		_passes.EmplaceBack(name);
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::AttachInput(uint32 index, VkImageLayout layout) {
		enum : uint32 { MaxInputAttachments = _countof(Pass::inputAttachments) };

		Pass& pass(_passes.Back());
		if (pass.inputAttachmentCount == MaxInputAttachments || _attachments.GetSize() <= index) {
			return false;
		}

		_attachments[index].MarkUsed(uint32(_passes.GetSize() - 1u), VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
		pass.inputAttachments[pass.inputAttachmentCount++] = AttachmentReference{ index, layout };
		return true;
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::AttachColorOutput(uint32 index, VkImageLayout layout) {
		enum : uint32 { MaxAttachments = _countof(Pass::attachments) };

		Pass& pass(_passes.Back());
		if (pass.inputAttachmentCount == MaxAttachments || _attachments.GetSize() <= index) {
			return false;
		}

		_attachments[index].MarkUsed(uint32(_passes.GetSize() - 1u), VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
		pass.attachments[pass.attachmentCount++] = AttachmentReference{ index, layout };

		return true;
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::AttachDepthStencilBuffer(uint32 index, VkImageLayout layout) {
		Pass& pass(_passes.Back());
		if (_attachments.GetSize() <= index || pass.depthStencilAttachment.index != VK_ATTACHMENT_UNUSED) {
			return false;
		}

		_attachments[index].MarkUsed(uint32(_passes.GetSize() - 1u), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
		pass.depthStencilAttachment = AttachmentReference{ index, layout };

		return true;
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::Finish() {}

	// ---------------------------------------------------

	void Swap(GraphicsPipelineBuilder& lhs, GraphicsPipelineBuilder& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._attachments, rhs._attachments);
		Swap(lhs._passes, rhs._passes);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
