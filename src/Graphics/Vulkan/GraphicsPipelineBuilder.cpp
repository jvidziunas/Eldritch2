/*==================================================================*\
  GraphicsPipelineBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		ETConstexpr uint32 EndOfFrame(~0u), StartOfFrame(0u);

		// ---

		template <uint32 count>
		ETInlineHint ETForceInlineHint PipelineAttachmentReference* FindOpenSlot(PipelineAttachmentReference (&attachments)[count]) ETNoexceptHint {
			return FindIf(attachments, End(attachments), [](const PipelineAttachmentReference& attachment) { return attachment.globalIndex == VK_ATTACHMENT_UNUSED; });
		}

	} // anonymous namespace

	PipelineAttachmentDescription::PipelineAttachmentDescription(VkFormat format, uint32 mipLevels, bool persistent) ETNoexceptHint : format(format),
																																	  samplesPerPixel(0u),
																																	  flags(0u),
																																	  usages(0u),
																																	  viewFlags(0u),
																																	  layers(1u),
																																	  mipLevels(mipLevels),
																																	  firstRead(persistent ? StartOfFrame : EndOfFrame),
																																	  lastRead(persistent ? EndOfFrame : StartOfFrame),
																																	  firstWrite(persistent ? StartOfFrame : EndOfFrame),
																																	  lastWrite(persistent ? EndOfFrame : StartOfFrame),
																																	  resolution(/*width =*/0.0f, /*height =*/0.0f) {}

	// ---------------------------------------------------

	bool PipelineAttachmentDescription::TryMarkUsed(VkImageUsageFlags usage, uint32 pass, const PipelineFramebufferDescription& framebuffer) ETNoexceptHint {
		if (ET_UNLIKELY(!framebuffer.resolution.IsCompatible(resolution))) {
			return false;
		}

		usages |= usage;
		samplesPerPixel = Max(framebuffer.samplesPerPixel, samplesPerPixel);
		firstRead       = Min(pass, firstRead);
		lastRead        = Max(pass, lastRead);
		firstWrite      = Min(pass, firstWrite);
		lastWrite       = Max(pass, lastWrite);

		return true;
	}

	// ---------------------------------------------------

	bool PipelineAttachmentDescription::TryMarkWritten(VkImageUsageFlags usage, uint32 pass, const PipelineFramebufferDescription& framebuffer) ETNoexceptHint {
		if (ET_UNLIKELY(!framebuffer.resolution.IsCompatible(resolution))) {
			return false;
		}

		usages |= usage;
		samplesPerPixel = Max(framebuffer.samplesPerPixel, samplesPerPixel);
		firstWrite      = Min(pass, firstWrite);
		lastWrite       = Max(pass, lastWrite);

		return true;
	}

	// ---------------------------------------------------

	bool PipelineAttachmentDescription::TryMarkRead(VkImageUsageFlags usage, uint32 pass, const PipelineFramebufferDescription& framebuffer) ETNoexceptHint {
		if (ET_UNLIKELY(!framebuffer.resolution.IsCompatible(resolution))) {
			return false;
		}

		usages |= usage;
		firstRead = Min(pass, firstRead);
		lastRead  = Max(pass, lastRead);

		return true;
	}

	// ---------------------------------------------------

	PipelineBufferDescription::PipelineBufferDescription(VkDeviceSize byteSize) ETNoexceptHint : byteSize(byteSize), flags(0u), usages(0u) {}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::GraphicsPipelineBuilder() :
		_attachments(MallocAllocator("Vulkan Graphics Pipeline Attachment List Allocator")),
		_buffers(MallocAllocator("Vulkan Graphics Pipeline Buffer List Allocator")),
		_passes(MallocAllocator("Vulkan Graphics Pipeline Pass List Allocator")) {}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::GraphicsPipelineBuilder(GraphicsPipelineBuilder&& builder) :
		GraphicsPipelineBuilder() {
		Swap(*this, builder);
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::BeginPass(PassType type, PassDimensions dimensions, uint32 samplesPerPixel, StringView name) {
		_passes.EmplaceBack(type, dimensions, samplesPerPixel, name);
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::AppendInput(uint32 globalId) {
		if (ET_UNLIKELY(globalId > _attachments.GetSize())) {
			return false;
		}

		PipelineFramebufferDescription& framebuffer(_passes.Back().framebuffer);
		const auto                      slot(FindOpenSlot(framebuffer.inputAttachments));
		if (slot == End(framebuffer.inputAttachments) || !_attachments[globalId].TryMarkRead(VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, uint32(_passes.GetSize() - 1u), framebuffer)) {
			return false;
		}

		*slot = PipelineAttachmentReference(globalId, /*firstLayer =*/0u, /*layers =*/VK_REMAINING_ARRAY_LAYERS);
		return true;
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::AppendColorOutput(uint32 globalId) {
		if (ET_UNLIKELY(globalId > _attachments.GetSize())) {
			return false;
		}

		PipelineFramebufferDescription& framebuffer(_passes.Back().framebuffer);
		const auto                      slot(FindOpenSlot(framebuffer.colorAttachments));
		if (slot == End(framebuffer.colorAttachments) || !_attachments[globalId].TryMarkWritten(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, uint32(_passes.GetSize() - 1u), framebuffer)) {
			return false;
		}

		*slot = PipelineAttachmentReference(globalId, /*firstLayer =*/0u, /*layers =*/VK_REMAINING_ARRAY_LAYERS);
		return true;
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::SetDepthStencilBuffer(uint32 globalId) {
		if (ET_UNLIKELY(_attachments.GetSize() <= globalId)) {
			return false;
		}

		PipelineFramebufferDescription& framebuffer(_passes.Back().framebuffer);
		const auto                      slot(ETAddressOf(framebuffer.depthAttachment));
		if (slot->globalIndex != VK_ATTACHMENT_UNUSED || !_attachments[globalId].TryMarkUsed(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, uint32(_passes.GetSize() - 1u), framebuffer)) {
			return false;
		}

		*slot = PipelineAttachmentReference(globalId, /*firstLayer =*/0u, /*layers =*/VK_REMAINING_ARRAY_LAYERS);
		return true;
	}

	// ---------------------------------------------------

	void Swap(GraphicsPipelineBuilder& lhs, GraphicsPipelineBuilder& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._attachments, rhs._attachments);
		Swap(lhs._buffers, rhs._buffers);
		Swap(lhs._passes, rhs._passes);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
