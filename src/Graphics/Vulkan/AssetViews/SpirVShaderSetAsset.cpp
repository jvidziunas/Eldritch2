/*==================================================================*\
  SpirVShaderSetAsset.cpp
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
#include <Graphics/Vulkan/AssetViews/SpirVShaderSetAsset.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Flatbuffers/FlatBufferTools.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/SpirVShaderSet_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	using namespace ::Eldritch2::Graphics::Vulkan::FlatBuffers;
	using namespace ::Eldritch2::Graphics::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;
	using namespace ::flatbuffers;

	// ---------------------------------------------------

	namespace {

		template <typename BitField>
		ETConstexpr ETForceInlineHint ETPureFunctionHint bool TestFlag(BitField field, BitField flags) ETNoexceptHint {
			using Bits = eastl::underlying_type_t<BitField>;

			// ---

			return (Bits(field) & Bits(flags)) == Bits(flags);
		}

		// ---------------------------------------------------

		ETCpp14Constexpr ETForceInlineHint ETPureFunctionHint VkStencilOp ParseStencilOp(StencilOperation operation) ETNoexceptHint {
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

		ETCpp14Constexpr ETForceInlineHint ETPureFunctionHint VkCompareOp ParseComparisonOp(ComparisonFunction function) ETNoexceptHint {
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

		ETCpp14Constexpr ETForceInlineHint ETPureFunctionHint VkBlendOp ParseBlendOp(BlendOperator operation) ETNoexceptHint {
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

		ETCpp14Constexpr ETForceInlineHint ETPureFunctionHint VkBlendFactor ParseBlendFactor(ColorBlendFactor factor) ETNoexceptHint {
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

		ETCpp14Constexpr ETForceInlineHint ETPureFunctionHint VkBlendFactor ParseBlendFactor(AlphaBlendFactor factor) ETNoexceptHint {
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
			return (TestFlag(channels, ChannelWriteMask::Red) ? VK_COLOR_COMPONENT_R_BIT : 0u) // clang-format off
				| (TestFlag(channels, ChannelWriteMask::Green) ? VK_COLOR_COMPONENT_G_BIT : 0u)
				| (TestFlag(channels, ChannelWriteMask::Blue) ? VK_COLOR_COMPONENT_B_BIT : 0u)
				| (TestFlag(channels, ChannelWriteMask::Alpha) ? VK_COLOR_COMPONENT_A_BIT : 0u); // clang-format on
		}

		// ---------------------------------------------------

		ETForceInlineHint ETPureFunctionHint Result ParseDepthStencilState(SpirVShader& shader, const DepthStencilDescriptor* asset) ETNoexceptHint {
			using FlagBits = eastl::underlying_type<DepthStencilFlags>::type;

			static ETConstexpr auto ParseStencilOpState([](uint8 readMask, uint8 writeMask, uint8 reference, const StencilDescriptor* asset) ETNoexceptHint -> VkStencilOpState {
				return VkStencilOpState{
					ParseStencilOp(asset->StencilAndDepthFailBehavior()),
					ParseStencilOp(asset->StencilAndDepthPassBehavior()),
					ParseStencilOp(asset->StencilPassDepthFailBehavior()),
					ParseComparisonOp(asset->StencilComparisonFunction()),
					readMask,
					writeMask,
					reference
				};
			});

			// ---

			shader.depthStencil.flags                = 0u;
			shader.depthStencil.shouldWrite          = TestFlag(asset->Flags(), DepthStencilFlags::DepthWriteEnable) ? VK_TRUE : VK_FALSE;
			shader.depthStencil.depthOperator        = ParseComparisonOp(asset->DepthComparisonFunction());
			shader.depthStencil.frontStencilOperator = ParseStencilOpState(/*readMask =*/0xFF, /*writeMask =*/0xFF, /*reference =*/0xFF, asset->FrontFaceBehavior());
			shader.depthStencil.backStencilOperator  = ParseStencilOpState(/*readMask =*/0xFF, /*writeMask =*/0xFF, /*reference =*/0xFF, asset->BackFaceBehavior());

			return Result::Success;
		}

		// ---------------------------------------------------

		ETForceInlineHint ETPureFunctionHint Result ParseRasterizationState(SpirVShader& shader, const RasterizerDescriptor* asset) ETNoexceptHint {
			static ETConstexpr auto ParseCullMode([](FaceCullMode cull) ETNoexceptHint -> VkCullModeFlags {
				switch (cull) {
				default:
				case FaceCullMode::None: return VK_CULL_MODE_NONE;
				case FaceCullMode::FrontFace: return VK_CULL_MODE_FRONT_BIT;
				case FaceCullMode::BackFace: return VK_CULL_MODE_BACK_BIT;
				}; // switch(mode)
			});

			// ---

			shader.rasterizer.flags          = 0u;
			shader.rasterizer.cullMode       = ParseCullMode(asset->CullMode());
			shader.rasterizer.depthBias      = AsFloatBits(asset->DepthBias());
			shader.rasterizer.depthBiasClamp = asset->DepthBiasClamp();
			shader.rasterizer.slopeDepthBias = asset->SlopeScaledDepthBias();

			return Result::Success;
		}

		// ---------------------------------------------------

		ETForceInlineHint Result ParseOutputStates(SpirVShader& shader, const flatbuffers::Vector<Offset<OutputDescriptor>>* assets) ETNoexceptHint {
			ET_ABORT_UNLESS(assets->size() <= ETCountOf(shader.blending.attachments) ? Result::Success : Result::InvalidParameter);

			Transform(assets->begin(), assets->end(), shader.blending.attachments, [](const OutputDescriptor* asset) ETNoexceptHint -> VkPipelineColorBlendAttachmentState {
				return VkPipelineColorBlendAttachmentState{
					/*blendEnable =*/VK_TRUE,
					ParseBlendFactor(asset->SourceColorBlendFactor()),
					ParseBlendFactor(asset->DestinationColorBlendFactor()),
					ParseBlendOp(asset->ColorBlendOperator()),
					ParseBlendFactor(asset->SourceAlphaBlendFactor()),
					ParseBlendFactor(asset->DestinationAlphaBlendFactor()),
					ParseBlendOp(asset->AlphaBlendOperator()),
					ParseWriteMask(asset->WriteMask())
				};
			});

			return Result::Success;
		}

		// ---------------------------------------------------

		ETForceInlineHint Result ParseShaders(SpirVShaderSet::SubshaderMap& shaders, const flatbuffers::Vector<Offset<ShaderDescriptor>>* asset) {
			static ETConstexpr auto ParseBytecode([](ArrayList<SpirVShader::Opcode>& bytecode, const flatbuffers::Vector<uint32>* asset) {
				bytecode.Assign(reinterpret_cast<const SpirVShader::Opcode*>(asset->data()), reinterpret_cast<const SpirVShader::Opcode*>(asset->data() + asset->size()));
			});

			const auto ParseShader([&](SpirVShader& shader, const ShaderDescriptor* asset) ETNoexceptHint -> Result {
				ET_ABORT_UNLESS(ParseRasterizationState(shader, asset->Rasterizer()));
				ET_ABORT_UNLESS(ParseOutputStates(shader, asset->Outputs()));
				ET_ABORT_UNLESS(ParseDepthStencilState(shader, asset->DepthStencil()));

				return Result::Success;
			});

			// ---

			for (uoffset_t shader(0u); shader < asset->size(); ++shader) {
				String name(String::AllocatorType("Shader Name Allocator"), AsString(asset->Get(shader)->Name()));
				ET_ABORT_UNLESS(ParseShader(shaders[Move(name)], asset->Get(shader)));
			}

			return Result::Success;
		}

	} // anonymous namespace

	SpirVShaderSetAsset::SpirVShaderSetAsset(StringSpan path) ETNoexceptHint : Asset(path) {}

	// ---------------------------------------------------

	Result SpirVShaderSetAsset::BindResources(Log& log, const AssetBuilder& asset) {
		//	Ensure we're working with data that can plausibly represent a shader set.
		const auto shaderDescriptor(GetVerifiedRoot<SpirVShaderSetDescriptor>(asset.bytes, SpirVShaderSetDescriptorIdentifier()));
		ET_ABORT_UNLESS(shaderDescriptor ? Result::Success : Result::InvalidParameter, log.Write(Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath()));

		SpirVShaderSet::BytecodeList bytecode;

		SpirVShaderSet::SubshaderMap subshaderByPass;
		ET_ABORT_UNLESS(ParseShaders(subshaderByPass, shaderDescriptor->Shaders()));

		return SpirVShaderSet::BindResources(bytecode, subshaderByPass);
	}

	// ---------------------------------------------------

	void SpirVShaderSetAsset::FreeResources() ETNoexceptHint {
		SpirVShaderSet::FreeResources();
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringSpan SpirVShaderSetAsset::GetExtension() ETNoexceptHint {
		return StringSpan(SpirVShaderSetDescriptorExtension(), StringSpan::SizeType(StringLength(SpirVShaderSetDescriptorExtension())));
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
