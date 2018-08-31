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

		ETInlineHint ETForceInlineHint void ParseAttachment(GraphicsPipelineBuilder& builder, VkFormat format) {
			builder.DefineAttachment(format);
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint void ParseAttachments(GraphicsPipelineBuilder& builder, const GraphicsPipeline* pipeline) {
			const auto attachments(pipeline->Attachments());
			for (uoffset_t id(0u); id < attachments->size(); ++id) {
				//const auto attachment(attachments->Get(id));
				ParseAttachment(builder, VK_FORMAT_UNDEFINED);
			}
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint GraphicsPassScale ParseScale(const Dynamic* scale) {
			return { scale->Width(), scale->Height() };
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint VkExtent3D ParseScale(const Static* resolution) {
			return { resolution->Width(), resolution->Height(), resolution->Depth() };
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseStage(GraphicsPipelineBuilder& pipeline, uoffset_t /*stageIndex*/, const DrawStage* stage) {
			switch (stage->Dimensions_type()) {
			case PassDimensions::Dynamic: pipeline.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, ParseScale(stage->Dimensions_as<Dynamic>()), VK_SAMPLE_COUNT_1_BIT, AsString(stage->ShaderUsageName())); break;
			case PassDimensions::Static: pipeline.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, ParseScale(stage->Dimensions_as<Static>()), VK_SAMPLE_COUNT_1_BIT, AsString(stage->ShaderUsageName())); break;
			default: return Error::Unspecified;
			};

			for (uint32 attachment : *stage->InputAttachments()) {
				pipeline.AppendInput(attachment, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
				/*	if (images.GetSize() <= id) {
					asset.WriteLog(MessageType::Error, "Graphics pipeline {} references out-of-bounds input attachment {} in stage {}!" UTF8_NEWLINE, GetPath(), id, pass);
					return Error::InvalidParameter;
				}*/
			}

			for (uint32 attachment : *stage->ColorAttachments()) {
				pipeline.AppendColorOutput(attachment, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
				/*	if (images.GetSize() <= id) {
					asset.WriteLog(MessageType::Error, "Graphics pipeline {} references out-of-bounds color attachment {} in stage {}!" UTF8_NEWLINE, GetPath(), id, pass);
					return Error::InvalidParameter;
				}
				*/
			}

			if (stage->DepthAttachment() != VK_ATTACHMENT_UNUSED) {
				const uint32 attachment(stage->DepthAttachment());
				pipeline.SetDepthStencilBuffer(attachment, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
				/*	if (images.GetSize() <= id) {
					asset.WriteLog(MessageType::Error, "Graphics pipeline {} references out-of-bounds depth/stencil attachment {} in pass {}!" UTF8_NEWLINE, GetPath(), id, pass);
					return Error::InvalidParameter;
				}
				*/
			}

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseStage(GraphicsPipelineBuilder& pipeline, uoffset_t /*stageIndex*/, const CopyStage* stage) {
			pipeline.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, VkExtent3D { 1u, 1u, 1u }, VK_SAMPLE_COUNT_1_BIT, "Copy");
			const uint32 source(stage->Source());
			const uint32 target(stage->Target());
			/*
			if (images.GetSize() <= sourceId) {
				asset.WriteLog(MessageType::Error, "Graphics pipeline {} references unknown source attachment {} in stage {}!" UTF8_NEWLINE, GetPath(), source, stageId);
				return Error::InvalidParameter;
			}

			if (images.GetSize() <= targetId) {
				asset.WriteLog(MessageType::Error, "Graphics pipeline {} references unknown target attachment {} in stage {}!" UTF8_NEWLINE, GetPath(), target, stageId);
				return Error::InvalidParameter;
			}
			*/

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseStage(GraphicsPipelineBuilder& pipeline, uoffset_t stageIndex, const ComputeStage* stage) {
			pipeline.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, VkExtent3D { 1u, 1u, 1u }, VK_SAMPLE_COUNT_1_BIT, AsString(stage->ShaderName()));
			for (uint32 attachment : *stage->Attachments()) {
				/*
				if (images.GetSize() <= id) {
					asset.WriteLog(MessageType::Error, "Graphics pipeline {} references out-of-bounds input attachment {} in stage {}!" UTF8_NEWLINE, GetPath(), attachment, stageId);
					return Error::InvalidParameter;
				}
				*/
			}

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseStages(GraphicsPipelineBuilder& pipeline, const FlatBuffers::GraphicsPipeline* asset) {
			const auto stageTypes(asset->Stages_type());
			const auto stages(asset->Stages());

			for (uoffset_t stage(0u); stage < stages->size(); ++stage) {
				switch (stageTypes->GetEnum<PipelineStage>(stage)) {
				case PipelineStage::DrawStage: ET_ABORT_UNLESS(ParseStage(pipeline, stage, stages->GetAs<DrawStage>(stage))); break;
				case PipelineStage::CopyStage: ET_ABORT_UNLESS(ParseStage(pipeline, stage, stages->GetAs<CopyStage>(stage))); break;
				case PipelineStage::ComputeStage: ET_ABORT_UNLESS(ParseStage(pipeline, stage, stages->GetAs<ComputeStage>(stage))); break;
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

		const FlatBuffers::GraphicsPipeline* const pipeline(FlatBuffers::GetGraphicsPipeline(asset.Begin()));
		GraphicsPipelineBuilder                    builder;

		ParseAttachments(builder, pipeline);
		ParseStages(builder, pipeline);
		builder.Finish();

		Swap(*this, builder);

		return Error::None;
	}

	// ---------------------------------------------------

	void GraphicsPipelineAsset::FreeResources() {
		GraphicsPipelineBuilder empty;
		empty.Finish(/*andOptimize =*/false);
		Swap(*this, empty);
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringView GraphicsPipelineAsset::GetExtension() ETNoexceptHint {
		return { GraphicsPipelineExtension(), StringLength(GraphicsPipelineExtension()) };
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
