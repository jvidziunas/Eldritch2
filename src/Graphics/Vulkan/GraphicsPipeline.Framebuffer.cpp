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

#define MAKE_CASE(is2D, isArray) (static_cast<int>(is2D) << 1u) | static_cast<int>(isArray)

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
	namespace {

		ETInlineHint ETPureFunctionHint VkImageViewType GetAttachmentViewType(VkExtent3D extent, VkImageCreateFlags createFlags, bool isArray) {
			if (createFlags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) {
				return isArray ? VK_IMAGE_VIEW_TYPE_CUBE_ARRAY : VK_IMAGE_VIEW_TYPE_CUBE;
			}

			if (createFlags & VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT) {
				return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			}

			switch (MAKE_CASE(/*is2D =*/extent.height > 1, isArray)) { // clang-format off
			case MAKE_CASE(/*is2D =*/false, /*isArray =*/false): return VK_IMAGE_VIEW_TYPE_1D;
			case MAKE_CASE(/*is2D =*/false, /*isArray =*/true): return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
			case MAKE_CASE(/*is2D =*/true, /*isArray =*/false): return VK_IMAGE_VIEW_TYPE_2D;
			case MAKE_CASE(/*is2D =*/true, /*isArray =*/true): return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			ET_NO_DEFAULT_CASE;
			} // switch (MAKE_CASE(is2D, isArray)) // clang-format on										
		}

	// ---------------------------------------------------

		ETInlineHint VkResult BindAttachments(Gpu& gpu, ArrayList<Framebuffer::Attachment>& attachments, const GraphicsPipelineBuilder& pipeline, VkExtent2D baseDimensions, uint32 arrayLayers) {
			attachments.Resize(pipeline.GetAttachmentCount());

			for (uint32 index(0u); index < pipeline.GetAttachmentCount(); ++index) {
				const GraphicsPipelineBuilder::Attachment& attachment(pipeline.GetAttachment(index));
				const VkExtent3D                           dimensions{};

				if (!attachment.IsReferenced()) {
					continue;
				}

				ET_FAIL_UNLESS(attachments[index].BindResources(gpu, attachment.format, attachment.flags, VkSampleCountFlags(attachment.msaaQuality), attachment.usages, dimensions, arrayLayers ));
			}

			return VK_SUCCESS;
		}

	// ---------------------------------------------------
		
		ETInlineHint VkResult BindFramebuffers(Gpu &gpu, ArrayList<VkFramebuffer>& framebufferByPass, const ArrayList<Framebuffer::Attachment>& combinedAttachments, const GraphicsPipeline& pipeline, const GraphicsPipelineBuilder& source, VkExtent2D baseDimensions, uint32 arrayLayers) {
			enum : uint32 { MaxBindingsPerPass = _countof(GraphicsPipelineBuilder::Pass::attachments) + _countof(GraphicsPipelineBuilder::Pass::inputAttachments) + 1u };
			framebufferByPass.Resize(source.GetPassCount());

			for (uint32 passId(0u); passId < source.GetPassCount(); ++passId) {
				const GraphicsPipelineBuilder::Pass& pass(source[passId]);
				VkImageView                          attachments[MaxBindingsPerPass] = { VK_NULL_HANDLE };
				
				for (uint32 i(0u); i < pass.attachmentCount; ++i) {
					attachments[i] = combinedAttachments[pass.attachments[i].index].view;
				}

				if (pass.depthStencilAttachment.index != VK_ATTACHMENT_UNUSED) {
					attachments[_countof(attachments) - 1u] = combinedAttachments[pass.depthStencilAttachment.index].view;
				}

				VkFramebuffer                 framebuffer;
				const VkFramebufferCreateInfo framebufferInfo{
					VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
					/*pNext =*/nullptr,
					/*flags =*/0u,
					pipeline[passId].renderPass,
					pass.attachmentCount,
					attachments,
					baseDimensions.width,
					baseDimensions.height,
					arrayLayers
				};
				ET_FAIL_UNLESS(vkCreateFramebuffer(gpu, &framebufferInfo, gpu.GetAllocationCallbacks(), &framebufferByPass[passId]));
			}

			return VK_SUCCESS;
		}

	} // anonymous namespace

	Framebuffer::Attachment::Attachment(Attachment&& attachment) :
		Attachment() {
		Swap(*this, attachment);
	}

	// ---------------------------------------------------

	Framebuffer::Attachment::Attachment() :
		image(nullptr),
		view(nullptr) {}

	// ---------------------------------------------------

	Framebuffer::Attachment::~Attachment() {
		ET_ASSERT(image == nullptr, "Leaking Vulkan framebuffer attachment image!");
		ET_ASSERT(view == nullptr, "Leaking Vulkan framebuffer attachment image view!");
	}

	// ---------------------------------------------------

	VkResult Framebuffer::Attachment::BindResources(Gpu& gpu, VkFormat format, VkImageCreateFlags flags, VkImageUsageFlags usage, VkSampleCountFlags msaaFlags, VkExtent3D extent, uint32_t arrayLayers) {
		using ::Eldritch2::Swap;

		VkImage                 image;
		const VkImageCreateInfo imageInfo{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			/*pNext =*/nullptr,
			flags,
			GetImageType(extent),
			format,
			extent,
			/*mipLevels =*/1u,
			GetImageType(extent) != VK_IMAGE_TYPE_3D ? arrayLayers : 1u,
			VkSampleCountFlagBits(msaaFlags),
			VK_IMAGE_TILING_OPTIMAL,
			usage,
			VK_SHARING_MODE_EXCLUSIVE,
			/*queueFamilyIndexCount =*/0u,
			/*pQueueFamilyIndices =*/nullptr,
			VK_IMAGE_LAYOUT_UNDEFINED
		};
		ET_FAIL_UNLESS(vkCreateImage(gpu, &imageInfo, gpu.GetAllocationCallbacks(), &image));
		ET_AT_SCOPE_EXIT(vkDestroyImage(gpu, image, gpu.GetAllocationCallbacks()));

		VkImageView                 view;
		const VkImageViewCreateInfo viewInfo{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			image,
			GetAttachmentViewType(extent, flags, arrayLayers > 1u),
			format,
			MakeIdentitySwizzle(),
			VkImageSubresourceRange{
				IsDepthStencilFormat(format) ? VkImageAspectFlags(VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT) : VK_IMAGE_ASPECT_COLOR_BIT,
				/*baseMipLevel =*/0u,
				/*levelCount =*/1u, // Vulkan requires all image views bound as output attachments have exactly 1 MIP level.
				/*baseArrayLayer =*/0u,
				VK_REMAINING_ARRAY_LAYERS }
		};
		ET_FAIL_UNLESS(vkCreateImageView(gpu, &viewInfo, gpu.GetAllocationCallbacks(), &view));
		ET_AT_SCOPE_EXIT(vkDestroyImageView(gpu, view, gpu.GetAllocationCallbacks()));

		Swap(this->image, image);
		Swap(this->view, view);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult Framebuffer::Attachment::BindResources(Gpu& gpu, VkFormat format, VkImageViewCreateFlags flags, VkImage image, VkImageViewType type) {
		using ::Eldritch2::Swap;

		VkImage image(nullptr);
		ET_AT_SCOPE_EXIT(vkDestroyImage(gpu, image, gpu.GetAllocationCallbacks()));

		VkImageView view;
		const VkImageViewCreateInfo viewInfo{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			image,
			type,
			format,
			MakeIdentitySwizzle(),
			VkImageSubresourceRange{
				IsDepthStencilFormat(format) ? VkImageAspectFlags(VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT) : VK_IMAGE_ASPECT_COLOR_BIT,
				/*baseMipLevel =*/0u,
				/*levelCount =*/1u,
				/*baseArrayLayer =*/0u,
				VK_REMAINING_ARRAY_LAYERS }
		};
		ET_FAIL_UNLESS(vkCreateImageView(gpu, &viewInfo, gpu.GetAllocationCallbacks(), &view));
		ET_AT_SCOPE_EXIT(vkDestroyImageView(gpu, view, gpu.GetAllocationCallbacks()));

		Swap(this->image, image);
		Swap(this->view, view);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Framebuffer::Attachment::FreeResources(Gpu& gpu) {
		vkDestroyImageView(gpu, view, gpu.GetAllocationCallbacks());
		vkDestroyImage(gpu, image, gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	Framebuffer::Framebuffer(Framebuffer&& framebuffer) :
		Framebuffer() {
		Swap(*this, framebuffer);
	}

	// ---------------------------------------------------

	Framebuffer::Framebuffer() :
		_combinedAttachments(MallocAllocator("Vulkan Framebuffer Attachment List Allocator")),
		_allocations(MallocAllocator("Vulkan Framebuffer Memory Allocation List Allocator")),
		_framebufferByPass(MallocAllocator("Vulkan Framebuffer By Pass List Allocator")) {
	}

	// ---------------------------------------------------

	Framebuffer::~Framebuffer() {
		ET_ASSERT(_combinedAttachments.IsEmpty(), "Leaking Vulkan framebuffer attachments!");
		ET_ASSERT(_allocations.IsEmpty(), "Leaking Vulkan memory allocations!");
		ET_ASSERT(_framebufferByPass.IsEmpty(), "Leaking Vulkan framebuffers!");
	}

	// ---------------------------------------------------

	VkResult Framebuffer::BindResources(Gpu& gpu, const GraphicsPipeline& pipeline, const GraphicsPipelineBuilder& pipelineSource, VkExtent2D baseDimensions, uint32 arrayLayers) {
		using ::Eldritch2::Swap;

		ArrayList<Attachment> combinedAttachments(_combinedAttachments.GetAllocator());
		ET_AT_SCOPE_EXIT(for (Attachment& attachment : combinedAttachments) attachment.FreeResources(gpu));
		ET_FAIL_UNLESS(BindAttachments(gpu, combinedAttachments, pipelineSource, baseDimensions, arrayLayers));

		ArrayList<VkFramebuffer> framebufferByPass(_framebufferByPass.GetAllocator());
		ET_AT_SCOPE_EXIT(for (VkFramebuffer framebuffer : framebufferByPass) vkDestroyFramebuffer(gpu, framebuffer, gpu.GetAllocationCallbacks()));
		ET_FAIL_UNLESS(BindFramebuffers(gpu, framebufferByPass, combinedAttachments, pipeline, pipelineSource, baseDimensions, arrayLayers));
		
		ArrayList<VmaAllocation> allocations(_allocations.GetAllocator());
		ET_AT_SCOPE_EXIT(for (VmaAllocation allocation : allocations) vmaFreeMemory(gpu, allocation));

		Swap(_combinedAttachments, combinedAttachments);
		Swap(_allocations, allocations);
		Swap(_framebufferByPass, framebufferByPass);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Framebuffer::FreeResources(Gpu& gpu) {
		for (VkFramebuffer framebuffer : _framebufferByPass) {
			vkDestroyFramebuffer(gpu, framebuffer, gpu.GetAllocationCallbacks());
		}

		for (Attachment& attachment : _combinedAttachments) {
			attachment.FreeResources(gpu);
		}

		for (VmaAllocation allocation : _allocations) {
			gpu.AddGarbage(allocation);
		}

		_framebufferByPass.Clear(ReleaseMemorySemantics());
		_combinedAttachments.Clear(ReleaseMemorySemantics());
		_allocations.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	void Swap(Framebuffer::Attachment& lhs, Framebuffer::Attachment& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs.image, rhs.image);
		Swap(lhs.view, rhs.view);
	}

	// ---------------------------------------------------

	void Swap(Framebuffer& lhs, Framebuffer& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._combinedAttachments, rhs._combinedAttachments);
		Swap(lhs._allocations, rhs._allocations);
		Swap(lhs._framebufferByPass, rhs._framebufferByPass);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
