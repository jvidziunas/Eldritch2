/*==================================================================*\
  GpuResourceApi.Framebuffer.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/PipelineBuilder.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	VkResult Framebuffer::Image::BindResources(Gpu& gpu, const PipelineAttachment& image, VkExtent2D dimensions, uint32 slices) {
		const VkImageCreateInfo imageInfo{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			image.resolution.GetDepth(/*base =*/1u) > 1u ? VK_IMAGE_TYPE_3D : VK_IMAGE_TYPE_2D,
			AsVkFormat(image.format),
			VkExtent3D{ image.resolution.GetWidth(dimensions.width), image.resolution.GetHeight(dimensions.height), image.resolution.GetDepth(/*base =*/1u) },
			image.smallestMip,
			image.slices,
			GetSampleCountFlags(image.samplesPerPixel),
			VK_IMAGE_TILING_OPTIMAL,
			/*usage =*/0u,
			VK_SHARING_MODE_EXCLUSIVE,
			/*queueFamilyIndexCount =*/0u,    // Exclusive ownership.
			/*pQueueFamilyIndices =*/nullptr, // Exclusive ownership.
			/*initialLayout =*/VK_IMAGE_LAYOUT_UNDEFINED
		};

		const VmaAllocationCreateInfo allocationInfo{
			VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, VMA_MEMORY_USAGE_GPU_ONLY,
			/*requiredFlags =*/0u,
			/*preferredFlags =*/0u,
			/*memoryTypeBits =*/0u,
			/*pool =*/NullVulkanHandle,
			/*pUserData =*/nullptr
		};

		return GpuImage::BindResources(gpu, imageInfo, allocationInfo);
	}

	// ---------------------------------------------------

	VkResult Framebuffer::Image::BindResources(Gpu& gpu, VkImage image) ETNoexceptHint {
		return GpuImage::BindResources(gpu, image, NullVulkanHandle);
	}

	// ---------------------------------------------------

	Framebuffer::StageImageSet::~StageImageSet() {}

	// ---------------------------------------------------

	VkResult Framebuffer::StageImageSet::BindResources(Gpu& gpu, const StageBuilder& stage, const PipelineAttachment attachments[], const Image images[]) {
		const auto CreateImageView([&](VkImageView& view, const StageAttachment& attachment) ETNoexceptHint -> VkResult {
			const VkImageViewType       viewType(attachments[attachment.globalIndex].resolution.GetDepth(/*base =*/1u) > 1u ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D);
			const auto                  format(AsVulkan(attachments[attachment.globalIndex].format));
			const VkImageViewCreateInfo createInfo{
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u,
				images[attachment.globalIndex],
				viewType,
				format.format,
				format.swizzle,
				VkImageSubresourceRange{
					VK_IMAGE_ASPECT_COLOR_BIT,
					attachment.mip,
					/*levelCount =*/1u, // Color attachment views must reference exactly one level in the target image.
					attachment.firstSlice,
					attachment.lastSlice - attachment.firstSlice }
			};

			return vkCreateImageView(gpu, ETAddressOf(createInfo), gpu.GetAllocationCallbacks(), ETAddressOf(view));
		});

		// ---

		using ::Eldritch2::Swap;

		VkImageView  views[ETCountOf(this->views)];
		VkImageView* viewsEnd(views);
		ET_FOREACH_AT_SCOPE_EXIT(view, views, vkDestroyImageView(gpu, view, gpu.GetAllocationCallbacks()));

		if (stage.depthAttachment.globalIndex != VK_ATTACHMENT_UNUSED) {
			ET_ABORT_UNLESS(CreateImageView(*viewsEnd++, stage.depthAttachment));
		}

		for (const StageAttachment& attachment : stage.inputAttachments) {
			if (attachment.globalIndex == VK_ATTACHMENT_UNUSED) {
				break;
			}

			ET_ABORT_UNLESS(CreateImageView(*viewsEnd++, attachment));
		}

		for (const StageAttachment& attachment : stage.colorAttachments) {
			if (attachment.globalIndex == VK_ATTACHMENT_UNUSED) {
				break;
			}

			ET_ABORT_UNLESS(CreateImageView(*viewsEnd++, attachment));
		}

		Swap(this->views, views);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Framebuffer::StageImageSet::FreeResources(Gpu& gpu) ETNoexceptHint {
		for (VkImageView& view : views) {
			vkDestroyImageView(gpu, Exchange(view, NullVulkanHandle), gpu.GetAllocationCallbacks());
		}
	}

	// ---------------------------------------------------

	Framebuffer::StageFramebuffer::StageFramebuffer(StageFramebuffer&& stage) ETNoexceptHint : StageFramebuffer() {
		Swap(*this, stage);
	}

	// ---------------------------------------------------

	Framebuffer::StageFramebuffer::StageFramebuffer() ETNoexceptHint : renderPass(NullVulkanHandle), framebuffer(NullVulkanHandle), granularity{ /*width =*/1u, /*height =*/1u } {}

	// ---------------------------------------------------

	Framebuffer::StageFramebuffer::~StageFramebuffer() {
		ETAssert(renderPass == NullVulkanHandle, "Leaking Vulkan render pass {}!", fmt::ptr(renderPass));
		ETAssert(framebuffer == NullVulkanHandle, "Leaking Vulkan framebuffer {}!", fmt::ptr(framebuffer));
	}

	// ---------------------------------------------------

	VkResult Framebuffer::StageFramebuffer::BindResources(Gpu& gpu, const StageBuilder& stage, VkExtent2D dimensions, uint32 layers, const StageImageSet& images) {
		using ::Eldritch2::Swap;

		ResolutionScaler scaler;
		scaler.ScaleBounds(stage.resolution, dimensions.width, dimensions.height);

		VkRenderPass                 renderPass;
		const VkRenderPassCreateInfo renderPassInfo{
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			/*pNext =*/nullptr,
		};

		VkFramebuffer                 framebuffer;
		const VkFramebufferCreateInfo framebufferInfo{
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

	void Framebuffer::StageFramebuffer::FreeResources(Gpu& gpu) ETNoexceptHint {
		vkDestroyFramebuffer(gpu, Exchange(framebuffer, NullVulkanHandle), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	Framebuffer::Framebuffer(Framebuffer&& framebuffer) ETNoexceptHint : Framebuffer() {
		Swap(*this, framebuffer);
	}

	// ---------------------------------------------------

	Framebuffer::Framebuffer() ETNoexceptHint : _timingPool(nullptr),
												_images(ImageList::AllocatorType("Framebuffer Image List Allocator")),
												_stageResources(StageResourceList::AllocatorType("Framebuffer Stage Resource List Allocator")) {}

	// ---------------------------------------------------

	Framebuffer::~Framebuffer() {
		ETAssert(_timingPool == nullptr, "Leaking Vulkan query pool {}!", fmt::ptr(_timingPool));
	}

	// ---------------------------------------------------

	VkResult Framebuffer::UpdateScaling(Gpu& gpu) ETNoexceptHint {
		const uint32 timingCount(uint32(_stageResources.GetSize()) * 2u);
		const auto   timings(ETStackAlloc(uint64, timingCount));
		const auto   stages(_stageResources.Begin<StageFramebuffer>());

		ET_ABORT_UNLESS(vkGetQueryPoolResults(gpu, _timingPool, 0u, timingCount, timingCount * sizeof(*timings), timings, sizeof(*timings), VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT));
		for (uint32 stage(0u); stage < _stageResources.GetSize(); ++stage) {
			ET_SUPPRESS_MSVC_WARNINGS(6385) // Static analyzer doesn't correctly determine read bounds.
			stages[stage].scaler.UpdateTime(MicrosecondTime(timings[2u * stage + 1u] - timings[2u * stage]));
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult Framebuffer::BindResources(Gpu& gpu, const PipelineBuilder& pipeline, VkExtent2D dimensions, uint32 slices, InitializerList<ExternalImage> externalImages) {
		using ::Eldritch2::Swap;

		const auto FindExternalImage([&](ImageList::SizeType index) ETNoexceptHint -> VkImage {
			const auto externalImage(FindIf(Begin(externalImages), End(externalImages), [=](const ExternalImage& image) ETNoexceptHint {
				return image.attachmentIndex == index;
			}));

			return externalImage != End(externalImages) ? externalImage->image : NullVulkanHandle;
		});

		// ---

		// Create the query pool that will be used to track draw call timing information for each stage in the pipeline.
		VkQueryPool                 timingPool;
		const VkQueryPoolCreateInfo timingPoolInfo{
			VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			VK_QUERY_TYPE_TIMESTAMP,
			/*queryCount =*/uint32(2u * pipeline.GetStages().GetSize()), // Start/stop timestamp for each stage.
			/*pipelineStatistics =*/0u                                   // Timestamp queries do not have flags.
		};
		ET_ABORT_UNLESS(vkCreateQueryPool(gpu, ETAddressOf(timingPoolInfo), gpu.GetAllocationCallbacks(), ETAddressOf(timingPool)));
		ET_AT_SCOPE_EXIT(vkDestroyQueryPool(gpu, timingPool, gpu.GetAllocationCallbacks()));

		// Allocate a private image or copy an external image reference for each pipeline attachment.
		ImageList::SizeType imageIndex(0u);
		ImageList           images(_images.GetAllocator(), pipeline.GetAttachments().GetSize());
		ET_AT_SCOPE_EXIT(images.ClearAndDispose([&](Image& image) ETNoexceptHint { image.FreeResources(gpu); }));
		for (PipelineBuilder::AttachmentList::ConstReference attachment : pipeline.GetAttachments()) {
			const VkImage externalImage(FindExternalImage(imageIndex++));
			if (ET_LIKELY(externalImage == NullVulkanHandle)) {
				ET_ABORT_UNLESS(images.EmplaceBack().BindResources(gpu, attachment, dimensions, slices));
			} else {
				ET_ABORT_UNLESS(images.EmplaceBack().BindResources(gpu, externalImage));
			}
		}

		// Create stage-specific state.
		StageResourceList stageResources(_stageResources.GetAllocator(), pipeline.GetStages().GetSize());
		ET_AT_SCOPE_EXIT(stageResources.ClearAndDispose([&](StageImageSet& images, StageFramebuffer& framebuffer) ETNoexceptHint {
			framebuffer.FreeResources(gpu);
			images.FreeResources(gpu);
		}));
		for (PipelineBuilder::StageList::ConstReference stage : pipeline.GetStages()) {
			auto resources(stageResources.EmplaceBack(StageImageSet(), StageFramebuffer()));
			ET_ABORT_UNLESS(Get<StageImageSet&>(resources).BindResources(gpu, stage, pipeline.GetAttachments().Begin(), images.GetData()));
			ET_ABORT_UNLESS(Get<StageFramebuffer&>(resources).BindResources(gpu, stage, dimensions, slices, Get<StageImageSet&>(resources)));
		}

		// Create shader bindings for any pipeline resources.
		DescriptorTable shaderResources;
		ET_ABORT_UNLESS(shaderResources.BindResources(gpu, /*layout =*/NullVulkanHandle, { VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, /*descriptorCount =*/512u } }));
		ET_AT_SCOPE_EXIT(shaderResources.FreeResources(gpu));

		Swap(_timingPool, timingPool);
		Swap(_images, images);
		Swap(_stageResources, stageResources);
		Swap(_shaderResources, shaderResources);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Framebuffer::FreeResources(Gpu& gpu) ETNoexceptHint {
		_shaderResources.FreeResources(gpu);
		_stageResources.ClearAndDispose([&](StageImageSet& images, StageFramebuffer& framebuffer) ETNoexceptHint {
			framebuffer.FreeResources(gpu);
			images.FreeResources(gpu);
		});
		_images.ClearAndDispose([&](Image& image) ETNoexceptHint {
			image.FreeResources(gpu);
		});
		vkDestroyQueryPool(gpu, Exchange(_timingPool, NullVulkanHandle), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	void Swap(Framebuffer::Image& lhs, Framebuffer::Image& rhs) ETNoexceptHint {
		Swap(static_cast<GpuImage&>(lhs), static_cast<GpuImage&>(rhs));
	}

	// ---------------------------------------------------

	void Swap(Framebuffer::StageImageSet& lhs, Framebuffer::StageImageSet& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs.views, rhs.views);
	}

	// ---------------------------------------------------

	void Swap(Framebuffer::StageFramebuffer& lhs, Framebuffer::StageFramebuffer& rhs) ETNoexceptHint {
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
		Swap(lhs._stageResources, rhs._stageResources);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
