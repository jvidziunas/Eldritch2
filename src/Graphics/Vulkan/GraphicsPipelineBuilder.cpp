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

		constexpr GraphicsPipelineBuilder::AttachmentReference InvalidAttachment{ VK_ATTACHMENT_UNUSED, VK_IMAGE_LAYOUT_UNDEFINED };

		template <uint32 size>
		ETInlineHint void PatchAttachmentReferences(GraphicsPipelineBuilder::AttachmentReference (&references)[size], const ArrayMap<uint32, uint32>& remapTable) {
			for (GraphicsPipelineBuilder::AttachmentReference& reference : references) {
				if (reference.index != VK_ATTACHMENT_UNUSED) {
					reference.index = remapTable[reference.index];
				}
			}
		}

	} // anonymous namespace

	GraphicsPipelineBuilder::Attachment::Attachment(VkFormat format, VkSampleCountFlags quality, float32 widthScale, float32 heightScale, float32 depthScale) :
		format(format),
		flags(0u),
		usages(0u),
		firstPass(~0u),
		lastPass(0u),
		msaaQuality(quality),
		staticResolution(0u) {
		scale[0] = widthScale;
		scale[1] = heightScale;
		scale[2] = depthScale;
	}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::Attachment::Attachment(VkFormat format, VkSampleCountFlags quality, VkExtent3D dimensions) :
		format(format),
		flags(0u),
		usages(0u),
		firstPass(~0u),
		lastPass(0u),
		msaaQuality(quality),
		staticResolution(1u) {
		this->dimensions = dimensions;
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::Attachment::MarkUsed(uint32 pass, VkImageUsageFlags usage) {
		usages |= usage;

		firstPass = Min(pass, firstPass);
		lastPass  = Max(pass, lastPass);
	}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::Buffer::Buffer(VkDeviceSize sizeInBytes) :
		sizeInBytes(sizeInBytes),
		flags(0u),
		usages(0u) {}

	// ---------------------------------------------------

	GraphicsPipelineBuilder::Pass::Pass(const Utf8Char* const name) :
		inputAttachmentCount(0u),
		attachmentCount(0u),
		inputAttachments{ InvalidAttachment },
		attachments{ InvalidAttachment },
		depthStencilAttachment(InvalidAttachment) {
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

	void GraphicsPipelineBuilder::Begin(VkPipelineBindPoint bindPoint, const Utf8Char* name) {
		_attachments.Clear();
		_passes.Clear();

		BeginPass(bindPoint, name);
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::BeginPass(VkPipelineBindPoint bindPoint, const Utf8Char* name) {
		_passes.EmplaceBack(name);
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::AttachInput(uint32 index, VkImageLayout layout) {
		enum : uint32 { MaxInputAttachments = _countof(Pass::inputAttachments) };

		Pass& pass(_passes.Back());
		if (pass.inputAttachmentCount == MaxInputAttachments || _attachments.GetSize() <= index) {
			return false;
		}

		pass.inputAttachments[pass.inputAttachmentCount++] = AttachmentReference{ index, layout };
		_attachments[index].MarkUsed(uint32(_passes.GetSize() - 1u), VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);

		return true;
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::AttachColorOutput(uint32 index, VkImageLayout layout) {
		enum : uint32 { MaxAttachments = _countof(Pass::attachments) };

		Pass& pass(_passes.Back());
		if (pass.inputAttachmentCount == MaxAttachments || _attachments.GetSize() <= index) {
			return false;
		}

		pass.attachments[pass.attachmentCount++] = AttachmentReference{ index, layout };
		_attachments[index].MarkUsed(uint32(_passes.GetSize() - 1u), VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);

		return true;
	}

	// ---------------------------------------------------

	bool GraphicsPipelineBuilder::AttachDepthStencilBuffer(uint32 index, VkImageLayout layout) {
		Pass& pass(_passes.Back());
		if (_attachments.GetSize() <= index || pass.depthStencilAttachment.index != VK_ATTACHMENT_UNUSED) {
			return false;
		}

		pass.depthStencilAttachment = AttachmentReference{ index, layout };
		_attachments[index].MarkUsed(uint32(_passes.GetSize() - 1u), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

		return true;
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::Finish(bool andOptimize) {
		if (!andOptimize) {
			return;
		}

		StripUnusedResources();
	}

	// ---------------------------------------------------

	void GraphicsPipelineBuilder::StripUnusedResources() {
		ArrayMap<uint32, uint32> remapTable(MallocAllocator("Graphics Pipeline Remap Table Allocator"));
		remapTable.Reserve(_attachments.GetSize());

		// We need to figure out the new index of each attachment after all dead elements have been removed.
		for (uint32 index(0u), validIndex(0u); index < _attachments.GetSize(); ++index) {
			remapTable.Insert(index, validIndex);

			if (attachments[index].IsReferenced()) {
				++validIndex;
			}
		}

		_passes.Erase(RemoveIf(_passes.Begin(), _passes.End(), [](const Attachment& attachment) { return attachment.IsReferenced() == false; }), _passes.End());

		for (Pass& pass : _passes) {
			PatchAttachmentReferences(pass.attachments, remapTable);
			PatchAttachmentReferences(pass.inputAttachments, remapTable);
			if (pass.depthStencilAttachment.index != VK_ATTACHMENT_UNUSED) {
				pass.depthStencilAttachment.index.index = remapTable[pass.depthStencilAttachment.index.index];
			}
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
