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

	ETInlineHint const GraphicsPipeline::Pass& GraphicsPipeline::operator[](uint32 pass) const {
		return _passes[pass];
	}

	// ---------------------------------------------------

	ETInlineHint uint32 GraphicsPipeline::GetPassCount() const {
		return uint32(_passes.GetSize());
	}

	// ---------------------------------------------------

	ETInlineHint VkDescriptorSetLayout GraphicsPipeline::GetDescriptorLayout() const {
		return _descriptorLayout;
	}

	// ---------------------------------------------------

	ETInlineHint VkQueryPool GraphicsPipeline::GetTimingPool() const {
		return _timingPool;
	}

	// ---------------------------------------------------

	ETInlineHint VkFramebuffer Framebuffer::operator[](uint32_t pass) const {
		return _framebufferByPass[pass];
	}

}}} // namespace Eldritch2::Graphics::Vulkan
