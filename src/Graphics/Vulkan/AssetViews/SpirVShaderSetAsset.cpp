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
	namespace {

		using namespace ::Eldritch2::Graphics::Vulkan::FlatBuffers;
		using namespace ::Eldritch2::Graphics::FlatBuffers;

		static ETInlineHint ETPureFunctionHint VkStencilOp AsStencilOp(StencilOperation operation) {
			switch (operation) {
			case StencilOperation::KeepExisting: return VK_STENCIL_OP_KEEP;
			case StencilOperation::SetZero: return VK_STENCIL_OP_ZERO;
			case StencilOperation::ReplaceExisting: return VK_STENCIL_OP_REPLACE;
			case StencilOperation::IncrementAndSaturate: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			case StencilOperation::DecrementAndSaturate: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			case StencilOperation::InvertExisting: return VK_STENCIL_OP_INVERT;
			case StencilOperation::IncrementWithWrap: return VK_STENCIL_OP_INCREMENT_AND_WRAP;
			case StencilOperation::DecrementWithWrap:
				return VK_STENCIL_OP_DECREMENT_AND_WRAP;
				ET_NO_DEFAULT_CASE;
			}; // switch( operation )
		}

		// ---------------------------------------------------

		static ETInlineHint ETPureFunctionHint VkCompareOp AsComparisonOp(ComparisonFunction function) {
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
			};
		}

		// ---------------------------------------------------

		static ETInlineHint ETPureFunctionHint VkBlendOp AsBlendOp(BlendOperator operation) {
			switch (operation) {
			case BlendOperator::Add: return VK_BLEND_OP_ADD;
			case BlendOperator::Subtract: return VK_BLEND_OP_SUBTRACT;
			case BlendOperator::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
			case BlendOperator::Minimum: return VK_BLEND_OP_MIN;
			case BlendOperator::Maximum:
				return VK_BLEND_OP_MAX;
				ET_NO_DEFAULT_CASE;
			}; // switch( operation )
		}

		// ---------------------------------------------------

		static ETInlineHint ETPureFunctionHint VkBlendFactor AsBlendFactor(ColorBlendFactor factor) {
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
			}; // switch( factor )
		}

		// ---------------------------------------------------

		static ETInlineHint ETPureFunctionHint VkBlendFactor AsBlendFactor(AlphaBlendFactor factor) {
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
			}; // switch( factor )
		}

		// ---------------------------------------------------

		static ETInlineHint ETPureFunctionHint VkColorComponentFlags AsWriteMask(ChannelWriteMask mask) {
			using FlagBits = eastl::underlying_type<ChannelWriteMask>::type;

			// ---

			VkColorComponentFlags flags(0u);

			if (static_cast<FlagBits>(mask) & static_cast<FlagBits>(ChannelWriteMask::Red)) {
				flags |= VK_COLOR_COMPONENT_R_BIT;
			}

			if (static_cast<FlagBits>(mask) & static_cast<FlagBits>(ChannelWriteMask::Green)) {
				flags |= VK_COLOR_COMPONENT_G_BIT;
			}

			if (static_cast<FlagBits>(mask) & static_cast<FlagBits>(ChannelWriteMask::Blue)) {
				flags |= VK_COLOR_COMPONENT_B_BIT;
			}

			if (static_cast<FlagBits>(mask) & static_cast<FlagBits>(ChannelWriteMask::Alpha)) {
				flags |= VK_COLOR_COMPONENT_A_BIT;
			}

			return flags;
		}

		// ---------------------------------------------------

		static ETInlineHint ETPureFunctionHint VkPipelineColorBlendAttachmentState AsColorBlendState(const OutputState* definition) {
			return VkPipelineColorBlendAttachmentState{
				/*blendEnable =*/VK_TRUE,
				AsBlendFactor(definition->SourceColorBlendFactor()),
				AsBlendFactor(definition->DestinationColorBlendFactor()),
				AsBlendOp(definition->ColorBlendOperator()),
				AsBlendFactor(definition->SourceAlphaBlendFactor()),
				AsBlendFactor(definition->DestinationAlphaBlendFactor()),
				AsBlendOp(definition->AlphaBlendOperator()),
				AsWriteMask(definition->WriteMask())
			};
		}

		// ---------------------------------------------------

		static ETInlineHint ETPureFunctionHint VkStencilOpState AsStencilOpState(uint8 readMask, uint8 writeMask, uint8 stencilReference, const StencilBehavior* const definition) {
			return VkStencilOpState{
				AsStencilOp(definition->StencilAndDepthFailBehavior()),
				AsStencilOp(definition->StencilAndDepthPassBehavior()),
				AsStencilOp(definition->StencilPassDepthFailBehavior()),
				AsComparisonOp(definition->StencilComparisonFunction()),
				readMask,
				writeMask,
				stencilReference
			};
		}

		// ---------------------------------------------------

		static ETInlineHint ETPureFunctionHint VkPolygonMode AsPolygonMode(PolygonFillMode mode) {
			switch (mode) {
			case PolygonFillMode::Wireframe: return VK_POLYGON_MODE_LINE;
			case PolygonFillMode::Solid:
				return VK_POLYGON_MODE_FILL;
				ET_NO_DEFAULT_CASE;
			}; // switch( mode )
		}

		// ---------------------------------------------------

		static ETInlineHint ETPureFunctionHint VkCullModeFlags AsCullMode(FaceCullMode mode) {
			switch (mode) {
			case FaceCullMode::FrontFace: return VK_CULL_MODE_FRONT_BIT;
			case FaceCullMode::BackFace: return VK_CULL_MODE_BACK_BIT;
			case FaceCullMode::None:
				return VK_CULL_MODE_NONE;
				ET_NO_DEFAULT_CASE;
			}; // switch( mode )
		}

		// ---------------------------------------------------

		static ETInlineHint ETPureFunctionHint VkPipelineDepthStencilStateCreateInfo ParseDepthStencilState(const DepthStencilState* const definition) {
			using FlagBits = eastl::underlying_type<DepthStencilFlags>::type;

			// ---

			return VkPipelineDepthStencilStateCreateInfo{
				VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0,
				VkBool32(static_cast<FlagBits>(definition->Flags()) & static_cast<FlagBits>(DepthStencilFlags::DepthTestEnable) ? VK_TRUE : VK_FALSE),
				VkBool32(static_cast<FlagBits>(definition->Flags()) & static_cast<FlagBits>(DepthStencilFlags::DepthWriteEnable) ? VK_TRUE : VK_FALSE),
				AsComparisonOp(definition->DepthComparisonFunction()),
				/*depthBoundsTestEnable =*/VK_FALSE,
				VkBool32(static_cast<FlagBits>(definition->Flags()) & static_cast<FlagBits>(DepthStencilFlags::StencilTestEnable) ? VK_TRUE : VK_FALSE),
				AsStencilOpState(0xFF, 0xFF, 0xFF, definition->FrontFaceBehavior()),
				AsStencilOpState(0xFF, 0xFF, 0xFF, definition->BackFaceBehavior()),
				/*minDepthBounds =*/0.0f,
				/*maxDepthBounds =*/1.0f
			};
		}

		// ---------------------------------------------------

		static ETInlineHint ETPureFunctionHint VkPipelineRasterizationStateCreateInfo ParseRasterizationState(const RasterizationState* const definition) {
			return VkPipelineRasterizationStateCreateInfo{
				VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0,
				/*depthClampEnable =*/VK_FALSE,
				/*rasterizerDiscardEnable =*/VK_FALSE, //
				AsPolygonMode(definition->FillMode()),
				AsCullMode(definition->CullMode()),
				VK_FRONT_FACE_CLOCKWISE,
				VkBool32(definition->DepthBias() != 0 ? VK_TRUE : VK_FALSE),
				static_cast<const float&>(definition->DepthBias()),
				definition->DepthBiasClamp(),
				definition->SlopeScaledDepthBias(),
				/*lineWidth =*/1.0f
			};
		}

	} // anonymous namespace

	using namespace ::Eldritch2::Graphics::Vulkan::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	SpirVShaderSetAsset::UsageAsset::UsageAsset(
		const Utf8Char* name,
		const VkPipelineColorBlendAttachmentState (&blends)[MaxColorAttachments],
		VkShaderStageFlagBits                         enabledShaders,
		const VkPipelineMultisampleStateCreateInfo&   multisampleInfo,
		const VkPipelineRasterizationStateCreateInfo& rasterizationInfo,
		const VkPipelineDepthStencilStateCreateInfo&  depthStencilInfo) :
		enabledShaders(enabledShaders),
		multisampleInfo(multisampleInfo),
		rasterizationInfo(rasterizationInfo),
		depthStencilInfo(depthStencilInfo) {
		CopyString(this->name, name, _countof(this->name));
		CopyArrayNonTemporal(this->blends, blends);
	}

	// ---------------------------------------------------

	SpirVShaderSetAsset::SpirVShaderSetAsset(const Utf8Char* const path) :
		Asset(path),
		_usages(MallocAllocator("SPIR-V Shader Package Usage Set Allocator")),
		_bytecode(MallocAllocator("SPIR-V Shader Package Bytecode Allocator")) {}

	// ---------------------------------------------------

	ErrorCode SpirVShaderSetAsset::BindResources(const Builder& asset) {
		//	Ensure we're working with data that can plausibly represent a shader package.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifySpirVShaderSetBuffer(verifier)) {
			asset.WriteLog(MessageType::Error, "Data integrity check failed for {}, aborting load." UTF8_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		const SpirVShaderSet* set(GetSpirVShaderSet(asset.Begin()));

		ArrayList<uint32> bytecode(_bytecode.GetAllocator());
		bytecode.Resize(set->Bytecode()->size());
		CopyArrayNonTemporal(bytecode.GetData(), set->Bytecode()->data(), bytecode.GetSize());

		UsageSet usages(_usages.GetAllocator());
		for (const UsageDescriptor* usage : *set->Usages()) {
			VkPipelineColorBlendAttachmentState colorBlendStates[UsageAsset::MaxColorAttachments];
			usages.Emplace(
				usage->Name()->c_str(),
				colorBlendStates,
				VK_SHADER_STAGE_ALL_GRAPHICS,
				VkPipelineMultisampleStateCreateInfo{},
				ParseRasterizationState(usage->RasterizationBehavior()),
				ParseDepthStencilState(usage->DepthStencilBehavior()));
		}

		Swap(_bytecode, bytecode);
		Swap(_usages, usages);

		return Error::None;
	}

	// ---------------------------------------------------

	void SpirVShaderSetAsset::FreeResources() {
		_usages.Clear();
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
