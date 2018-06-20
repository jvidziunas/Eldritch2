/*==================================================================*\
  GraphicsPipelineBuilder.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint bool GraphicsPipelineBuilder::Attachment::IsReferenced() const {
		return firstPass <= lastPass;
	}

	// ---------------------------------------------------

	ETInlineHint bool GraphicsPipelineBuilder::Attachment::ShouldPreserveInPass(uint32 pass) const {
		return firstPass < pass && pass < lastPass;
	}

	// ---------------------------------------------------

	ETInlineHint const GraphicsPipelineBuilder::Pass& GraphicsPipelineBuilder::operator[](uint32 pass) const {
		return _passes[pass];
	}

	// ---------------------------------------------------

	ETInlineHint uint32 GraphicsPipelineBuilder::GetPassCount() const {
		return uint32(_passes.GetSize());
	}

	// ---------------------------------------------------

	ETInlineHint const GraphicsPipelineBuilder::Attachment& GraphicsPipelineBuilder::GetAttachment(uint32 attachment) const {
		return _attachments[attachment];
	}

	// ---------------------------------------------------

	ETInlineHint uint32 GraphicsPipelineBuilder::GetAttachmentCount() const {
		return uint32(_attachments.GetSize());
	}

	// ---------------------------------------------------

	template <typename... Args>
	ETInlineHint void GraphicsPipelineBuilder::DefineAttachment(VkFormat format, VkSampleCountFlags quality, VkImageLayout initial, VkImageLayout final, Args&&... args) {
		const VkAttachmentLoadOp loadOperation(initial != VK_IMAGE_LAYOUT_UNDEFINED ? VK_ATTACHMENT_LOAD_OP_LOAD : VK_ATTACHMENT_LOAD_OP_DONT_CARE);

		_attachments.EmplaceBack(format, quality, eastl::forward<Args>(args)...);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
