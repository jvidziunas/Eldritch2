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

	ETInlineHint ETForceInlineHint VkImageViewType GraphicsPipeline::AttachmentDescription::GetViewType() const ETNoexceptHint {
		if (_staticResolution) {
			_arraySize > 1u ? VK_IMAGE_VIEW_TYP
		}
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkImageType GraphicsPipeline::AttachmentDescription::GetType() const ETNoexceptHint {
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkExtent3D GraphicsPipeline::AttachmentDescription::GetResolution(VkExtent2D baseResolution) const ETNoexceptHint {
		return _staticResolution ? _resolution : VkExtent3D { uint32(baseResolution.width * _scale.width), uint32(baseResolution.height * _scale.height), 1u };
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 GraphicsPipeline::AttachmentDescription::GetLayers(uint32 layers) const ETNoexceptHint {
		return _arraySize;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const GraphicsPipeline::PassDescription* GraphicsPipeline::GetPasses() const ETNoexceptHint {
		return _passes.GetData();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 GraphicsPipeline::GetPassCount() const ETNoexceptHint {
		return uint32(_passes.GetSize());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const GraphicsPipeline::AttachmentDescription* GraphicsPipeline::GetFramebufferAttachments() const ETNoexceptHint {
		return _attachments.GetData();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 GraphicsPipeline::GetFramebufferAttachmentCount() const ETNoexceptHint {
		return uint32(_attachments.GetSize());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkDescriptorSetLayout GraphicsPipeline::GetDescriptorLayout() const ETNoexceptHint {
		return _descriptorLayout;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Framebuffer::Attachment::OwnsImage() const ETNoexceptHint {
		return image == VK_NULL_HANDLE;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const Framebuffer::Pass* Framebuffer::GetPasses() const ETNoexceptHint {
		return _passes.GetData();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 Framebuffer::GetPassCount() const ETNoexceptHint {
		return uint32(_passes.GetSize());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkQueryPool Framebuffer::GetTimingPool() const ETNoexceptHint {
		return _timingPool;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkResult Viewport::Resize(Gpu& gpu, VkExtent2D extent, uint32 layers, ExternalImageMap externalImages) {
		const GraphicsPipeline* const generator(_generator.load(std::memory_order_consume));
		if (generator == nullptr) {
			return VK_SUCCESS;
		}

		return this->BindResources(gpu, *generator, extent, layers, eastl::move(externalImages));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Viewport::TryAcquire(const GraphicsPipeline& generator) ETNoexceptHint {
		const GraphicsPipeline* expected(nullptr);
		return _generator.compare_exchange_strong(expected, ETAddressOf(generator), std::memory_order_acq_rel);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Viewport::Release() ETNoexceptHint {
		_generator.store(nullptr, std::memory_order_release);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint void ViewportDisposer::operator()(Viewport* viewport) const ETNoexceptHint {
		viewport->Release();
	}

}}} // namespace Eldritch2::Graphics::Vulkan
