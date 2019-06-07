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

	ETInlineHint void StripUnusedResourcesPass::operator()(PipelineBuilder::AttachmentList& attachments, PipelineBuilder::BufferList& buffers, PipelineBuilder::StageList& stages) const {
		using AttachmentIndex = PipelineBuilder::AttachmentIndex;
		using IndexMap        = ArrayMap<AttachmentIndex, AttachmentIndex>;

		static ETConstexpr auto PatchReference([](StageAttachment& source, const IndexMap& indexBySource) ETNoexceptHint {
			if (source.globalIndex == ~(0u)) {
				return;
			}

			source.globalIndex = indexBySource.Find(source.globalIndex)->second;
		});

		// ---

		// Determine the new index of each source attachment after all unused elements have been removed.
		IndexMap indexBySource(IndexMap::AllocatorType("Graphics Pipeline Attachment Remap Table Allocator"), IndexMap::SortPredicateType(), attachments.GetSize());
		for (IndexMap::KeyType source(0u), validIndex(0u); source != attachments.GetSize(); ++source) {
			if (!attachments[source].IsUsed()) {
				continue;
			}

			indexBySource.Emplace(source, validIndex++);
		}

		attachments.EraseIf([](const PipelineAttachment& attachment) { return !attachment.IsUsed(); });
		for (PipelineBuilder::StageList::Reference stage : stages) {
			for (auto& output : stage.colorAttachments) {
				PatchReference(output, indexBySource);
			}
			for (auto& input : stage.inputAttachments) {
				PatchReference(input, indexBySource);
			}

			PatchReference(stage.depthAttachment, indexBySource);
		}
	}

	// ---------------------------------------------------

	ETInlineHint void OptimizeAttachmentBandwidthPass::operator()(PipelineBuilder::AttachmentList& attachments, PipelineBuilder::BufferList& buffers, PipelineBuilder::StageList& stages) const ETNoexceptHint {
		for (PipelineBuilder::StageList::SizeType stageIndex(0u); stageIndex < stages.GetSize(); ++stageIndex) {
			for (auto& output : stages[stageIndex].colorAttachments) {
				PipelineBuilder::AttachmentList::ConstReference attachment(attachments[output.globalIndex]);

				output.shouldLoad  = stageIndex > attachment.firstWrite;
				output.shouldStore = stageIndex < attachment.lastRead;
			}
		}
	}

}} // namespace Eldritch2::Graphics
