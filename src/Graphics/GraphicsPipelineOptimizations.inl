/*==================================================================*\
  GraphicsPipelineOptimizations.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	namespace Detail {

		template <typename AttachmentReference>
		ETInlineHint ETForceInlineHint void PatchReference(AttachmentReference& source, const ArrayMap<uint32, uint32>& indexBySource) ETNoexceptHint {
			if (source.globalIndex == ~(0u)) {
				return;
			}

			source.globalIndex = indexBySource.Find(source.globalIndex).second;
		}

	} // namespace Detail

	template <class AttachmentsList, class BuffersList, class PassesList>
	ETInlineHint ETForceInlineHint void StripUnusedResourcesPass::operator()(AttachmentsList& attachments, BuffersList& /*buffers*/, PassesList& passes) const {
		// Determine the new index of each source attachment after all unused elements have been removed.
		ArrayMap<uint32, uint32> indexBySource(MallocAllocator("Graphics Pipeline Remap Table Allocator"), LessThan<uint32>(), attachments.GetSize());
		for (uint32 source(0u), validIndex(0u); source != attachments.GetSize(); ++source) {
			if (!attachments[source].IsUsed()) {
				continue;
			}

			indexBySource.Emplace(source, validIndex++);
		}

		attachments.EraseIf([](typename AttachmentsList::ConstReference attachment) { return !attachment.IsUsed(); });
		for (typename PassesList::Reference pass : passes) {
			for (auto& output : pass.colorAttachments) {
				Detail::PatchReference(output, indexBySource);
			}
			for (auto& input : pass.inputAttachments) {
				Detail::PatchReference(input, indexBySource);
			}

			Detail::PatchReference(pass.depthStencilAttachment, indexBySource);
		}
	}

}} // namespace Eldritch2::Graphics
