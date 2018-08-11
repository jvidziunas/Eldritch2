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
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/GraphicsPipeline_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace AssetViews {

	using namespace ::Eldritch2::Graphics::FlatBuffers;
	using namespace ::Eldritch2::Graphics::AssetViews;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	namespace {

		ETInlineHint void ParseAttachment(GraphicsPipelineBuilder& builder, VkFormat format, VkSampleCountFlags msaa) {
			builder.DefineAttachment(format, msaa);
		}

		// ---------------------------------------------------

		ETInlineHint void ParseAttachments(GraphicsPipelineBuilder& builder, const GraphicsPipeline* pipeline) {
			const auto attachments(pipeline->Attachments());
			for (uoffset_t id(0u); id < attachments->size(); ++id) {
				//const auto attachment(attachments->Get(id));

				ParseAttachment(builder, VK_FORMAT_UNDEFINED, VK_SAMPLE_COUNT_1_BIT);
			}
		}

		// ---------------------------------------------------

		ETInlineHint ErrorCode ParseStage(GraphicsPipelineBuilder& builder, uoffset_t /*stageIndex*/, const DrawStage* stage) {
			switch (stage->Dimensions_type()) {
			case PassDimensions::Dynamic: break;
			case PassDimensions::Static: break;
			default: return Error::Unspecified;
			};
			builder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, 1.0f, 1.0f, stage->ShaderUsageName()->c_str());

			for (uint32 id : *stage->InputAttachments()) {
				builder.AppendInput(id, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
				/*	if (images.GetSize() <= id) {
					asset.WriteLog(MessageType::Error, "Graphics pipeline {} references out-of-bounds input attachment {} in pass {}!" UTF8_NEWLINE, GetPath(), id, pass);
					return Error::InvalidParameter;
				}*/
			}

			for (uint32 id : *stage->ColorAttachments()) {
				builder.AppendColorOutput(id, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
				/*	if (images.GetSize() <= id) {
					asset.WriteLog(MessageType::Error, "Graphics pipeline {} references out-of-bounds color attachment {} in pass {}!" UTF8_NEWLINE, GetPath(), id, pass);
					return Error::InvalidParameter;
				}
				*/
			}

			if (stage->DepthAttachment() != VK_ATTACHMENT_UNUSED) {
				const uint32 id(stage->DepthAttachment());
				builder.SetDepthStencilBuffer(id, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
				/*	if (images.GetSize() <= id) {
					asset.WriteLog(MessageType::Error, "Graphics pipeline {} references out-of-bounds depth/stencil attachment {} in pass {}!" UTF8_NEWLINE, GetPath(), id, pass);
					return Error::InvalidParameter;
				}
				*/
			}

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ErrorCode ParseStage(GraphicsPipelineBuilder& builder, uoffset_t /*stageIndex*/, const CopyStage* stage) {
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

			builder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, VkExtent3D { 1u, 1u, 1u }, "Copy");

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ErrorCode ParseStage(GraphicsPipelineBuilder& builder, uoffset_t stageId, const ComputeStage* stage) {
			builder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, VkExtent3D { 1u, 1u, 1u }, stage->ShaderName()->c_str());

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

		ETInlineHint ErrorCode ParseStages(GraphicsPipelineBuilder& builder, const GraphicsPipeline* pipeline) {
			const auto stages(pipeline->Stages());
			const auto types(pipeline->Stages_type());

			for (uoffset_t stage(0u); stage < stages->size(); ++stage) {
				switch (types->GetEnum<PipelineStage>(stage)) {
				case PipelineStage::NONE: return Error::InvalidParameter; break;
				case PipelineStage::DrawStage: ET_FAIL_UNLESS(ParseStage(builder, stage, stages->GetAs<DrawStage>(stage))); break;
				case PipelineStage::CopyStage: ET_FAIL_UNLESS(ParseStage(builder, stage, stages->GetAs<CopyStage>(stage))); break;
				case PipelineStage::ComputeStage: ET_FAIL_UNLESS(ParseStage(builder, stage, stages->GetAs<ComputeStage>(stage))); break;
				default: return Error::InvalidParameter;
				} // switch (types->GetEnum<PipelineStage>(stage))
			}

			return Error::None;
		}

	} // anonymous namespace

	GraphicsPipelineAsset::GraphicsPipelineAsset(const Utf8Char* const assetPath) :
		Asset(assetPath) {
	}

	// ---------------------------------------------------

	ErrorCode GraphicsPipelineAsset::BindResources(const Builder& asset) {
		//	Ensure we're working with data that can plausibly represent a graphics pipeline.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifyGraphicsPipelineBuffer(verifier)) {
			asset.WriteLog(MessageType::Error, "Data integrity check failed for {}, aborting load." UTF8_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		const FlatBuffers::GraphicsPipeline* const pipeline(GetGraphicsPipeline(asset.Begin()));
		GraphicsPipelineBuilder                    pipelineBuilder;

		ParseAttachments(pipelineBuilder, pipeline);
		ParseStages(pipelineBuilder, pipeline);

		pipelineBuilder.Finish();

		Swap(_pipelineBuilder, pipelineBuilder);

		return Error::None;
	}

	// ---------------------------------------------------

	void GraphicsPipelineAsset::FreeResources() {}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
