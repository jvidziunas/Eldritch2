/*==================================================================*\
  GraphicsPipelineAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/GraphicsPipelineAsset.hpp>
#include <Graphics/PipelineOptimizations.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Flatbuffers/FlatbufferTools.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/GraphicsPipeline_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	Result GraphicsPipelineAsset::BindResources(Log& log, const AssetBuilder& asset) {
		using namespace ::Eldritch2::Graphics::FlatBuffers;
		using namespace ::flatbuffers;

		static ETConstexpr auto ParseDimensions([](const StaticDimensions* asset) ETNoexceptHint { return StageResolution(asset->Width(), asset->Height(), asset->Depth()); });
		static ETConstexpr auto ParseDynamicDimensions([](const DynamicDimensions* asset) ETNoexceptHint { return StageResolution(asset->Width(), asset->Height()); });
		static ETConstexpr auto ParseAttachment([](const AttachmentReference* asset) ETNoexceptHint { return StageAttachment(asset->GlobalIndex(), asset->Mip(), asset->FirstSlice(), asset->LastSlice()); });

		static ETConstexpr auto ParseSharedAttachments([](PipelineBuilder& builder, const flatbuffers::Vector<Offset<AttachmentDescriptor>>* assets) -> Result {
			const auto ParseSharedAttachment([&](const AttachmentDescriptor* asset) -> Result {
				return builder.DefineSharedAttachment(asset->Format(), asset->SmallestMip(), (asset->Flags() & AttachmentFlags::Persistent) != AttachmentFlags::NONE);
			});

			// ---

			for (uoffset_t attachment(0u), end(assets->size()); attachment < end; ++attachment) {
				ET_ABORT_UNLESS(ParseSharedAttachment(assets->Get(attachment)));
			}

			return Result::Success;
		});

		static ETConstexpr auto ParseStages([](PipelineBuilder& builder, const flatbuffers::Vector<Offset<void>>* stages, const flatbuffers::Vector<uint8>* stageTypes) ETNoexceptHint {
			const auto ParseDrawStage([&](uoffset_t /*stage*/, const DrawStage* stage) ETNoexceptHint -> Result {
				const StringSpan shader(AsString(stage->ShaderUsageName()));
				const uint32     samplesPerPixel(1u);
				const auto       firstAttachment(ETStackAlloc(StageAttachment, Maximum(stage->InputAttachments()->size() + stage->ColorAttachments()->size(), 1u)));
				const auto       firstOutput(firstAttachment + stage->InputAttachments()->size());
				const auto       inputs(Span<const StageAttachment*>(firstAttachment, Transform(stage->InputAttachments()->begin(), stage->InputAttachments()->end(), firstAttachment, ParseAttachment)));
				const auto       outputs(Span<const StageAttachment*>(firstOutput, Transform(stage->ColorAttachments()->begin(), stage->ColorAttachments()->end(), firstOutput, ParseAttachment)));

				switch (stage->Dimensions_type()) {
				default: return Result::InvalidParameter;
				case PassDimensions::DynamicDimensions: return builder.DefineDrawStage(shader, ParseDynamicDimensions(stage->Dimensions_as<DynamicDimensions>()), samplesPerPixel, inputs, outputs);
				case PassDimensions::StaticDimensions: return builder.DefineDrawStage(shader, ParseDimensions(stage->Dimensions_as<StaticDimensions>()), samplesPerPixel, inputs, outputs);
				};
			});

			const auto ParseCopyStage([&](uoffset_t /*stage*/, const CopyStage* /*stage*/) ETNoexceptHint -> Result {
				const StringSpan shader("Copy");
				return builder.DefineCopyStage(shader, {}, {});
			});

			const auto ParseComputeStage([&](uoffset_t /*stage*/, const ComputeStage* stage) ETNoexceptHint -> Result {
				const StringSpan shader(AsString(stage->ShaderName()));
				const auto       firstAttachment(ETStackAlloc(StageAttachment, Maximum(stage->Attachments()->size(), 1u)));
				const auto       attachments(Span<const StageAttachment*>(firstAttachment, Transform(stage->Attachments()->begin(), stage->Attachments()->end(), firstAttachment, ParseAttachment)));

				switch (stage->Dimensions_type()) {
				default: return Result::InvalidParameter;
				case PassDimensions::DynamicDimensions: return builder.DefineComputeStage(shader, ParseDynamicDimensions(stage->Dimensions_as<DynamicDimensions>()), attachments);
				case PassDimensions::StaticDimensions: return builder.DefineComputeStage(shader, ParseDimensions(stage->Dimensions_as<StaticDimensions>()), attachments);
				};
			});

			// ---

			for (uoffset_t stage(0u); stage < stages->size(); ++stage) {
				switch (stageTypes->GetEnum<PipelineStage>(stage)) {
				default: break;
				case PipelineStage::DrawStage: ET_ABORT_UNLESS(ParseDrawStage(stage, stages->GetAs<DrawStage>(stage))); break;
				case PipelineStage::CopyStage: ET_ABORT_UNLESS(ParseCopyStage(stage, stages->GetAs<CopyStage>(stage))); break;
				case PipelineStage::ComputeStage: ET_ABORT_UNLESS(ParseComputeStage(stage, stages->GetAs<ComputeStage>(stage))); break;
				} // switch (assetTypes->GetEnum<PipelineStage>(stage))
			}

			return Result::Success;
		});

		// ---

		//	Ensure we're working with data that can plausibly represent a graphics pipeline.
		const auto pipeline(GetVerifiedRoot<GraphicsPipelineDescriptor>(asset.bytes, GraphicsPipelineDescriptorIdentifier()));
		ET_ABORT_UNLESS(pipeline ? Result::Success : Result::InvalidParameter, log.Write(Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath()));

		PipelineBuilder builder;
		ET_ABORT_UNLESS(ParseSharedAttachments(builder, pipeline->Attachments()));
		ET_ABORT_UNLESS(ParseStages(builder, pipeline->Stages(), pipeline->Stages_type()));

		Swap(*this, builder.Optimize(StripUnusedResourcesPass(), OptimizeAttachmentBandwidthPass()));

		return Result::Success;
	}

	// ---------------------------------------------------

	void GraphicsPipelineAsset::FreeResources() ETNoexceptHint {
		PipelineBuilder empty;
		Swap(*this, empty.Optimize(/*passes =*/));
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringSpan GraphicsPipelineAsset::GetExtension() ETNoexceptHint {
		using namespace ::Eldritch2::Graphics::FlatBuffers;

		return { GraphicsPipelineDescriptorExtension(), StringSpan::SizeType(StringLength(GraphicsPipelineDescriptorExtension())) };
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
