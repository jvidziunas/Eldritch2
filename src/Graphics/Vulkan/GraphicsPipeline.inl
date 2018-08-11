/*==================================================================*\
  GraphicsPipeline.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint const GraphicsPipeline::PassDescription* GraphicsPipeline::GetPasses() const {
		return _passes.GetData();
	}

	// ---------------------------------------------------

	ETInlineHint uint32 GraphicsPipeline::GetPassCount() const {
		return uint32(_passes.GetSize());
	}

	// ---------------------------------------------------

	ETInlineHint const GraphicsPipeline::AttachmentDescription* GraphicsPipeline::GetFramebufferAttachments() const {
		return _framebufferAttachments.GetData();
	}

	// ---------------------------------------------------

	ETInlineHint uint32 GraphicsPipeline::GetFramebufferAttachmentCount() const {
		return uint32(_framebufferAttachments.GetSize());
	}

	// ---------------------------------------------------

	ETInlineHint VkDescriptorSetLayout GraphicsPipeline::GetDescriptorLayout() const {
		return _descriptorLayout;
	}

	// ---------------------------------------------------

	ETInlineHint const Framebuffer::Pass* Framebuffer::GetPasses() const {
		return _passes.GetData();
	}

	// ---------------------------------------------------

	ETInlineHint uint32 Framebuffer::GetPassCount() const {
		return uint32(_passes.GetSize());
	}

	// ---------------------------------------------------

	ETInlineHint VkQueryPool Framebuffer::GetTimingPool() const {
		return _timingPool;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
