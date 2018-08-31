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

	ETInlineHint ETForceInlineHint bool GraphicsPipelineBuilder::AttachmentDescription::SupportsResolution(GraphicsPassScale desired) const ETNoexceptHint {
		return !IsUsed() || (staticResolution == 0 && OrderBuffers(ETAddressOf(scale), ETAddressOf(desired), sizeof(desired)) == 0);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool GraphicsPipelineBuilder::AttachmentDescription::SupportsResolution(VkExtent3D desired) const ETNoexceptHint {
		return !IsUsed() || (staticResolution != 0 && OrderBuffers(ETAddressOf(resolution), ETAddressOf(desired), sizeof(desired)) == 0);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool GraphicsPipelineBuilder::AttachmentDescription::ShouldPreserve(uint32 pass) const ETNoexceptHint {
		return isPersistent || (firstWrite < pass && pass < lastRead);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool GraphicsPipelineBuilder::AttachmentDescription::IsWritten() const ETNoexceptHint {
		return firstWrite <= lastWrite;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool GraphicsPipelineBuilder::AttachmentDescription::IsRead() const ETNoexceptHint {
		return firstRead <= lastRead;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool GraphicsPipelineBuilder::AttachmentDescription::IsUsed() const ETNoexceptHint {
		return IsWritten() | IsRead();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const GraphicsPipelineBuilder::PassDescription& GraphicsPipelineBuilder::operator[](uint32 pass) const ETNoexceptHint {
		return _passes[pass];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 GraphicsPipelineBuilder::GetPassCount() const ETNoexceptHint {
		return uint32(_passes.GetSize());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const GraphicsPipelineBuilder::AttachmentDescription& GraphicsPipelineBuilder::GetAttachment(uint32 attachment) const ETNoexceptHint {
		return _attachments[attachment];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 GraphicsPipelineBuilder::GetAttachmentCount() const ETNoexceptHint {
		return uint32(_attachments.GetSize());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 GraphicsPipelineBuilder::DefineAttachment(VkFormat format) {
		_attachments.EmplaceBack(format);
		return uint32(_attachments.GetSize() - 1u);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(const GraphicsPipelineBuilder::AttachmentReference& lhs, const GraphicsPipelineBuilder::AttachmentReference& rhs) {
		return lhs.globalIndex == rhs.globalIndex;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator!=(const GraphicsPipelineBuilder::AttachmentReference& lhs, const GraphicsPipelineBuilder::AttachmentReference& rhs) {
		return !(lhs == rhs);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
