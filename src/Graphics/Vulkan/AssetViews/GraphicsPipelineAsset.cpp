/*==================================================================*\
  GraphicsPipelineAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/GraphicsPipelineAsset.hpp>
#include <Graphics/GraphicsPipelineOptimizations.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Flatbuffers/FlatbufferTools.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/GraphicsPipeline_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	using namespace ::Eldritch2::Graphics::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	namespace {

		ETInlineHint ETForceInlineHint ETPureFunctionHint bool IsPersistent(const Attachment* attachment) ETNoexceptHint {
			return int(attachment->Flags() & AttachmentFlags::Persistent) != 0;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseGlobalAttachment(GraphicsPipelineBuilder& builder, const Attachment* attachment) ETNoexceptHint {
			if (size_t(attachment->Format()) >= ETCountOf(TextureFormats)) {
				return Error::InvalidParameter;
			}

			builder.DefineAttachment(TextureFormats[size_t(attachment->Format())].deviceFormat, IsPersistent(attachment));
			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseGlobalAttachments(GraphicsPipelineBuilder& builder, const GraphicsPipeline* pipeline) ETNoexceptHint {
			const auto attachments(pipeline->Attachments());
			for (uoffset_t attachment(0u), end(attachments->size()); attachment < end; ++attachment) {
				ET_ABORT_UNLESS(ParseGlobalAttachment(builder, attachments->Get(attachment)));
			}

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint Graphics::PassDimensions ParseDimensions(const Dynamic* dimensions) ETNoexceptHint {
			return { dimensions->Width(), dimensions->Height() };
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint Graphics::PassDimensions ParseDimensions(const Static* dimensions) ETNoexceptHint {
			return { dimensions->Width(), dimensions->Height(), dimensions->Depth() };
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseInput(GraphicsPipelineBuilder& builder, const AttachmentReference* input) ETNoexceptHint {
			return builder.AppendInput(input->GlobalIndex()) ? Error::None : Error::InvalidParameter;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseOutput(GraphicsPipelineBuilder& builder, const AttachmentReference* output) ETNoexceptHint {
			return builder.AppendColorOutput(output->GlobalIndex()) ? Error::None : Error::InvalidParameter;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseDepthAttachment(GraphicsPipelineBuilder& builder, const AttachmentReference* attachment) ETNoexceptHint {
			return builder.SetDepthStencilBuffer(attachment->GlobalIndex()) ? Error::None : Error::InvalidParameter;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseInputs(GraphicsPipelineBuilder& builder, const FlatbufferVector<const AttachmentReference*>* inputs) ETNoexceptHint {
			for (uoffset_t input(0u), end(inputs->size()); input < end; ++input) {
				ET_ABORT_UNLESS(ParseInput(builder, inputs->Get(input)));
			}

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseOutputs(GraphicsPipelineBuilder& builder, const FlatbufferVector<const AttachmentReference*>* outputs) ETNoexceptHint {
			for (uoffset_t output(0u), end(outputs->size()); output < end; ++output) {
				ET_ABORT_UNLESS(ParseOutput(builder, outputs->Get(output)));
			}

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseStage(GraphicsPipelineBuilder& builder, uoffset_t /*stageIndex*/, const DrawStage* stage) ETNoexceptHint {
			switch (stage->Dimensions_type()) {
			default: return Error::InvalidParameter;
			case AssetViews::PassDimensions::Dynamic: builder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, ParseDimensions(stage->Dimensions_as<Dynamic>()), /*samplesPerPixel =*/1u, AsString(stage->ShaderUsageName())); break;
			case AssetViews::PassDimensions::Static: builder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, ParseDimensions(stage->Dimensions_as<Static>()), /*samplesPerPixel =*/1u, AsString(stage->ShaderUsageName())); break;
			};

			ET_ABORT_UNLESS(ParseInputs(builder, stage->InputAttachments()));
			ET_ABORT_UNLESS(ParseOutputs(builder, stage->ColorAttachments()));
			if (stage->DepthAttachment()) {
				ET_ABORT_UNLESS(ParseDepthAttachment(builder, stage->DepthAttachment()));
			}

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseStage(GraphicsPipelineBuilder& builder, uoffset_t /*stageIndex*/, const CopyStage* /*stage*/) ETNoexceptHint {
			builder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, /*dimensions =*/ { 1u, 1u, 1u }, /*samplesPerPixel =*/1u, "Copy");
			/* if (images.GetSize() <= sourceId) {
				asset.WriteLog(MessageType::Error, "Graphics pipeline {} references unknown source attachment {} in stage {}!" UTF8_NEWLINE, GetPath(), source, stageId);
				return Error::InvalidParameter;
			}

			if (images.GetSize() <= targetId) {
				asset.WriteLog(MessageType::Error, "Graphics pipeline {} references unknown target attachment {} in stage {}!" UTF8_NEWLINE, GetPath(), target, stageId);
				return Error::InvalidParameter;
			}*/

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseStage(GraphicsPipelineBuilder& builder, uoffset_t /*stageIndex*/, const ComputeStage* stage) ETNoexceptHint {
			builder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, /*dimensions =*/ { 1u, 1u, 1u }, /*samplesPerPixel =*/1u, AsString(stage->ShaderName()));
			/* for (uint32 attachment : *stage->Attachments()) {
				if (images.GetSize() <= id) {
					asset.WriteLog(MessageType::Error, "Graphics pipeline {} references out-of-bounds input attachment {} in stage {}!" UTF8_NEWLINE, GetPath(), attachment, stageId);
					return Error::InvalidParameter;
				}
			}*/

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseStages(GraphicsPipelineBuilder& builder, const FlatBuffers::GraphicsPipeline* asset) ETNoexceptHint {
			const auto stageTypes(asset->Stages_type());
			const auto stages(asset->Stages());

			for (uoffset_t stage(0u); stage < stages->size(); ++stage) {
				switch (stageTypes->GetEnum<PipelineStage>(stage)) {
				case PipelineStage::DrawStage: ET_ABORT_UNLESS(ParseStage(builder, stage, stages->GetAs<DrawStage>(stage))); break;
				case PipelineStage::CopyStage: ET_ABORT_UNLESS(ParseStage(builder, stage, stages->GetAs<CopyStage>(stage))); break;
				case PipelineStage::ComputeStage: ET_ABORT_UNLESS(ParseStage(builder, stage, stages->GetAs<ComputeStage>(stage))); break;
				} // switch (types->GetEnum<PipelineStage>(stage))
			}

			return Error::None;
		}

	} // anonymous namespace

	ErrorCode GraphicsPipelineAsset::BindResources(const Builder& asset) {
		//	Ensure we're working with data that can plausibly represent a graphics pipeline.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifyGraphicsPipelineBuffer(verifier)) {
			asset.WriteLog(Severity::Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		const GraphicsPipeline* const pipeline(GetGraphicsPipeline(asset.Begin()));
		GraphicsPipelineBuilder       builder;

		ET_ABORT_UNLESS(ParseGlobalAttachments(builder, pipeline));
		ET_ABORT_UNLESS(ParseStages(builder, pipeline));

		Swap(*this, builder.Optimize(StripUnusedResourcesPass()));

		return Error::None;
	}

	// ---------------------------------------------------

	void GraphicsPipelineAsset::FreeResources() {
		GraphicsPipelineBuilder empty;
		Swap(*this, empty.Optimize());
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringView GraphicsPipelineAsset::GetExtension() ETNoexceptHint {
		return { GraphicsPipelineExtension(), StringLength(GraphicsPipelineExtension()) };
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
