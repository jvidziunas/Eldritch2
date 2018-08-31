/*==================================================================*\
  GraphicsPipeline.Framebuffer.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.hpp>
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

#define MAKE_CASE(is2D, isArray) (int(is2D) << 1u) | int(isArray)

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		ETInlineHint ETForceInlineHint ETPureFunctionHint VkImageViewType GetViewType(VkExtent3D extent, VkImageCreateFlags flags, bool isArray) ETNoexceptHint {
			if (flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) {
				return isArray ? VK_IMAGE_VIEW_TYPE_CUBE_ARRAY : VK_IMAGE_VIEW_TYPE_CUBE;
			}

			if (flags & VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT) {
				return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			}

			switch (MAKE_CASE(/*is2D =*/extent.height > 1, isArray)) {
			case MAKE_CASE(/*is2D =*/false, /*isArray =*/false): return VK_IMAGE_VIEW_TYPE_1D;
			case MAKE_CASE(/*is2D =*/false, /*isArray =*/true): return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
			case MAKE_CASE(/*is2D =*/true, /*isArray =*/false): return VK_IMAGE_VIEW_TYPE_2D;
			case MAKE_CASE(/*is2D =*/true, /*isArray =*/true):
				return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
				ET_NO_DEFAULT_CASE;
			} // switch (MAKE_CASE(is2D, isArray))
		}

	} // anonymous namespace

	Framebuffer::Attachment::Attachment(Attachment&& attachment) ETNoexceptHint : Attachment() {
		Swap(*this, attachment);
	}

	// ---------------------------------------------------

	Framebuffer::Attachment::Attachment() ETNoexceptHint : image(nullptr), view(nullptr) {}

	// ---------------------------------------------------

	Framebuffer::Attachment::~Attachment() {
		ET_ASSERT(image == nullptr, "Leaking Vulkan framebuffer attachment image!");
		ET_ASSERT(view == nullptr, "Leaking Vulkan framebuffer attachment image view!");
	}

	// ---------------------------------------------------

	VkResult Framebuffer::Attachment::BindResources(Gpu& gpu, const GraphicsPipeline& pipeline, uint32 attachmentIndex, VkExtent2D baseDimensions, uint32 layers) {
		using ::Eldritch2::Swap;

		const GraphicsPipeline::AttachmentDescription& attachment(pipeline.GetFramebufferAttachments()[attachmentIndex]);
		VkImage                                        image;
		const VkImageCreateInfo                        imageInfo(attachment.GetImageCreateInfo(baseDimensions, layers));
		ET_ABORT_UNLESS(vkCreateImage(gpu, ETAddressOf(imageInfo), gpu.GetAllocationCallbacks(), ETAddressOf(image)));
		ET_AT_SCOPE_EXIT(vkDestroyImage(gpu, image, gpu.GetAllocationCallbacks()));

		VkImageView                 view;
		const VkImageViewCreateInfo viewInfo(attachment.GetImageViewCreateInfo(image, /*mip =*/0u));
		ET_ABORT_UNLESS(vkCreateImageView(gpu, ETAddressOf(viewInfo), gpu.GetAllocationCallbacks(), ETAddressOf(view)));
		ET_AT_SCOPE_EXIT(vkDestroyImageView(gpu, view, gpu.GetAllocationCallbacks()));

		Swap(this->image, image);
		Swap(this->view, view);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult Framebuffer::Attachment::BindResources(Gpu& gpu, const GraphicsPipeline& pipeline, uint32 attachmentIndex, VkImage image) {
		using ::Eldritch2::Swap;

		const GraphicsPipeline::AttachmentDescription& attachment(pipeline.GetFramebufferAttachments()[attachmentIndex]);
		VkImageView                                    view;
		const VkImageViewCreateInfo                    viewInfo(attachment.GetImageViewCreateInfo(image, /*mip =*/0u));
		ET_ABORT_UNLESS(vkCreateImageView(gpu, ETAddressOf(viewInfo), gpu.GetAllocationCallbacks(), ETAddressOf(view)));
		ET_AT_SCOPE_EXIT(vkDestroyImageView(gpu, view, gpu.GetAllocationCallbacks()));

		image = VK_NULL_HANDLE;
		Swap(this->view, view);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Framebuffer::Attachment::FreeResources(Gpu& gpu) {
		vkDestroyImageView(gpu, view, gpu.GetAllocationCallbacks());
		vkDestroyImage(gpu, image, gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	Framebuffer::Pass::Pass(Pass&& pass) ETNoexceptHint : Pass() {
		Swap(*this, pass);
	}

	// ---------------------------------------------------

	Framebuffer::Pass::Pass() ETNoexceptHint : nativeFramebuffer(nullptr),
											   granularity { 0u, 0u },
											   nativeArea { 0u, 0u } {}

	// ---------------------------------------------------

	Framebuffer::Pass::~Pass() {
		ET_ASSERT(nativeFramebuffer == nullptr, "Leaking Vulkan framebuffer!");
	}

	// ---------------------------------------------------

	VkRect2D Framebuffer::Pass::GetRenderArea() const ETNoexceptHint {
		uint32 scaledWidth(nativeArea.extent.width);
		uint32 scaledHeight(nativeArea.extent.height);

		scaler.Scale(scaledWidth, scaledHeight);

		return VkRect2D { nativeArea.offset, VkExtent2D { scaledWidth, scaledHeight } };
	}

	// ---------------------------------------------------

	VkResult Framebuffer::Pass::BindResources(Gpu& gpu, const GraphicsPipeline& pipeline, const ArrayList<Attachment>& attachments, uint32 passIndex) {
		using ::Eldritch2::Swap;
		using PipelinePass = GraphicsPipeline::PassDescription;

		const PipelinePass& pass(pipeline.GetPasses()[passIndex]);
		VkImageView         views[PipelinePass::MaxTotalAttachments];
		uint32              totalViews(0u);

		for (uint32 attachment : pass.colorAttachments) {
			if (attachment == VK_ATTACHMENT_UNUSED) {
				continue;
			}

			views[totalViews++] = attachments[attachment].view;
		}

		for (uint32 attachment : pass.inputAttachments) {
			if (attachment == VK_ATTACHMENT_UNUSED) {
				continue;
			}

			views[totalViews++] = attachments[attachment].view;
		}

		if (pass.depthStencilAttachment != VK_ATTACHMENT_UNUSED) {
			views[totalViews++] = attachments[pass.depthStencilAttachment].view;
		}

		VkFramebuffer                 nativeFramebuffer;
		const VkFramebufferCreateInfo nativeFramebufferInfo {
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			pass.renderPass,
			totalViews,
			views,
			nativeArea.extent.width,
			nativeArea.extent.height,
			/*layers =*/1u
		};
		ET_ABORT_UNLESS(vkCreateFramebuffer(gpu, ETAddressOf(nativeFramebufferInfo), gpu.GetAllocationCallbacks(), ETAddressOf(nativeFramebuffer)));
		ET_AT_SCOPE_EXIT(vkDestroyFramebuffer(gpu, nativeFramebuffer, gpu.GetAllocationCallbacks()));

		Swap(this->nativeFramebuffer, nativeFramebuffer);
		vkGetRenderAreaGranularity(gpu, pass.renderPass, &granularity);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Framebuffer::Pass::FreeResources(Gpu& gpu) {
		vkDestroyFramebuffer(gpu, eastl::exchange(nativeFramebuffer, nullptr), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	Framebuffer::Framebuffer(Framebuffer&& framebuffer) :
		Framebuffer() {
		Swap(*this, framebuffer);
	}

	// ---------------------------------------------------

	Framebuffer::Framebuffer() :
		_attachments(MallocAllocator("Vulkan Framebuffer Attachment List Allocator")),
		_allocations(MallocAllocator("Vulkan Framebuffer Memory Allocation List Allocator")),
		_passes(MallocAllocator("Vulkan Framebuffer Pass Info List Allocator")),
		_timingPool(nullptr) {
	}

	// ---------------------------------------------------

	Framebuffer::~Framebuffer() {
		ET_ASSERT(_attachments.IsEmpty(), "Leaking Vulkan framebuffer attachments!");
		ET_ASSERT(_allocations.IsEmpty(), "Leaking Vulkan memory allocations!");
		ET_ASSERT(_passes.IsEmpty(), "Leaking Vulkan framebuffers!");
		ET_ASSERT(_timingPool == nullptr, "Leaking Vulkan query pool!");
	}

	// ---------------------------------------------------

	VkResult Framebuffer::UpdateDynamicScaling(Gpu& gpu) {
		const uint32  timingCount(uint32(_passes.GetSize()) * 2u);
		uint64* const timings(ETStackAlloc(uint64, timingCount));
		ET_ABORT_UNLESS(vkGetQueryPoolResults(gpu, _timingPool, 0u, timingCount, timingCount * sizeof(*timings), timings, sizeof(*timings), VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT));

		for (uint32 pass(0u); pass < _passes.GetSize(); ++pass) {
			// Static analyzer doesn't correctly determine read bounds.
			ET_SUPPRESS_MSVC_WARNINGS(6385)
			_passes[pass].scaler.UpdateTime(timings[2u * pass + 1u] - timings[2u * pass]);
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult Framebuffer::BindResources(Gpu& gpu, const GraphicsPipeline& pipeline, VkExtent2D dimensions, uint32 arrayLayers, ExternalImageMap externalImages) {
		using ::Eldritch2::Swap;

		VkQueryPool                 timingPool;
		const VkQueryPoolCreateInfo timingPoolInfo {
			VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			VK_QUERY_TYPE_TIMESTAMP,
			/*queryCount =*/uint32(2u * pipeline.GetPassCount()), // Start/stop timestamp for each pass.
			/*pipelineStatistics =*/0u                            // Timestamp queries do not have flags.
		};
		ET_ABORT_UNLESS(vkCreateQueryPool(gpu, ETAddressOf(timingPoolInfo), gpu.GetAllocationCallbacks(), ETAddressOf(timingPool)));
		ET_AT_SCOPE_EXIT(vkDestroyQueryPool(gpu, timingPool, gpu.GetAllocationCallbacks()));

		ArrayList<Attachment> combinedAttachments(_attachments.GetAllocator());
		combinedAttachments.Resize(pipeline.GetFramebufferAttachmentCount());
		ET_AT_SCOPE_EXIT(for (Attachment& attachment
							  : combinedAttachments) attachment.FreeResources(gpu));
		for (uint32 attachment(0u); attachment < pipeline.GetFramebufferAttachmentCount(); ++attachment) {
			ET_ABORT_UNLESS(combinedAttachments[attachment].BindResources(gpu, pipeline, attachment, dimensions, arrayLayers));
		}

		ArrayList<Pass> passes(_passes.GetAllocator());
		ET_AT_SCOPE_EXIT(for (Pass& pass
							  : passes) pass.FreeResources(gpu));
		passes.Resize(pipeline.GetPassCount());
		for (uint32 pass(0u); pass < pipeline.GetPassCount(); ++pass) {
			ET_ABORT_UNLESS(passes[pass].BindResources(gpu, pipeline, combinedAttachments, pass));
		}

		ArrayList<VmaAllocation> allocations(_allocations.GetAllocator());
		ET_AT_SCOPE_EXIT(for (VmaAllocation allocation
							  : allocations) vmaFreeMemory(gpu, allocation));

		Swap(_timingPool, timingPool);
		Swap(_attachments, combinedAttachments);
		Swap(_allocations, allocations);
		Swap(_passes, passes);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Framebuffer::FreeResources(Gpu& gpu) {
		for (Pass& pass : _passes) {
			pass.FreeResources(gpu);
		}

		for (Attachment& attachment : _attachments) {
			attachment.FreeResources(gpu);
		}

		for (VmaAllocation allocation : _allocations) {
			gpu.AddGarbage(allocation);
		}

		vkDestroyQueryPool(gpu, eastl::exchange(_timingPool, nullptr), gpu.GetAllocationCallbacks());

		_passes.Clear(ReleaseMemorySemantics());
		_attachments.Clear(ReleaseMemorySemantics());
		_allocations.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	void Swap(Framebuffer::Attachment& lhs, Framebuffer::Attachment& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs.image, rhs.image);
		Swap(lhs.view, rhs.view);
	}

	// ---------------------------------------------------

	void Swap(Framebuffer::Pass& lhs, Framebuffer::Pass& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs.scaler, rhs.scaler);
		Swap(lhs.nativeFramebuffer, rhs.nativeFramebuffer);
		Swap(lhs.granularity, rhs.granularity);
		Swap(lhs.nativeArea, rhs.nativeArea);
	}

	// ---------------------------------------------------

	void Swap(Framebuffer& lhs, Framebuffer& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._timingPool, rhs._timingPool);
		Swap(lhs._attachments, rhs._attachments);
		Swap(lhs._allocations, rhs._allocations);
		Swap(lhs._passes, rhs._passes);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
