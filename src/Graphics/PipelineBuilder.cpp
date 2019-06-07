/*==================================================================*\
  PipelineBuilder.cpp
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
#include <Graphics/PipelineBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	PipelineBuilder::PipelineBuilder() ETNoexceptHint : _attachments(AttachmentList::AllocatorType("Graphics Pipeline Builder Attachment List Allocator")),
														_buffers(BufferList::AllocatorType("Graphics Pipeline Builder Buffer List Allocator")),
														_stages(StageList::AllocatorType("Graphics Pipeline Builder Pass List Allocator")) {}

	// ---------------------------------------------------

	PipelineBuilder::PipelineBuilder(PipelineBuilder&& builder) ETNoexceptHint : PipelineBuilder() {
		Swap(*this, builder);
	}

	// ---------------------------------------------------

	Result PipelineBuilder::DefineDrawStage(StringSpan name, StageResolution resolution, uint32 samplesPerPixel, StageAttachment depthAttachment, Span<const StageAttachment*> inputAttachments, Span<const StageAttachment*> colorAttachments) {
		const auto IsValidAttachment([&](const StageAttachment& attachment) ETNoexceptHint -> bool {
			return attachment.globalIndex <= _attachments.GetSize() && _attachments[attachment.globalIndex].resolution.IsCompatibleWith(resolution);
		});

		// ---

		ET_ABORT_UNLESS(IsValidAttachment(depthAttachment) ? Result::Success : Result::InvalidParameter);
		ET_ABORT_UNLESS(inputAttachments.GetSize() <= ETCountOf(StageBuilder::inputAttachments) ? Result::Success : Result::InvalidParameter);
		ET_ABORT_UNLESS(All(inputAttachments.Begin(), inputAttachments.End(), IsValidAttachment) ? Result::Success : Result::InvalidParameter);
		ET_ABORT_UNLESS(colorAttachments.GetSize() <= ETCountOf(StageBuilder::colorAttachments) ? Result::Success : Result::InvalidParameter);
		ET_ABORT_UNLESS(All(colorAttachments.Begin(), colorAttachments.End(), IsValidAttachment) ? Result::Success : Result::InvalidParameter);

		StageBuilder& stage(_stages.EmplaceBack(Stage::Draw, name, resolution, samplesPerPixel));
		stage.depthAttachment = depthAttachment;
		Copy(inputAttachments.Begin(), inputAttachments.End(), stage.inputAttachments);
		Copy(colorAttachments.Begin(), colorAttachments.End(), stage.colorAttachments);

		return Result::Success;
	}

	// ---------------------------------------------------

	Result PipelineBuilder::DefineDrawStage(StringSpan name, StageResolution resolution, uint32 samplesPerPixel, Span<const StageAttachment*> inputAttachments, Span<const StageAttachment*> colorAttachments) {
		const auto IsValidAttachment([&](const StageAttachment& attachment) ETNoexceptHint -> bool {
			return attachment.globalIndex <= _attachments.GetSize() && _attachments[attachment.globalIndex].resolution.IsCompatibleWith(resolution);
		});

		// ---

		ET_ABORT_UNLESS(inputAttachments.GetSize() <= ETCountOf(StageBuilder::inputAttachments) ? Result::Success : Result::InvalidParameter);
		ET_ABORT_UNLESS(All(inputAttachments.Begin(), inputAttachments.End(), IsValidAttachment) ? Result::Success : Result::InvalidParameter);
		ET_ABORT_UNLESS(colorAttachments.GetSize() <= ETCountOf(StageBuilder::colorAttachments) ? Result::Success : Result::InvalidParameter);
		ET_ABORT_UNLESS(All(colorAttachments.Begin(), colorAttachments.End(), IsValidAttachment) ? Result::Success : Result::InvalidParameter);

		StageBuilder& stage(_stages.EmplaceBack(Stage::Draw, name, resolution, samplesPerPixel));
		Copy(inputAttachments.Begin(), inputAttachments.End(), stage.inputAttachments);
		Copy(colorAttachments.Begin(), colorAttachments.End(), stage.colorAttachments);

		return Result::Success;
	}

	// ---------------------------------------------------

	Result PipelineBuilder::DefineComputeStage(StringSpan name, StageResolution resolution, Span<const StageAttachment*> attachments) {
		const auto IsValidAttachment([&](const StageAttachment& attachment) ETNoexceptHint -> bool {
			return attachment.globalIndex <= _attachments.GetSize();
		});

		// ---

		ET_ABORT_UNLESS(attachments.GetSize() <= ETCountOf(StageBuilder::inputAttachments) ? Result::Success : Result::InvalidParameter);

		StageBuilder& stage(_stages.EmplaceBack(Stage::Compute, name, resolution, /*samplesPerPixel =*/0u));
		Copy(attachments.Begin(), attachments.End(), stage.inputAttachments);

		return Result::Success;
	}

	// ---------------------------------------------------

	Result PipelineBuilder::DefineCopyStage(StringSpan name, Span<const StageAttachment*> targets, Span<const StageAttachment*> sources) {
		const auto IsValidAttachment([&](const StageAttachment& attachment) ETNoexceptHint -> bool {
			return attachment.globalIndex <= _attachments.GetSize();
		});

		// ---

		ET_ABORT_UNLESS(targets.GetSize() == sources.GetSize() ? Result::Success : Result::InvalidParameter);
		ET_ABORT_UNLESS(targets.GetSize() <= ETCountOf(StageBuilder::colorAttachments) ? Result::Success : Result::InvalidParameter);

		StageBuilder& stage(_stages.EmplaceBack(Stage::Copy, name, StageResolution(1u, 1u, 1u), /*samplesPerPixel =*/0u));
		Copy(sources.Begin(), sources.End(), stage.inputAttachments);
		Copy(targets.Begin(), targets.End(), stage.colorAttachments);

		return Result::Success;
	}

	// ---------------------------------------------------

	void Swap(PipelineBuilder& lhs, PipelineBuilder& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._attachments, rhs._attachments);
		Swap(lhs._buffers, rhs._buffers);
		Swap(lhs._stages, rhs._stages);
	}

}} // namespace Eldritch2::Graphics
