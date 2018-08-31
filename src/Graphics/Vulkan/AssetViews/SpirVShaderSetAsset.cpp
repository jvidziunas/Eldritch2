/*==================================================================*\
  SpirVShaderSetAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/SpirVShaderSetAsset.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/SpirVShaderSet_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	using namespace ::Eldritch2::Graphics::Vulkan::FlatBuffers;
	using namespace ::Eldritch2::Graphics::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	namespace {

		ETInlineHint ETPureFunctionHint VkStencilOp ParseStencilOp(StencilOperation operation) {
			switch (operation) {
			case StencilOperation::KeepExisting: return VK_STENCIL_OP_KEEP;
			case StencilOperation::SetZero: return VK_STENCIL_OP_ZERO;
			case StencilOperation::ReplaceExisting: return VK_STENCIL_OP_REPLACE;
			case StencilOperation::IncrementWithSaturate: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			case StencilOperation::DecrementWithSaturate: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			case StencilOperation::InvertExisting: return VK_STENCIL_OP_INVERT;
			case StencilOperation::IncrementWithWrap: return VK_STENCIL_OP_INCREMENT_AND_WRAP;
			case StencilOperation::DecrementWithWrap:
				return VK_STENCIL_OP_DECREMENT_AND_WRAP;
				ET_NO_DEFAULT_CASE;
			}; // switch(operation)
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint VkCompareOp ParseComparisonOp(ComparisonFunction function) {
			switch (function) {
			case ComparisonFunction::NeverPass: return VK_COMPARE_OP_NEVER;
			case ComparisonFunction::PassIfLess: return VK_COMPARE_OP_LESS;
			case ComparisonFunction::PassIfEqual: return VK_COMPARE_OP_EQUAL;
			case ComparisonFunction::PassIfLessOrEqual: return VK_COMPARE_OP_LESS_OR_EQUAL;
			case ComparisonFunction::PassIfGreater: return VK_COMPARE_OP_GREATER;
			case ComparisonFunction::PassIfNotEqual: return VK_COMPARE_OP_NOT_EQUAL;
			case ComparisonFunction::PassIfGreaterOrEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case ComparisonFunction::AlwaysPass:
				return VK_COMPARE_OP_ALWAYS;
				ET_NO_DEFAULT_CASE;
			}; // switch(function)
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint VkBlendOp ParseBlendOp(BlendOperator operation) {
			switch (operation) {
			case BlendOperator::Add: return VK_BLEND_OP_ADD;
			case BlendOperator::Subtract: return VK_BLEND_OP_SUBTRACT;
			case BlendOperator::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
			case BlendOperator::Minimum: return VK_BLEND_OP_MIN;
			case BlendOperator::Maximum:
				return VK_BLEND_OP_MAX;
				ET_NO_DEFAULT_CASE;
			}; // switch(operation)
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint VkBlendFactor ParseBlendFactor(ColorBlendFactor factor) {
			switch (factor) {
			case ColorBlendFactor::Zero: return VK_BLEND_FACTOR_ZERO;
			case ColorBlendFactor::One: return VK_BLEND_FACTOR_ONE;
			case ColorBlendFactor::SourceColor: return VK_BLEND_FACTOR_SRC_COLOR;
			case ColorBlendFactor::OneMinusSourceColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case ColorBlendFactor::SourceAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
			case ColorBlendFactor::OneMinusSourceAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case ColorBlendFactor::DestinationAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
			case ColorBlendFactor::OneMinusDestinationAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case ColorBlendFactor::DestinationColor: return VK_BLEND_FACTOR_DST_COLOR;
			case ColorBlendFactor::OneMinusDestinationColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			case ColorBlendFactor::SaturatedSourceAlpha: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			case ColorBlendFactor::BlendFactor: return VK_BLEND_FACTOR_CONSTANT_COLOR;
			case ColorBlendFactor::OneMinusBlendFactor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			case ColorBlendFactor::BlendFactorAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case ColorBlendFactor::OneMinusBlendFactorAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
			case ColorBlendFactor::DualSourceColor: return VK_BLEND_FACTOR_SRC1_COLOR;
			case ColorBlendFactor::OneMinusDualSourceColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
			case ColorBlendFactor::DualSourceAlpha: return VK_BLEND_FACTOR_SRC1_ALPHA;
			case ColorBlendFactor::OneMinusDualSourceAlpha:
				return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
				ET_NO_DEFAULT_CASE;
			}; // switch(factor)
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint VkBlendFactor ParseBlendFactor(AlphaBlendFactor factor) {
			switch (factor) {
			case AlphaBlendFactor::Zero: return VK_BLEND_FACTOR_ZERO;
			case AlphaBlendFactor::One: return VK_BLEND_FACTOR_ONE;
			case AlphaBlendFactor::SourceAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
			case AlphaBlendFactor::OneMinusSourceAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case AlphaBlendFactor::DestinationAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
			case AlphaBlendFactor::OneMinusDestinationAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case AlphaBlendFactor::SaturatedSourceAlpha: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			case AlphaBlendFactor::BlendFactorAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case AlphaBlendFactor::OneMinusBlendFactorAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
			case AlphaBlendFactor::DualSourceAlpha: return VK_BLEND_FACTOR_SRC1_ALPHA;
			case AlphaBlendFactor::OneMinusDualSourceAlpha:
				return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
				ET_NO_DEFAULT_CASE;
			}; // switch(factor)
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint VkColorComponentFlags ParseWriteMask(ChannelWriteMask mask) {
			using FlagBits = eastl::underlying_type<ChannelWriteMask>::type;

			// ---

			VkColorComponentFlags flags(0u);

			if (FlagBits(mask) & FlagBits(ChannelWriteMask::Red)) {
				flags |= VK_COLOR_COMPONENT_R_BIT;
			}

			if (FlagBits(mask) & FlagBits(ChannelWriteMask::Green)) {
				flags |= VK_COLOR_COMPONENT_G_BIT;
			}

			if (FlagBits(mask) & FlagBits(ChannelWriteMask::Blue)) {
				flags |= VK_COLOR_COMPONENT_B_BIT;
			}

			if (FlagBits(mask) & FlagBits(ChannelWriteMask::Alpha)) {
				flags |= VK_COLOR_COMPONENT_A_BIT;
			}

			return flags;
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint VkPipelineColorBlendAttachmentState ParseColorBlendState(const OutputState* state) {
			return VkPipelineColorBlendAttachmentState {
				/*blendEnable =*/VK_TRUE,
				ParseBlendFactor(state->SourceColorBlendFactor()),
				ParseBlendFactor(state->DestinationColorBlendFactor()),
				ParseBlendOp(state->ColorBlendOperator()),
				ParseBlendFactor(state->SourceAlphaBlendFactor()),
				ParseBlendFactor(state->DestinationAlphaBlendFactor()),
				ParseBlendOp(state->AlphaBlendOperator()),
				ParseWriteMask(state->WriteMask())
			};
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint VkStencilOpState ParseStencilOpState(uint8 readMask, uint8 writeMask, uint8 stencilReference, const StencilBehavior* state) {
			return VkStencilOpState {
				ParseStencilOp(state->StencilAndDepthFailBehavior()),
				ParseStencilOp(state->StencilAndDepthPassBehavior()),
				ParseStencilOp(state->StencilPassDepthFailBehavior()),
				ParseComparisonOp(state->StencilComparisonFunction()),
				readMask,
				writeMask,
				stencilReference
			};
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint VkPolygonMode ParsePolygonMode(PolygonFillMode fill) {
			switch (fill) {
			case PolygonFillMode::Wireframe: return VK_POLYGON_MODE_LINE;
			case PolygonFillMode::Solid:
				return VK_POLYGON_MODE_FILL;
				ET_NO_DEFAULT_CASE;
			}; // switch(mode)
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint VkCullModeFlags ParseCullMode(FaceCullMode cull) {
			switch (cull) {
			case FaceCullMode::FrontFace: return VK_CULL_MODE_FRONT_BIT;
			case FaceCullMode::BackFace: return VK_CULL_MODE_BACK_BIT;
			case FaceCullMode::None:
				return VK_CULL_MODE_NONE;
				ET_NO_DEFAULT_CASE;
			}; // switch(mode)
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint VkPipelineDepthStencilStateCreateInfo ParseDepthStencilState(const DepthStencilState* state) {
			using FlagBits = eastl::underlying_type<DepthStencilFlags>::type;

			// ---

			return VkPipelineDepthStencilStateCreateInfo {
				VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0,
				VkBool32(FlagBits(state->Flags()) & FlagBits(DepthStencilFlags::DepthTestEnable) ? VK_TRUE : VK_FALSE),
				VkBool32(FlagBits(state->Flags()) & FlagBits(DepthStencilFlags::DepthWriteEnable) ? VK_TRUE : VK_FALSE),
				ParseComparisonOp(state->DepthComparisonFunction()),
				/*depthBoundsTestEnable =*/VK_FALSE,
				VkBool32(FlagBits(state->Flags()) & FlagBits(DepthStencilFlags::StencilTestEnable) ? VK_TRUE : VK_FALSE),
				ParseStencilOpState(0xFF, 0xFF, 0xFF, state->FrontFaceBehavior()),
				ParseStencilOpState(0xFF, 0xFF, 0xFF, state->BackFaceBehavior()),
				/*minDepthBounds =*/0.0f,
				/*maxDepthBounds =*/1.0f
			};
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint VkPipelineRasterizationStateCreateInfo ParseRasterizationState(const RasterizationState* state) {
			return VkPipelineRasterizationStateCreateInfo {
				VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0,
				/*depthClampEnable =*/VK_FALSE,
				/*rasterizerDiscardEnable =*/VK_FALSE,
				ParsePolygonMode(state->FillMode()),
				ParseCullMode(state->CullMode()),
				VK_FRONT_FACE_CLOCKWISE,
				VkBool32(state->DepthBias() != 0 ? VK_TRUE : VK_FALSE),
				static_cast<const float&>(state->DepthBias()),
				state->DepthBiasClamp(),
				state->SlopeScaledDepthBias(),
				/*lineWidth =*/1.0f
			};
		}

		// ---------------------------------------------------

		ETInlineHint void ParseBytecode(ArrayList<uint32>& bytecode, const flatbuffers::Vector<uint32>* spirv) {
			bytecode.Assign(spirv->data(), spirv->data() + spirv->size());
		}

		// ---------------------------------------------------

		ETInlineHint void ParseUsage(ArrayList<SpirVShaderSetAsset::Usage>& usages, const UsageDescriptor* usage) {
			// Typedef here because compiler has a hard time dealing with array literals.
			using BlendList = VkPipelineColorBlendAttachmentState[SpirVShaderSetAsset::Usage::MaxColorAttachments];

			static ETConstexpr VkPipelineColorBlendAttachmentState BlendDisabled { /*blendEnable =*/VK_FALSE };

			const uint32 blendCount(usage->Outputs()->size());
			usages.EmplaceBack(
				BlendList {
					blendCount > 0u ? ParseColorBlendState(usage->Outputs()->Get(0)) : BlendDisabled,
					blendCount > 1u ? ParseColorBlendState(usage->Outputs()->Get(1)) : BlendDisabled,
					blendCount > 2u ? ParseColorBlendState(usage->Outputs()->Get(2)) : BlendDisabled,
					blendCount > 3u ? ParseColorBlendState(usage->Outputs()->Get(3)) : BlendDisabled },
				VK_SHADER_STAGE_ALL_GRAPHICS,
				VkPipelineMultisampleStateCreateInfo {},
				ParseRasterizationState(usage->RasterizationBehavior()),
				ParseDepthStencilState(usage->DepthStencilBehavior()));
		}

		// ---------------------------------------------------

		ETInlineHint void ParseUsages(ArrayList<SpirVShaderSetAsset::Usage>& usages, CachingHashMap<String, uint32>& indexByName, const flatbuffers::Vector<Offset<UsageDescriptor>>* assets) {
			for (uint32 index(0u); index < assets->size(); ++index) {
				const UsageDescriptor* usage(assets->Get(index));

				indexByName.Emplace(String(MallocAllocator("Usage Name Allocator"), StringView(usage->Name()->c_str(), usage->Name()->size())), index);
				ParseUsage(usages, usage);
			}
		}

	} // anonymous namespace

	SpirVShaderSetAsset::Usage::Usage(
		const VkPipelineColorBlendAttachmentState (&blends)[MaxColorAttachments],
		VkShaderStageFlagBits                         enabledShaders,
		const VkPipelineMultisampleStateCreateInfo&   multisampleInfo,
		const VkPipelineRasterizationStateCreateInfo& rasterizationInfo,
		const VkPipelineDepthStencilStateCreateInfo&  depthStencilInfo) :
		enabledShaders(enabledShaders),
		multisampleInfo(multisampleInfo),
		rasterizationInfo(rasterizationInfo),
		depthStencilInfo(depthStencilInfo) {
		CopyArrayNonTemporal(this->blends, blends);
	}

	// ---------------------------------------------------

	SpirVShaderSetAsset::SpirVShaderSetAsset(StringView path) :
		Asset(path),
		_bytecode(MallocAllocator("SPIR-V Shader Package Bytecode Allocator")),
		_usages(MallocAllocator("SPIR-V Shader Package Usage List Allocator")),
		_indexByName(MallocAllocator("SPIR-V Shader Package Usage Name Table Allocator")) {}

	// ---------------------------------------------------

	ErrorCode SpirVShaderSetAsset::BindResources(const Builder& asset) {
		//	Ensure we're working with data that can plausibly represent a shader package.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifySpirVShaderSetBuffer(verifier)) {
			asset.WriteLog(Severity::Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		ArrayList<uint32> bytecode(_bytecode.GetAllocator());
		ParseBytecode(bytecode, GetSpirVShaderSet(asset.Begin())->Bytecode());

		ArrayList<Usage>               usages(_usages.GetAllocator());
		CachingHashMap<String, uint32> indexByName(_indexByName.GetAllocator());
		ParseUsages(usages, indexByName, GetSpirVShaderSet(asset.Begin())->Usages());

		Swap(_bytecode, bytecode);
		Swap(_usages, usages);
		Swap(_indexByName, indexByName);

		return Error::None;
	}

	// ---------------------------------------------------

	void SpirVShaderSetAsset::FreeResources() {
		_indexByName.Clear();
		_usages.Clear();
		_bytecode.Clear();
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringView SpirVShaderSetAsset::GetExtension() ETNoexceptHint {
		return { SpirVShaderSetExtension(), StringLength(SpirVShaderSetExtension()) };
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
