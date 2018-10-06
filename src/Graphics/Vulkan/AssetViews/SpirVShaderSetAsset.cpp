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
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Flatbuffers/FlatBufferTools.hpp>
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

		template <typename BitField>
		ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint bool IsFlagEnabled(BitField field, BitField flags) ETNoexceptHint {
			using FlagBits = typename eastl::underlying_type<BitField>::type;

			// ---

			return FlagBits(field) & FlagBits(flags) == FlagBits(flags);
		}

		// ---------------------------------------------------

		ETCpp14Constexpr ETInlineHint ETForceInlineHint ETPureFunctionHint VkStencilOp ParseStencilOp(StencilOperation operation) ETNoexceptHint {
			switch (operation) {
			default:
			case StencilOperation::KeepExisting: return VK_STENCIL_OP_KEEP;
			case StencilOperation::SetZero: return VK_STENCIL_OP_ZERO;
			case StencilOperation::ReplaceExisting: return VK_STENCIL_OP_REPLACE;
			case StencilOperation::IncrementWithSaturate: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			case StencilOperation::DecrementWithSaturate: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			case StencilOperation::InvertExisting: return VK_STENCIL_OP_INVERT;
			case StencilOperation::IncrementWithWrap: return VK_STENCIL_OP_INCREMENT_AND_WRAP;
			case StencilOperation::DecrementWithWrap: return VK_STENCIL_OP_DECREMENT_AND_WRAP;
			}; // switch(operation)
		}

		// ---------------------------------------------------

		ETCpp14Constexpr ETInlineHint ETForceInlineHint ETPureFunctionHint VkCompareOp ParseComparisonOp(ComparisonFunction function) ETNoexceptHint {
			switch (function) {
			default:
			case ComparisonFunction::NeverPass: return VK_COMPARE_OP_NEVER;
			case ComparisonFunction::PassIfLess: return VK_COMPARE_OP_LESS;
			case ComparisonFunction::PassIfEqual: return VK_COMPARE_OP_EQUAL;
			case ComparisonFunction::PassIfLessOrEqual: return VK_COMPARE_OP_LESS_OR_EQUAL;
			case ComparisonFunction::PassIfGreater: return VK_COMPARE_OP_GREATER;
			case ComparisonFunction::PassIfNotEqual: return VK_COMPARE_OP_NOT_EQUAL;
			case ComparisonFunction::PassIfGreaterOrEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case ComparisonFunction::AlwaysPass: return VK_COMPARE_OP_ALWAYS;
			}; // switch(function)
		}

		// ---------------------------------------------------

		ETCpp14Constexpr ETInlineHint ETForceInlineHint ETPureFunctionHint VkBlendOp ParseBlendOp(BlendOperator operation) ETNoexceptHint {
			switch (operation) {
			default:
			case BlendOperator::Add: return VK_BLEND_OP_ADD;
			case BlendOperator::Subtract: return VK_BLEND_OP_SUBTRACT;
			case BlendOperator::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
			case BlendOperator::Minimum: return VK_BLEND_OP_MIN;
			case BlendOperator::Maximum: return VK_BLEND_OP_MAX;
			}; // switch(operation)
		}

		// ---------------------------------------------------

		ETCpp14Constexpr ETInlineHint ETForceInlineHint ETPureFunctionHint VkBlendFactor ParseBlendFactor(ColorBlendFactor factor) ETNoexceptHint {
			switch (factor) {
			default:
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
			case ColorBlendFactor::OneMinusDualSourceAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
			}; // switch(factor)
		}

		// ---------------------------------------------------

		ETCpp14Constexpr ETInlineHint ETForceInlineHint ETPureFunctionHint VkBlendFactor ParseBlendFactor(AlphaBlendFactor factor) ETNoexceptHint {
			switch (factor) {
			default:
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
			case AlphaBlendFactor::OneMinusDualSourceAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
			}; // switch(factor)
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ETPureFunctionHint VkColorComponentFlags ParseWriteMask(ChannelWriteMask channels) ETNoexceptHint {
			return IsFlagEnabled(channels, ChannelWriteMask::Red) ? VK_COLOR_COMPONENT_R_BIT : 0u // clang-format off
				| IsFlagEnabled(channels, ChannelWriteMask::Green) ? VK_COLOR_COMPONENT_G_BIT : 0u
				| IsFlagEnabled(channels, ChannelWriteMask::Blue) ? VK_COLOR_COMPONENT_B_BIT : 0u
				| IsFlagEnabled(channels, ChannelWriteMask::Alpha) ? VK_COLOR_COMPONENT_A_BIT : 0u; // clang-format on
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ETPureFunctionHint VkPipelineColorBlendAttachmentState ParseColorBlendState(const OutputState* asset) ETNoexceptHint {
			return VkPipelineColorBlendAttachmentState {
				/*blendEnable =*/VK_TRUE,
				ParseBlendFactor(asset->SourceColorBlendFactor()),
				ParseBlendFactor(asset->DestinationColorBlendFactor()),
				ParseBlendOp(asset->ColorBlendOperator()),
				ParseBlendFactor(asset->SourceAlphaBlendFactor()),
				ParseBlendFactor(asset->DestinationAlphaBlendFactor()),
				ParseBlendOp(asset->AlphaBlendOperator()),
				ParseWriteMask(asset->WriteMask())
			};
		}

		// ---------------------------------------------------

		ETCpp14Constexpr ETInlineHint ETForceInlineHint ETPureFunctionHint VkStencilOpState ParseStencilOpState(uint8 readMask, uint8 writeMask, uint8 reference, const StencilBehavior* asset) ETNoexceptHint {
			return VkStencilOpState {
				ParseStencilOp(asset->StencilAndDepthFailBehavior()),
				ParseStencilOp(asset->StencilAndDepthPassBehavior()),
				ParseStencilOp(asset->StencilPassDepthFailBehavior()),
				ParseComparisonOp(asset->StencilComparisonFunction()),
				readMask,
				writeMask,
				reference
			};
		}

		// ---------------------------------------------------

		ETCpp14Constexpr ETInlineHint ETForceInlineHint ETPureFunctionHint VkPolygonMode ParsePolygonMode(PolygonFillMode fill) ETNoexceptHint {
			switch (fill) {
			default:
			case PolygonFillMode::Solid: return VK_POLYGON_MODE_FILL;
			case PolygonFillMode::Wireframe: return VK_POLYGON_MODE_LINE;
			}; // switch(mode)
		}

		// ---------------------------------------------------

		ETCpp14Constexpr ETInlineHint ETForceInlineHint ETPureFunctionHint VkCullModeFlags ParseCullMode(FaceCullMode cull) ETNoexceptHint {
			switch (cull) {
			default:
			case FaceCullMode::None: return VK_CULL_MODE_NONE;
			case FaceCullMode::FrontFace: return VK_CULL_MODE_FRONT_BIT;
			case FaceCullMode::BackFace: return VK_CULL_MODE_BACK_BIT;
			}; // switch(mode)
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ETPureFunctionHint VkResult ParseDepthStencilState(SpirVShader& shader, const DepthStencilState* asset) ETNoexceptHint {
			using FlagBits = eastl::underlying_type<DepthStencilFlags>::type;

			// ---

			shader.depthStencil.flags                = 0u;
			shader.depthStencil.shouldTest           = IsFlagEnabled(asset->Flags(), DepthStencilFlags::DepthTestEnable) ? VK_TRUE : VK_FALSE;
			shader.depthStencil.shouldWrite          = IsFlagEnabled(asset->Flags(), DepthStencilFlags::DepthWriteEnable) ? VK_TRUE : VK_FALSE;
			shader.depthStencil.depthOperator        = ParseComparisonOp(asset->DepthComparisonFunction());
			shader.depthStencil.shouldClipBounds     = VK_FALSE;
			shader.depthStencil.shouldTestStencil    = FlagBits(asset->Flags()) & FlagBits(DepthStencilFlags::StencilTestEnable) ? VK_TRUE : VK_FALSE;
			shader.depthStencil.frontStencilOperator = ParseStencilOpState(0xFF, 0xFF, 0xFF, asset->FrontFaceBehavior());
			shader.depthStencil.backStencilOperator  = ParseStencilOpState(0xFF, 0xFF, 0xFF, asset->BackFaceBehavior());
			shader.depthStencil.minDepthBounds       = 0.0f;
			shader.depthStencil.maxDepthBounds       = 1.0f;

			return VK_SUCCESS;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ETPureFunctionHint VkResult ParseRasterizationState(SpirVShader& shader, const RasterizationState* asset) ETNoexceptHint {
			shader.rasterizer.flags               = 0,
			shader.rasterizer.shouldClampDepth    = VK_FALSE;
			shader.rasterizer.shouldDiscardOutput = VK_FALSE;
			shader.rasterizer.fill                = ParsePolygonMode(asset->FillMode());
			shader.rasterizer.cullMode            = ParseCullMode(asset->CullMode());
			shader.rasterizer.frontFace           = VK_FRONT_FACE_CLOCKWISE;
			shader.rasterizer.depthBias           = AsFloatBits(asset->DepthBias());
			shader.rasterizer.depthBiasClamp      = asset->DepthBiasClamp();
			shader.rasterizer.slopeDepthBias      = asset->SlopeScaledDepthBias();

			return VK_SUCCESS;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint VkResult ParseOutputStates(SpirVShader& shader, const FlatbufferVector<Offset<OutputState>>* assets) ETNoexceptHint {
			if (assets->size() > ETCountOf(shader.blending.attachments)) {
				return VK_ERROR_FEATURE_NOT_PRESENT;
			}

			Transform(assets->begin(), assets->end(), shader.blending.attachments, [](const OutputState* asset) { return ParseColorBlendState(asset); });
			return VK_SUCCESS;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint void ParseBytecode(ArrayList<uint32>& bytecode, const FlatbufferVector<uint32>* asset) {
			bytecode.Assign(asset->data(), asset->data() + asset->size());
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint VkResult ParseShader(SpirVShader& shader, Range<const uint32*> bytecode, const Shader* asset) ETNoexceptHint {
			ET_ABORT_UNLESS(ParseRasterizationState(shader, asset->RasterizationBehavior()));
			ET_ABORT_UNLESS(ParseOutputStates(shader, asset->Outputs()));
			ET_ABORT_UNLESS(ParseDepthStencilState(shader, asset->DepthStencilBehavior()));

			return VK_SUCCESS;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint VkResult ParseShaders(SpirVShaderSetAsset::ContainerType& shaders, Range<const uint32*> bytecode, const FlatbufferVector<Offset<Shader>>* asset) {
			for (uint32 shader(0u); shader < asset->size(); ++shader) {
				SpirVShader asdf;
				ET_ABORT_UNLESS(ParseShader(asdf, bytecode, asset->Get(shader)));
				shaders.Insert(eastl::move(asdf));
			}

			return VK_SUCCESS;
		}

	} // anonymous namespace

	SpirVShaderSetAsset::SpirVShaderSetAsset(StringView path) :
		Vulkan::SpirVShaderSet(MallocAllocator("SPIR-V Shader Set Allocator")),
		Asset(path),
		_bytecode(MallocAllocator("SPIR-V Shader Package Bytecode Allocator")) {}

	// ---------------------------------------------------

	ErrorCode SpirVShaderSetAsset::BindResources(const Builder& asset) {
		//	Ensure we're working with data that can plausibly represent a shader set.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifySpirVShaderSetBuffer(verifier)) {
			asset.WriteLog(Severity::Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		ArrayList<uint32> bytecode(_bytecode.GetAllocator());
		ParseBytecode(bytecode, GetSpirVShaderSet(asset.Begin())->Bytecode());

		const auto    shaderAsset(GetSpirVShaderSet(asset.Begin()));
		ContainerType shaders(GetAllocator(), shaderAsset->Shaders()->size());
		if (Failed(ParseShaders(shaders, { bytecode.GetData(), bytecode.GetData() + bytecode.GetSize() }, shaderAsset->Shaders()))) {
			return Error::InvalidParameter;
		}

		Swap(*this, shaders);
		Swap(_bytecode, bytecode);

		return Error::None;
	}

	// ---------------------------------------------------

	void SpirVShaderSetAsset::FreeResources() {
		Clear();
		_bytecode.Clear();
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringView SpirVShaderSetAsset::GetExtension() ETNoexceptHint {
		return { SpirVShaderSetExtension(), StringLength(SpirVShaderSetExtension()) };
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
