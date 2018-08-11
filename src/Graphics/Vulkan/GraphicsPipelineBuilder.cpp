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

		constexpr GraphicsPipelineBuilder::AttachmentReference InvalidAttachment { VK_ATTACHMENT_UNUSED, VK_IMAGE_LAYOUT_UNDEFINED };

		template <uint32 count>
		GraphicsPipelineBuilder::AttachmentReference* FindSlot(GraphicsPipelineBuilder::AttachmentReference (&attachments)[count]) {
			return FindIf(eastl::begin(attachments), eastl::end(attachments), [](const GraphicsPipelineBuilder::AttachmentReference& reference) {
				return reference.globalIndex == VK_ATTACHMENT_UNUSED;
			});
		}

	} // anonymous namespace

	GraphicsPipelineBuilder::AttachmentDescription::AttachmentDescription(VkFormat format, VkSampleCountFlags quality) :
		format(format),
		flags(0u),
		usages(0u),
		firstWrite(~0u),
		lastWrite(0u),
		msaaQuality(quality),
		isPersistent(1u),
		staticDimensions(0u) {
		scales[0] = 0.0f;
		scales[1] = 0.0f;
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::AttachmentDescription::MarkUsed(uint32 pass, VkImageUsageFlags usage) {
		usages |= usage;

		firstRead  = Min(pass, firstRead);
		lastRead   = Max(pass, lastRead);
		firstWrite = Min(pass, firstWrite);
		lastWrite  = Max(pass, lastWrite);
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::AttachmentDescription::MarkWritten(uint32 pass, VkImageUsageFlags usage) {
		usages |= usage;

		firstWrite = Min(pass, firstWrite);
		lastWrite  = Max(pass, lastWrite);
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::AttachmentDescription::MarkRead(uint32 pass, VkImageUsageFlags usage) {
		usages |= usage;

		firstRead = Min(pass, firstRead);
		lastRead  = Max(pass, lastRead);
	}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::BufferDescription::BufferDescription(VkDeviceSize sizeInBytes) :
		sizeInBytes(sizeInBytes),
		flags(0u),
		usages(0u) {
	}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::PassDescription::PassDescription(float32 widthScale, float32 heightScale, const Utf8Char* name) :
		inputAttachments { InvalidAttachment, InvalidAttachment, InvalidAttachment, InvalidAttachment },
		colorAttachments { InvalidAttachment, InvalidAttachment, InvalidAttachment, InvalidAttachment },
		depthStencilAttachment(InvalidAttachment) {
		this->widthScale  = widthScale;
		this->heightScale = heightScale;
		CopyString(this->name, name);
	}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::PassDescription::PassDescription(VkExtent3D dimensions, const Utf8Char* const name) :
		inputAttachments { InvalidAttachment, InvalidAttachment, InvalidAttachment, InvalidAttachment },
		colorAttachments { InvalidAttachment, InvalidAttachment, InvalidAttachment, InvalidAttachment },
		depthStencilAttachment(InvalidAttachment) {
		this->dimensions = dimensions;
		CopyString(this->name, name);
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

	void GraphicsPipelineBuilder::BeginPass(VkPipelineBindPoint bindPoint, float32 width, float32 height, const Utf8Char* name) {
		_passes.EmplaceBack(width, height, name);
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::BeginPass(VkPipelineBindPoint bindPoint, VkExtent3D resolution, const Utf8Char* name) {
		_passes.EmplaceBack(resolution, name);
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::AppendInput(uint32 attachment, VkImageLayout layout) {
		PassDescription&           pass(_passes.Back());
		AttachmentReference* const slot(FindSlot(pass.inputAttachments));
		if (slot == eastl::end(pass.inputAttachments)) {
			return false;
		}

		ET_ASSERT(_attachments.GetSize() <= attachment, "Attachment index {} out of range", attachment);

		*slot = AttachmentReference { attachment, layout };
		_attachments[attachment].MarkRead(uint32(_passes.GetSize() - 1u), VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);

		return true;
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::AppendColorOutput(uint32 attachment, VkImageLayout layout) {
		PassDescription&           pass(_passes.Back());
		AttachmentReference* const slot(FindSlot(pass.colorAttachments));
		if (slot == eastl::end(pass.colorAttachments)) {
			return false;
		}

		ET_ASSERT(_attachments.GetSize() <= attachment, "Attachment index {} out of range", attachment);

		*slot = AttachmentReference { attachment, layout };
		_attachments[attachment].MarkWritten(uint32(_passes.GetSize() - 1u), VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);

		return true;
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::SetDepthStencilBuffer(uint32 attachment, VkImageLayout layout) {
		PassDescription& pass(_passes.Back());
		if (pass.depthStencilAttachment.globalIndex != VK_ATTACHMENT_UNUSED) {
			return false;
		}

		ET_ASSERT(_attachments.GetSize() <= attachment, "Attachment index {} out of range", attachment);

		pass.depthStencilAttachment = AttachmentReference { attachment, layout };
		_attachments[attachment].MarkUsed(uint32(_passes.GetSize() - 1u), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

		return true;
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::Finish(bool andOptimize) {
		if (andOptimize) {
			StripUnusedResources();
		}
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::StripUnusedResources() {
		const auto PatchReference([](AttachmentReference& source, const ArrayMap<uint32, uint32>& indexBySource) {
			if (source.globalIndex == VK_ATTACHMENT_UNUSED) {
				return;
			}

			source.globalIndex = indexBySource.Find(source.globalIndex)->second;
		});

		ArrayMap<uint32, uint32> indexBySource(MallocAllocator("Graphics Pipeline Remap Table Allocator"));
		indexBySource.Reserve(_attachments.GetSize());

		// Determine the new index of each source attachment after all dead elements have been removed.
		for (uint32 source(0u), validIndex(0u); source < _attachments.GetSize(); ++source) {
			indexBySource.Emplace(source, validIndex);

			if (_attachments[source].IsReferenced()) {
				++validIndex;
			}
		}

		_attachments.Erase(RemoveIf(_attachments.Begin(), _attachments.End(), [](const AttachmentDescription& attachment) { return !attachment.IsReferenced(); }), _attachments.End());

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
