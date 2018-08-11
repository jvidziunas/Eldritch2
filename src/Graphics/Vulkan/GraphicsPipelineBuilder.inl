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

	ETInlineHint bool GraphicsPipelineBuilder::AttachmentDescription::SupportsResolution(float32 widthScale, float32 heightScale) const {
		return staticDimensions == 0 && (scales[0] == widthScale && scales[1] == heightScale);
	}

	// ---------------------------------------------------

	ETInlineHint bool GraphicsPipelineBuilder::AttachmentDescription::SupportsResolution(VkExtent3D dispatchDimensions) const {
		return staticDimensions != 0 && OrderBuffers(&dimensions, &dispatchDimensions, sizeof(VkExtent3D)) == 0;
	}

	// ---------------------------------------------------

	ETInlineHint bool GraphicsPipelineBuilder::AttachmentDescription::IsReferenced() const {
		return IsWritten() | IsRead();
	}

	// ---------------------------------------------------

	ETInlineHint bool GraphicsPipelineBuilder::AttachmentDescription::IsWritten() const {
		return firstWrite <= lastWrite;
	}

	// ---------------------------------------------------

	ETInlineHint bool GraphicsPipelineBuilder::AttachmentDescription::IsRead() const {
		return firstRead <= lastRead;
	}

	// ---------------------------------------------------

	ETInlineHint bool GraphicsPipelineBuilder::AttachmentDescription::ShouldPreserveInPass(uint32 pass) const {
		return isPersistent || (firstWrite < pass && pass < lastRead);
	}

	// ---------------------------------------------------

	ETInlineHint const GraphicsPipelineBuilder::PassDescription& GraphicsPipelineBuilder::operator[](uint32 pass) const {
		return _passes[pass];
	}

	// ---------------------------------------------------

	ETInlineHint uint32 GraphicsPipelineBuilder::GetPassCount() const {
		return uint32(_passes.GetSize());
	}

	// ---------------------------------------------------

	ETInlineHint const GraphicsPipelineBuilder::AttachmentDescription& GraphicsPipelineBuilder::GetAttachment(uint32 attachment) const {
		return _attachments[attachment];
	}

	// ---------------------------------------------------

	ETInlineHint uint32 GraphicsPipelineBuilder::GetAttachmentCount() const {
		return uint32(_attachments.GetSize());
	}

	// ---------------------------------------------------

	template <typename... Args, class /*SFINAE*/>
	ETInlineHint uint32 GraphicsPipelineBuilder::DefineAttachment(VkFormat format, VkSampleCountFlags quality, Args&&... args) {
		_attachments.EmplaceBack(format, quality, eastl::forward<Args>(args)...);

		return uint32(_attachments.GetSize() - 1u);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
