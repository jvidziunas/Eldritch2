/*==================================================================*\
  GpuResourceApi.Framebuffer.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.hpp>
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	Framebuffer::Image::Image(Image&& image) ETNoexceptHint : Image() {
		Swap(*this, image);
	}

	// ---------------------------------------------------

	Framebuffer::Image::Image() ETNoexceptHint : _image(nullptr), _backing(nullptr), _ownsImage(false) {}

	// ---------------------------------------------------

	Framebuffer::Image::~Image() {
		ET_ASSERT(_image == nullptr, "Leaking Vulkan framebuffer image {}!", fmt::ptr(_image));
		ET_ASSERT(_backing == nullptr, "Leaking Vulkan framebuffer allocation {}!", fmt::ptr(_backing));
	}

	// ---------------------------------------------------

	VkResult Framebuffer::Image::BindResources(Gpu& gpu, const PipelineAttachmentDescription& description, VkExtent2D baseDimensions, uint32 baseLayers) {
		using ::Eldritch2::Swap;

		VkImage                 image;
		const VkImageCreateInfo imageInfo {
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			/*pNext =*/nullptr,
			description.flags,
			description.resolution.GetMaxDepth(/*base =*/1u) > 1u ? VK_IMAGE_TYPE_3D : VK_IMAGE_TYPE_2D,
			description.format,
			{ description.resolution.GetMaxWidth(baseDimensions.width), description.resolution.GetMaxHeight(baseDimensions.height), description.resolution.GetMaxDepth(/*base =*/1u) },
			description.mipLevels,
			description.layers,
			GetSampleCountFlags(description.samplesPerPixel),
			VK_IMAGE_TILING_OPTIMAL,
			description.usages,
			VK_SHARING_MODE_EXCLUSIVE,
			/*queueFamilyIndexCount =*/0u,
			/*pQueueFamilyIndices =*/nullptr,
			/*initialLayout =*/VK_IMAGE_LAYOUT_UNDEFINED
		};
		ET_ABORT_UNLESS(vkCreateImage(gpu, ETAddressOf(imageInfo), gpu.GetAllocationCallbacks(), ETAddressOf(image)));
		ET_AT_SCOPE_EXIT(vkDestroyImage(gpu, image, gpu.GetAllocationCallbacks()));

		if (eastl::exchange(_ownsImage, true)) {
			gpu.AddGarbage(eastl::exchange(_image, image), eastl::exchange(_backing, nullptr));
		} else {
			Swap(_image, image);
			_backing = nullptr;
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult Framebuffer::Image::BindResources(Gpu& gpu, const PipelineAttachmentDescription& /*description*/, VkImage externalImage) {
		using ::Eldritch2::Swap;

		if (eastl::exchange(_ownsImage, false)) {
			gpu.AddGarbage(eastl::exchange(_image, externalImage), eastl::exchange(_backing, nullptr));
		} else {
			Swap(_image, externalImage);
			_backing = nullptr;
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Framebuffer::Image::FreeResources(Gpu& gpu) {
		if (eastl::exchange(_ownsImage, false)) {
			gpu.AddGarbage(eastl::exchange(_image, nullptr), eastl::exchange(_backing, nullptr));
		} else {
			_image   = nullptr;
			_backing = nullptr;
		}
	}

	// ---------------------------------------------------

	VkResult Framebuffer::PassImages::BindResources(Gpu& gpu, const PipelinePassDescription& pass, const PipelineAttachmentDescription descriptions[], const Image images[]) {
		using ::Eldritch2::Swap;

		const auto AsImageView([&](const PipelineAttachmentReference& attachment) ETNoexceptHint {
			return VkImageViewCreateInfo {
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				/*pNext =*/nullptr,
				descriptions[attachment.globalIndex].viewFlags,
				images[attachment.globalIndex],
				descriptions[attachment.globalIndex].resolution.GetMaxDepth(/*base =*/1u) > 1u ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D,
				descriptions[attachment.globalIndex].format,
				{ VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY },
				VkImageSubresourceRange {}
			};
		});

		VkImageView  views[ETCountOf(this->views)];
		VkImageView* viewsEnd(views);
		ET_AT_SCOPE_EXIT(for (VkImageView view
							  : views) vkDestroyImageView(gpu, view, gpu.GetAllocationCallbacks()));
		for (const PipelineAttachmentReference& output : pass.framebuffer.colorAttachments) {
			if (output.globalIndex == VK_ATTACHMENT_UNUSED) {
				break;
			}

			const VkImageViewCreateInfo viewInfo(AsImageView(output));
			ET_ABORT_UNLESS(vkCreateImageView(gpu, ETAddressOf(viewInfo), gpu.GetAllocationCallbacks(), viewsEnd++));
		}

		for (const PipelineAttachmentReference& input : pass.framebuffer.inputAttachments) {
			if (input.globalIndex == VK_ATTACHMENT_UNUSED) {
				break;
			}

			const VkImageViewCreateInfo viewInfo(AsImageView(input));
			ET_ABORT_UNLESS(vkCreateImageView(gpu, ETAddressOf(viewInfo), gpu.GetAllocationCallbacks(), viewsEnd++));
		}

		if (pass.framebuffer.depthAttachment.globalIndex != VK_ATTACHMENT_UNUSED) {
			const VkImageViewCreateInfo viewInfo(AsImageView(pass.framebuffer.depthAttachment));
			ET_ABORT_UNLESS(vkCreateImageView(gpu, ETAddressOf(viewInfo), gpu.GetAllocationCallbacks(), viewsEnd++));
		}

		Swap(this->views, views);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Framebuffer::PassImages::FreeResources(Gpu& gpu) {
		for (VkImageView& view : views) {
			vkDestroyImageView(gpu, eastl::exchange(view, nullptr), gpu.GetAllocationCallbacks());
		}
	}

	// ---------------------------------------------------

	Framebuffer::Pass::Pass(Pass&& pass) ETNoexceptHint : Pass() {
		Swap(*this, pass);
	}

	// ---------------------------------------------------

	Framebuffer::Pass::Pass() ETNoexceptHint : renderPass(nullptr), framebuffer(nullptr), granularity { /*width =*/1u, /*height =*/1u } {}

	// ---------------------------------------------------

	Framebuffer::Pass::~Pass() {
		ET_ASSERT(renderPass == nullptr, "Leaking Vulkan render pass {}!", fmt::ptr(renderPass));
		ET_ASSERT(framebuffer == nullptr, "Leaking Vulkan framebuffer {}!", fmt::ptr(framebuffer));
	}

	// ---------------------------------------------------

	VkResult Framebuffer::Pass::BindResources(Gpu& gpu, const PipelinePassDescription& pass, VkExtent2D dimensions, uint32 layers, const PassImages& images) {
		using ::Eldritch2::Swap;

		ResolutionScaler scaler;
		scaler.SetBounds(pass.framebuffer.resolution, dimensions.width, dimensions.height);

		VkRenderPass                 renderPass;
		const VkRenderPassCreateInfo renderPassInfo {
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			/*pNext =*/nullptr,
		};

		VkFramebuffer                 framebuffer;
		const VkFramebufferCreateInfo framebufferInfo {
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			renderPass,
			uint32(eastl::distance(images.views, End(images.views))),
			images.views,
			scaler.GetMaxWidth(),
			scaler.GetMaxHeight(),
			/*layers =*/layers
		};
		ET_ABORT_UNLESS(vkCreateFramebuffer(gpu, ETAddressOf(framebufferInfo), gpu.GetAllocationCallbacks(), ETAddressOf(framebuffer)));
		ET_AT_SCOPE_EXIT(vkDestroyFramebuffer(gpu, framebuffer, gpu.GetAllocationCallbacks()));

		Swap(this->renderPass, renderPass);
		Swap(this->framebuffer, framebuffer);
		Swap(this->scaler, scaler);
		vkGetRenderAreaGranularity(gpu, renderPass, ETAddressOf(granularity));

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Framebuffer::Pass::FreeResources(Gpu& gpu) {
		vkDestroyFramebuffer(gpu, eastl::exchange(framebuffer, nullptr), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	Framebuffer::Framebuffer(Framebuffer&& framebuffer) :
		Framebuffer() {
		Swap(*this, framebuffer);
	}

	// ---------------------------------------------------

	Framebuffer::Framebuffer() :
		_images(MallocAllocator("Vulkan Framebuffer Attachment List Allocator")),
		_passes(MallocAllocator("Vulkan Framebuffer Pass Info List Allocator")),
		_timingPool(nullptr) {
	}

	// ---------------------------------------------------

	Framebuffer::~Framebuffer() {
		ET_ASSERT(_timingPool == nullptr, "Leaking Vulkan query pool!");
	}

	// ---------------------------------------------------

	VkResult Framebuffer::UpdateScaling(Gpu& gpu) {
		const uint32  timingCount(uint32(_passes.GetSize()) * 2u);
		const auto    passes(_passes.Begin<Pass>());
		uint64* const timings(ETStackAlloc(uint64, timingCount));
		ET_ABORT_UNLESS(vkGetQueryPoolResults(gpu, _timingPool, 0u, timingCount, timingCount * sizeof(*timings), timings, sizeof(*timings), VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT));

		for (uint32 pass(0u); pass < _passes.GetSize(); ++pass) {
			ET_SUPPRESS_MSVC_WARNINGS(6385) // Static analyzer doesn't correctly determine read bounds.
			passes[pass].scaler.UpdateTime(timings[2u * pass + 1u] - timings[2u * pass]);
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult Framebuffer::BindResources(Gpu& gpu, const GraphicsPipelineBuilder& pipeline, VkExtent2D baseDimensions, uint32 baseLayers, ExternalImageMap externalImages) {
		using ::Eldritch2::Swap;

		VkQueryPool                 timingPool;
		const VkQueryPoolCreateInfo timingPoolInfo {
			VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			VK_QUERY_TYPE_TIMESTAMP,
			/*queryCount =*/uint32(2u * pipeline.GetPasses().GetSize()), // Start/stop timestamp for each pass.
			/*pipelineStatistics =*/0u                                   // Timestamp queries do not have flags.
		};
		ET_ABORT_UNLESS(vkCreateQueryPool(gpu, ETAddressOf(timingPoolInfo), gpu.GetAllocationCallbacks(), ETAddressOf(timingPool)));
		ET_AT_SCOPE_EXIT(vkDestroyQueryPool(gpu, timingPool, gpu.GetAllocationCallbacks()));

		ArrayList<Image> images(_images.GetAllocator(), pipeline.GetAttachments().GetSize());
		uint32           imageIndex(0u);
		ET_AT_SCOPE_EXIT(for (Image& image
							  : images) image.FreeResources(gpu));
		for (const PipelineAttachmentDescription& description : pipeline.GetAttachments()) {
			Image&     image(images.EmplaceBack());
			const auto candidate(externalImages.Find(imageIndex++));
			ET_ABORT_UNLESS(ET_LIKELY(candidate == externalImages.End()) ? image.BindResources(gpu, description, baseDimensions, baseLayers) : image.BindResources(gpu, description, candidate->second));
		}

		SoArrayList<PassImages, Pass> passes(_passes.GetAllocator(), pipeline.GetPasses().GetSize());
		ET_AT_SCOPE_EXIT(for (auto pass
							  : passes) {
			eastl::get<Pass&>(pass).FreeResources(gpu);
			eastl::get<PassImages&>(pass).FreeResources(gpu);
		});
		for (const PipelinePassDescription& source : pipeline.GetPasses()) {
			PassImages passImages;
			Pass       pass;

			ET_ABORT_UNLESS(passImages.BindResources(gpu, source, pipeline.GetAttachments().Begin(), images.GetData()));
			ET_ABORT_UNLESS(pass.BindResources(gpu, source, baseDimensions, baseLayers, passImages));
			passes.EmplaceBack(eastl::move(passImages), eastl::move(pass));
		}

		Swap(_timingPool, timingPool);
		Swap(_images, images);
		Swap(_passes, passes);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Framebuffer::FreeResources(Gpu& gpu) {
		for (auto pass : _passes) {
			eastl::get<Pass&>(pass).FreeResources(gpu);
			eastl::get<PassImages&>(pass).FreeResources(gpu);
		}

		for (Image& attachment : _images) {
			attachment.FreeResources(gpu);
		}

		_passes.Clear();
		_images.Clear(ReleaseMemorySemantics());
		vkDestroyQueryPool(gpu, eastl::exchange(_timingPool, nullptr), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	void Swap(Framebuffer::Image& lhs, Framebuffer::Image& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._image, rhs._image);
		Swap(lhs._backing, rhs._backing);
		Swap(lhs._ownsImage, rhs._ownsImage);
	}

	// ---------------------------------------------------

	void Swap(Framebuffer::PassImages& lhs, Framebuffer::PassImages& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs.views, rhs.views);
	}

	// ---------------------------------------------------

	void Swap(Framebuffer::Pass& lhs, Framebuffer::Pass& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs.renderPass, rhs.renderPass);
		Swap(lhs.framebuffer, rhs.framebuffer);
		Swap(lhs.scaler, rhs.scaler);
		Swap(lhs.granularity, rhs.granularity);
	}

	// ---------------------------------------------------

	void Swap(Framebuffer& lhs, Framebuffer& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._timingPool, rhs._timingPool);
		Swap(lhs._images, rhs._images);
		Swap(lhs._passes, rhs._passes);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
