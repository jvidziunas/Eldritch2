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

	ETConstexpr ETInlineHint ETForceInlineHint VkAttachmentStoreOp PipelineAttachmentDescription::GetStoreOp(uint32 thisPass) const ETNoexceptHint {
		return thisPass < lastRead ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint VkAttachmentLoadOp PipelineAttachmentDescription::GetLoadOp(uint32 thisPass) const ETNoexceptHint {
		return thisPass > firstWrite ? VK_ATTACHMENT_LOAD_OP_LOAD : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint bool PipelineAttachmentDescription::IsUsed() const ETNoexceptHint {
		return usages != 0u;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Range<const PipelineAttachmentDescription*> GraphicsPipelineBuilder::GetAttachments() const ETNoexceptHint {
		return { _attachments.Begin(), _attachments.End() };
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Range<const PipelineBufferDescription*> GraphicsPipelineBuilder::GetBuffers() const ETNoexceptHint {
		return { _buffers.Begin(), _buffers.End() };
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Range<const PipelinePassDescription*> GraphicsPipelineBuilder::GetPasses() const ETNoexceptHint {
		return { _passes.Begin(), _passes.End() };
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 GraphicsPipelineBuilder::DefineAttachment(VkFormat format, uint32 mips, bool isPersistent) {
		return _attachments.EmplaceBack(format, mips, isPersistent), uint32(_attachments.GetSize() - 1u);
	}

	// ---------------------------------------------------

	template <typename Pass, typename... AdditionalPasses>
	ETInlineHint ETForceInlineHint GraphicsPipelineBuilder& GraphicsPipelineBuilder::Optimize(Pass pass, AdditionalPasses... passes) {
		return pass(_attachments, _buffers, _passes), Optimize(eastl::move(passes)...);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint GraphicsPipelineBuilder& GraphicsPipelineBuilder::Optimize() {
		/* This space intentionally blank.*/
		return *this;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(const PipelineAttachmentReference& lhs, const PipelineAttachmentReference& rhs) {
		return lhs.globalIndex == rhs.globalIndex;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator!=(const PipelineAttachmentReference& lhs, const PipelineAttachmentReference& rhs) {
		return !(lhs == rhs);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
