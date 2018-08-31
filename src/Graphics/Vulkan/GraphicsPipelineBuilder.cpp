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

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	namespace {

		ETConstexpr GraphicsPipelineBuilder::AttachmentReference InvalidAttachment { VK_ATTACHMENT_UNUSED, VK_IMAGE_LAYOUT_UNDEFINED };

		// ---

		template <uint32 count>
		ETInlineHint ETForceInlineHint GraphicsPipelineBuilder::AttachmentReference* FindOpenSlot(GraphicsPipelineBuilder::AttachmentReference (&attachments)[count]) ETNoexceptHint {
			return Find(attachments, End(attachments), InvalidAttachment);
		}

	} // anonymous namespace

	GraphicsPipelineBuilder::AttachmentDescription::AttachmentDescription(VkFormat format) ETNoexceptHint : format(format),
																											sampleRates(0u),
																											flags(0u),
																											usages(0u),
																											firstWrite(~0u),
																											lastWrite(0u),
																											isPersistent(1u),
																											staticResolution(0u) {
		scale = GraphicsPassScale { 0.0f, 0.0f };
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::AttachmentDescription::MarkUsed(uint32 pass, VkSampleCountFlagBits sampleRate, VkImageUsageFlags usage) ETNoexceptHint {
		usages |= usage;
		sampleRates |= sampleRate;
		firstRead  = Min(pass, firstRead);
		lastRead   = Max(pass, lastRead);
		firstWrite = Min(pass, firstWrite);
		lastWrite  = Max(pass, lastWrite);
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::AttachmentDescription::MarkWritten(uint32 pass, VkSampleCountFlagBits sampleRate, VkImageUsageFlags usage) ETNoexceptHint {
		usages |= usage;
		sampleRates |= sampleRate;
		firstWrite = Min(pass, firstWrite);
		lastWrite  = Max(pass, lastWrite);
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::AttachmentDescription::MarkRead(uint32 pass, VkSampleCountFlagBits /*sampleRate*/, VkImageUsageFlags usage) ETNoexceptHint {
		usages |= usage;
		firstRead = Min(pass, firstRead);
		lastRead  = Max(pass, lastRead);
	}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::BufferDescription::BufferDescription(VkDeviceSize sizeInBytes) ETNoexceptHint : sizeInBytes(sizeInBytes), flags(0u), usages(0u) {}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::PassDescription::PassDescription(VkExtent3D resolution, VkSampleCountFlagBits samples, StringView name) ETNoexceptHint : depthStencilAttachment(InvalidAttachment), samples(samples) {
		Fill(inputAttachments, End(inputAttachments), InvalidAttachment);
		Fill(colorAttachments, End(colorAttachments), InvalidAttachment);

		this->resolution = resolution;
		name.Copy(this->name, ETCountOf(this->name));
	}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::PassDescription::PassDescription(GraphicsPassScale scale, VkSampleCountFlagBits samples, StringView name) ETNoexceptHint : depthStencilAttachment(InvalidAttachment), samples(samples) {
		Fill(inputAttachments, End(inputAttachments), InvalidAttachment);
		Fill(colorAttachments, End(colorAttachments), InvalidAttachment);

		this->scale = scale;
		name.Copy(this->name, ETCountOf(this->name));
	}

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

	void GraphicsPipelineBuilder::BeginPass(VkPipelineBindPoint bindPoint, VkExtent3D resolution, VkSampleCountFlagBits sampleRate, StringView name) {
		_passes.EmplaceBack(resolution, sampleRate, name);
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::BeginPass(VkPipelineBindPoint bindPoint, GraphicsPassScale scale, VkSampleCountFlagBits sampleRate, StringView name) {
		_passes.EmplaceBack(scale, sampleRate, name);
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::AppendInput(uint32 globalId, VkImageLayout layout) {
		if (ET_UNLIKELY(_attachments.GetSize() <= globalId)) {
			return false;
		}

		AttachmentDescription& attachment(_attachments[globalId]);
		PassDescription&       pass(_passes.Back());
		const auto             openSlot(FindOpenSlot(pass.inputAttachments));
		if (!attachment.SupportsResolution(GraphicsPassScale {}) || openSlot == End(pass.inputAttachments)) {
			return false;
		}

		attachment.MarkRead(uint32(_passes.GetSize() - 1u), pass.samples, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
		*openSlot = AttachmentReference { globalId, layout };

		return true;
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::AppendColorOutput(uint32 globalId, VkImageLayout layout) {
		if (ET_UNLIKELY(_attachments.GetSize() <= globalId)) {
			return false;
		}

		AttachmentDescription& attachment(_attachments[globalId]);
		PassDescription&       pass(_passes.Back());
		const auto             openSlot(FindOpenSlot(pass.colorAttachments));
		if (!attachment.SupportsResolution(GraphicsPassScale {}) || openSlot == End(pass.colorAttachments)) {
			return false;
		}

		attachment.MarkWritten(uint32(_passes.GetSize() - 1u), pass.samples, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
		*openSlot = AttachmentReference { globalId, layout };

		return true;
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::SetDepthStencilBuffer(uint32 globalId, VkImageLayout layout) {
		if (ET_UNLIKELY(_attachments.GetSize() <= globalId)) {
			return false;
		}

		AttachmentDescription& attachment(_attachments[globalId]);
		PassDescription&       pass(_passes.Back());
		if (!attachment.SupportsResolution(GraphicsPassScale {}) || pass.depthStencilAttachment.globalIndex != VK_ATTACHMENT_UNUSED) {
			return false;
		}

		attachment.MarkUsed(uint32(_passes.GetSize() - 1u), pass.samples, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
		pass.depthStencilAttachment = AttachmentReference { globalId, layout };

		return true;
	}

	// ---------------------------------------------------

	VkResult GraphicsPipelineBuilder::Finish(bool andOptimize) {
		if (ET_LIKELY(andOptimize)) {
			StripUnusedResources();
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::StripUnusedResources() {
		const auto PatchReference([](AttachmentReference& source, ArrayMap<uint32, uint32>& indexBySource) {
			if (source.globalIndex == VK_ATTACHMENT_UNUSED) {
				return;
			}

			source.globalIndex = indexBySource[source.globalIndex];
		});

		// Determine the new index of each source attachment after all unused elements have been removed.
		ArrayMap<uint32, uint32> indexBySource(MallocAllocator("Graphics Pipeline Remap Table Allocator"), LessThan<uint32>(), _attachments.GetSize());
		for (uint32 source(0u), validIndex(0u); source != _attachments.GetSize(); ++source) {
			if (!_attachments[source].IsUsed()) {
				continue;
			}

			indexBySource.Emplace(source, validIndex++);
		}

		_attachments.EraseIf([](const AttachmentDescription& attachment) { return !attachment.IsUsed(); });

		for (PassDescription& pass : _passes) {
			for (AttachmentReference& source : pass.colorAttachments) {
				PatchReference(source, indexBySource);
			}
			for (AttachmentReference& source : pass.inputAttachments) {
				PatchReference(source, indexBySource);
			}

			PatchReference(pass.depthStencilAttachment, indexBySource);
		}
	}

	// ---------------------------------------------------

	void Swap(GraphicsPipelineBuilder& lhs, GraphicsPipelineBuilder& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._attachments, rhs._attachments);
		Swap(lhs._buffers, rhs._buffers);
		Swap(lhs._passes, rhs._passes);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
