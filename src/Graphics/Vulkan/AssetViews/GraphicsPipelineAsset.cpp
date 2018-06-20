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

		ETInlineHint ETPureFunctionHint void ParseImageAttachment(GraphicsPipelineBuilder& builder, VkFormat format, bool allowMsaa, const Dynamic* size) {
			builder.DefineAttachment(format, allowMsaa, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL, size->Width(), size->Height(), size->Depth());
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint void ParseImageAttachment(GraphicsPipelineBuilder& builder, VkFormat format, bool allowMsaa, const Static* size) {
			builder.DefineAttachment(format, allowMsaa, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL, VkExtent3D{ size->Width(), size->Height(), size->Depth() });
		}

		// ---------------------------------------------------

		ETInlineHint void ParseAttachments(GraphicsPipelineBuilder& builder, const GraphicsPipeline* pipeline) {
			const auto attachments(pipeline->Attachments());
			for (uoffset_t id(0u); id < attachments->size(); ++id) {
				const auto attachment(attachments->Get(id));

				const VkSampleCountFlags msaaQuality(VK_SAMPLE_COUNT_1_BIT);
				const VkFormat           format(VK_FORMAT_UNDEFINED);

				switch (attachment->Size_type()) {
				case AttachmentSize::NONE: break;
				case AttachmentSize::Dynamic: ParseImageAttachment(builder, format, msaaQuality, attachment->Size_as<Dynamic>()); break;
				case AttachmentSize::Static: ParseImageAttachment(builder, format, msaaQuality, attachment->Size_as<Static>()); break;
				} // switch (attachment->Size_type())
			}
		}

		// ---------------------------------------------------

		ETInlineHint ErrorCode ParseStage(GraphicsPipelineBuilder& builder, uoffset_t stageId, const DrawStage* stage) {
			builder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, stage->ShaderUsageName()->c_str());

			for (uint32 id : *stage->InputAttachments()) {
				builder.AttachInput(id, VK_IMAGE_LAYOUT_GENERAL);
				/*	if (images.GetSize() <= id) {
					asset.WriteLog(MessageType::Error, "Graphics pipeline {} references out-of-bounds input attachment {} in pass {}!" UTF8_NEWLINE, GetPath(), id, pass);
					return Error::InvalidParameter;
				}*/
			}

			for (uint32 id : *stage->ColorAttachments()) {
				builder.AttachColorOutput(id, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
				/*	if (images.GetSize() <= id) {
					asset.WriteLog(MessageType::Error, "Graphics pipeline {} references out-of-bounds color attachment {} in pass {}!" UTF8_NEWLINE, GetPath(), id, pass);
					return Error::InvalidParameter;
				}
				*/
			}

			if (stage->DepthAttachment() != VK_ATTACHMENT_UNUSED) {
				const uint32 id(stage->DepthAttachment());
				builder.AttachDepthStencilBuffer(id, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
				/*	if (images.GetSize() <= id) {
					asset.WriteLog(MessageType::Error, "Graphics pipeline {} references out-of-bounds depth/stencil attachment {} in pass {}!" UTF8_NEWLINE, GetPath(), id, pass);
					return Error::InvalidParameter;
				}
				*/
			}

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ErrorCode ParseStage(GraphicsPipelineBuilder& builder, uoffset_t stageId, const CopyStage* stage) {
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

			builder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, "Copy");
			builder.AttachInput(source, VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
			builder.AttachInput(target, VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ErrorCode ParseStage(GraphicsPipelineBuilder& builder, uoffset_t stageId, const ComputeStage* stage) {
			builder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, stage->ShaderName()->c_str());

			for (uint32 attachment : *stage->Attachments()) {
				builder.AttachInput(attachment, VK_IMAGE_LAYOUT_GENERAL);
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
				case PipelineStage::NONE:
				default: return Error::InvalidParameter;
				case PipelineStage::DrawStage: ET_FAIL_UNLESS(ParseStage(builder, stage, stages->GetAs<DrawStage>(stage))); break;
				case PipelineStage::CopyStage: ET_FAIL_UNLESS(ParseStage(builder, stage, stages->GetAs<CopyStage>(stage))); break;
				case PipelineStage::ComputeStage: ET_FAIL_UNLESS(ParseStage(builder, stage, stages->GetAs<ComputeStage>(stage))); break;
				} // switch (types->GetEnum<GraphicsStage>(stage))
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

		GraphicsPipelineBuilder pipelineBuilder;

		ParseAttachments(pipelineBuilder, pipeline);
		ParseStages(pipelineBuilder, pipeline);

		pipelineBuilder.Finish();

		Swap(_pipelineBuilder, pipelineBuilder);

		return Error::None;
	}

	// ---------------------------------------------------

	void GraphicsPipelineAsset::FreeResources() {
		_pipelineBuilder.Clear();
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
