/*==================================================================*\
  SpirVShaderPipelineView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/AssetViews/SpirVShaderPipelineView.hpp>
#include <Renderer/Vulkan/ObjectBuilders.hpp>
#include <Renderer/Vulkan/VulkanResult.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Containers/Range.hpp>
#include <Renderer/Vulkan/Device.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/SpirVShaderPipeline_generated.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::flatbuffers;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace AssetViews {
namespace {

//	TODO: Investigate codegen for these, as it seems everything ends up being a no-behavior. Would using a switch statement improve?
	static ETPureFunctionHint VkStencilOp GetStencilOp( Renderer::FlatBuffers::StencilOperation operation ) {
		static const VkStencilOp	operations[static_cast<size_t>(Renderer::FlatBuffers::StencilOperation::MAX) + 1] = {
			VkStencilOp::VK_STENCIL_OP_KEEP,				// KeepExisting
			VkStencilOp::VK_STENCIL_OP_ZERO,				// SetZero
			VkStencilOp::VK_STENCIL_OP_REPLACE,				// ReplaceExisting
			VkStencilOp::VK_STENCIL_OP_INCREMENT_AND_CLAMP,	// IncrementAndSaturate
			VkStencilOp::VK_STENCIL_OP_DECREMENT_AND_CLAMP,	// DecrementAndSaturate
			VkStencilOp::VK_STENCIL_OP_INVERT,				// InvertExisting
			VkStencilOp::VK_STENCIL_OP_INCREMENT_AND_WRAP,	// IncrementWithWrap
			VkStencilOp::VK_STENCIL_OP_DECREMENT_AND_WRAP,	// DecrementWithWrap
		};

		return operations[static_cast<size_t>( operation )];
	}

// ---------------------------------------------------

	static ETPureFunctionHint VkCompareOp GetComparisonOp( Renderer::FlatBuffers::ComparisonFunction function ) {
		static const VkCompareOp	comparisons[static_cast<size_t>(Renderer::FlatBuffers::ComparisonFunction::MAX) + 1] = {
			VkCompareOp::VK_COMPARE_OP_NEVER,			// NeverPass
			VkCompareOp::VK_COMPARE_OP_LESS,			// PassIfLess
			VkCompareOp::VK_COMPARE_OP_EQUAL,			// PassIfEqual
			VkCompareOp::VK_COMPARE_OP_LESS_OR_EQUAL,	// PassIfLessOrEqual
			VkCompareOp::VK_COMPARE_OP_GREATER,			// PassIfGreater
			VkCompareOp::VK_COMPARE_OP_NOT_EQUAL,		// PassIfNotEqual
			VkCompareOp::VK_COMPARE_OP_GREATER_OR_EQUAL,// PassIfGreaterOrEqual
			VkCompareOp::VK_COMPARE_OP_ALWAYS			// AlwaysPass
		};

		return comparisons[static_cast<size_t>( function )];
	}

// ---------------------------------------------------

	static ETPureFunctionHint VkBlendOp GetBlendOp( Renderer::FlatBuffers::BlendOperator operation ) {
		static const VkBlendOp	blends[static_cast<size_t>(Renderer::FlatBuffers::BlendOperator::MAX) + 1] = {
			VkBlendOp::VK_BLEND_OP_ADD,				// Add
			VkBlendOp::VK_BLEND_OP_SUBTRACT,		// Subtract
			VkBlendOp::VK_BLEND_OP_REVERSE_SUBTRACT,// ReverseSubtract
			VkBlendOp::VK_BLEND_OP_MIN,				// Minimum
			VkBlendOp::VK_BLEND_OP_MAX				// Maximum
		};

		return blends[static_cast<size_t>( operation )];
	}

// ---------------------------------------------------

	static ETPureFunctionHint VkBlendFactor GetBlendFactor( Renderer::FlatBuffers::ColorBlendFactor factor ) {
		static const VkBlendFactor	factors[static_cast<size_t>(Renderer::FlatBuffers::ColorBlendFactor::MAX) + 1] = {
			VkBlendFactor::VK_BLEND_FACTOR_ZERO,					// Zero
			VkBlendFactor::VK_BLEND_FACTOR_ONE,						// One
			VkBlendFactor::VK_BLEND_FACTOR_SRC_COLOR,				// SourceColor
			VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,		// OneMinusSourceColor
			VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA,				// SourceAlpha
			VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, 	// OneMinusSourceAlpha
			VkBlendFactor::VK_BLEND_FACTOR_DST_ALPHA,				// DestinationAlpha
			VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA, 	// OneMinusDestinationAlpha
			VkBlendFactor::VK_BLEND_FACTOR_DST_COLOR, 				// DestinationColor
			VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,		// OneMinusDestinationColor
			VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,		// SaturatedSourceAlpha
			VkBlendFactor::VK_BLEND_FACTOR_CONSTANT_COLOR,			// BlendFactor
			VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,// OneMinusBlendFactor
			VkBlendFactor::VK_BLEND_FACTOR_CONSTANT_ALPHA,			// BlendFactorAlpha
			VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,// OneMinusBlendFactorAlpha
			VkBlendFactor::VK_BLEND_FACTOR_SRC1_COLOR,				// DualSourceColor
			VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,	// OneMinusDualSourceColor
			VkBlendFactor::VK_BLEND_FACTOR_SRC1_ALPHA,				// DualSourceAlpha
			VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA		// OneMinusDualSourceAlpha
		};

		return factors[static_cast<size_t>(factor)];
	}

// ---------------------------------------------------

	static ETPureFunctionHint VkBlendFactor GetBlendFactor( Renderer::FlatBuffers::AlphaBlendFactor operation ) {
		static const VkBlendFactor	conversionTable[static_cast<size_t>(Renderer::FlatBuffers::AlphaBlendFactor::MAX) + 1] = {
			VkBlendFactor::VK_BLEND_FACTOR_ZERO,					// Zero
			VkBlendFactor::VK_BLEND_FACTOR_ONE,						// One
			VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA,				// SourceAlpha
			VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,		// OneMinusSourceAlpha
			VkBlendFactor::VK_BLEND_FACTOR_DST_ALPHA,				// DestinationAlpha
			VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,		// OneMinusDestinationAlpha
			VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,		// SaturatedSourceAlpha
			VkBlendFactor::VK_BLEND_FACTOR_CONSTANT_ALPHA,			// BlendFactorAlpha
			VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,// OneMinusBlendFactorAlpha
			VkBlendFactor::VK_BLEND_FACTOR_SRC1_ALPHA,				// DualSourceAlpha
			VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA		// OneMinusDualSourceAlpha
		};

		return conversionTable[static_cast<size_t>(operation)];
	}

// ---------------------------------------------------

	static ETPureFunctionHint VkColorComponentFlags GetWriteMask( Renderer::FlatBuffers::ChannelWriteMask mask ) {
		using FlagBits = eastl::underlying_type<Renderer::FlatBuffers::ChannelWriteMask>::type;

	// ---

		VkColorComponentFlags	result( 0u );

		if( static_cast<FlagBits>(mask) & static_cast<FlagBits>(Renderer::FlatBuffers::ChannelWriteMask::Red) ) {
			result |= VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT;
		}

		if( static_cast<FlagBits>(mask) & static_cast<FlagBits>(Renderer::FlatBuffers::ChannelWriteMask::Green) ) {
			result |= VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT;
		}

		if( static_cast<FlagBits>(mask) & static_cast<FlagBits>(Renderer::FlatBuffers::ChannelWriteMask::Blue) ) {
			result |= VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT;
		}

		if( static_cast<FlagBits>(mask) & static_cast<FlagBits>(Renderer::FlatBuffers::ChannelWriteMask::Alpha) ) {
			result |= VkColorComponentFlagBits::VK_COLOR_COMPONENT_A_BIT;
		}
	
		return result;
	}

// ---------------------------------------------------

	static void ParseColorBlendState( VkPipelineColorBlendAttachmentState& state, const Renderer::FlatBuffers::RenderTargetState& definition ) {
		state.blendEnable			= VK_TRUE;
		state.srcColorBlendFactor	= GetBlendFactor( definition.SourceColorBlendFactor() );
		state.dstColorBlendFactor	= GetBlendFactor( definition.DestinationColorBlendFactor() );
		state.colorBlendOp			= GetBlendOp( definition.ColorBlendOperator() );
		state.srcAlphaBlendFactor	= GetBlendFactor( definition.SourceAlphaBlendFactor() );
		state.dstAlphaBlendFactor	= GetBlendFactor( definition.DestinationAlphaBlendFactor() );
		state.alphaBlendOp			= GetBlendOp( definition.AlphaBlendOperator() );
		state.colorWriteMask		= GetWriteMask( definition.WriteMask() );
	}

// ---------------------------------------------------

	static void ParseStencilOpState( VkStencilOpState& state, const Renderer::FlatBuffers::StencilBehavior& definition, uint8 stencilReadMask, uint8 stencilWriteMask, uint8 stencilReference ) {
		state.failOp		= GetStencilOp( definition.StencilAndDepthFailBehavior() );
		state.passOp		= GetStencilOp( definition.StencilAndDepthPassBehavior() );
		state.depthFailOp	= GetStencilOp( definition.StencilPassDepthFailBehavior() );
		state.compareOp		= GetComparisonOp( definition.StencilComparisonFunction() );
		state.compareMask	= stencilReadMask;
		state.writeMask		= stencilWriteMask;
		state.reference		= stencilReference;
	}

// ---------------------------------------------------

	static void ParseDepthStencilState( VkPipelineDepthStencilStateCreateInfo& createInfo, const Renderer::FlatBuffers::DepthStencilState& definition ) {
		using FlagBits = eastl::underlying_type<Renderer::FlatBuffers::DepthStencilFlags>::type;

	// ---

		createInfo.sType				= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		createInfo.pNext				= nullptr;
		createInfo.depthTestEnable		= static_cast<FlagBits>(definition.Flags()) & static_cast<FlagBits>(Renderer::FlatBuffers::DepthStencilFlags::DepthTestEnable)		? VK_TRUE : VK_FALSE;
		createInfo.depthWriteEnable		= static_cast<FlagBits>(definition.Flags()) & static_cast<FlagBits>(Renderer::FlatBuffers::DepthStencilFlags::DepthWriteEnable)		? VK_TRUE : VK_FALSE;
		createInfo.stencilTestEnable	= static_cast<FlagBits>(definition.Flags()) & static_cast<FlagBits>(Renderer::FlatBuffers::DepthStencilFlags::StencilTestEnable)	? VK_TRUE : VK_FALSE;

		ParseStencilOpState( createInfo.front, *definition.FrontFaceBehavior(), definition.StencilReadMask(), definition.StencilWriteMask(), definition.StencilReference() );
		ParseStencilOpState( createInfo.back, *definition.BackFaceBehavior(), definition.StencilReadMask(), definition.StencilWriteMask(), definition.StencilReference() );

		createInfo.depthCompareOp = GetComparisonOp( definition.DepthComparisonFunction() );
	}

// ---------------------------------------------------

	static ETPureFunctionHint bool IsValid( const flatbuffers::String* const string ) {
		return string && ( string->size() > 0 );
	}

// ---------------------------------------------------

	static ETInlineHint Result<UniquePointer<VkPipeline>> CreatePipelineForDefinition( Device& device, const FlatBuffers::UsageDescriptor& definition ) {
		return Error::Unspecified;
	}

}	// anonymous namespace

	SpirVShaderPipelineView::Usage::Usage( Vulkan::UniquePointer<VkPipeline> pipeline ) : _pipeline( eastl::move( pipeline ) ) {}

// ---------------------------------------------------

	SpirVShaderPipelineView::Usage::Usage( Usage&& other ) : _pipeline( eastl::move( other._pipeline ) ) {}

// ---------------------------------------------------

	SpirVShaderPipelineView::Usage::Usage( const Usage& ) {
	//	This function should not ever be called in practice.
		ETRuntimeAssert( false );
	}

// ---------------------------------------------------

	void SpirVShaderPipelineView::Usage::Bind( VkCommandBuffer commandBuffer ) const {
		vkCmdBindPipeline( commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline.Get() );
	}

// ---------------------------------------------------

	SpirVShaderPipelineView::SpirVShaderPipelineView( const Utf8Char* const name, UniquePointer<VkShaderModule> shader, HashMap<Utf8String<>, Usage>&& usages ) : AssetView( name ), _shader( eastl::move( shader ) ), _usages( eastl::move( usages ) ) {}

// ---------------------------------------------------

	bool SpirVShaderPipelineView::TryBindUsage( VkCommandBuffer commandBuffer, const Utf8Char* const name ) const {
		const auto candidate( _usages.Find( name, Hash<const Utf8Char*>(), eastl::equal_to_2<const Utf8String<>, const Utf8Char*>() ) );

		if( candidate == _usages.End() ) {
			return false;
		}

		candidate->second.Bind( commandBuffer );
		return true;
	}

// ---------------------------------------------------

	Result<Eldritch2::UniquePointer<AssetView>> SpirVShaderPipelineView::CreateView( Allocator& allocator, Device& deviceContext, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
		Verifier	verifier( reinterpret_cast<const uint8_t*>( rawBytes.Begin() ), rawBytes.GetSize() );

	//	Ensure we're working with data that can plausibly represent a pipeline.
		if( !FlatBuffers::SpirVShaderPipelineBufferHasIdentifier( rawBytes.Begin() ) || !FlatBuffers::VerifySpirVShaderPipelineBuffer( verifier ) ) {
			library.GetLog()( MessageSeverity::Error, "SPIR-V pipeline '{}' is malformed." ET_UTF8_NEWLINE_LITERAL, name );
			return Error::InvalidParameter;
		}

		const FlatBuffers::SpirVShaderPipeline&	pipelineDefinition( *FlatBuffers::GetSpirVShaderPipeline( rawBytes.Begin() ) );
		if( !pipelineDefinition.ShaderBytecode() ) {
			return Error::InvalidParameter;
		}

	//	Compile the bytecode into a form ready for GPU submission.
		VulkanResult<VkShaderModule>	shader( ShaderModuleBuilder().SetBytecode( pipelineDefinition.ShaderBytecode()->data(), pipelineDefinition.ShaderBytecode()->size() ).Create( deviceContext, deviceContext.GetAllocator() ) );
		if( !shader ) {
			library.GetLog()( MessageSeverity::Error, "SPIR-V pipeline '{}' contains invalid shader bytecode." ET_UTF8_NEWLINE_LITERAL, name );
			return Error::InvalidParameter;
		}

	//	Compile all pipeline sub-definitions into a form ready for GPU submission.
		HashMap<Utf8String<>, Usage>	usages( { allocator, "SPIR-V Graphics Pipeline Usage Collection" } );
		for( const auto& usage : *(pipelineDefinition.Usages()) ) {
			Result<UniquePointer<VkPipeline>>	createPipelineResult( CreatePipelineForDefinition( deviceContext, *usage ) );
			if( !createPipelineResult ) {
				library.GetLog()( MessageSeverity::Error, "SPIR-V pipeline '{}' contains invalid usage '{}'." ET_UTF8_NEWLINE_LITERAL, name, usage->Name()->c_str() );
				continue;
			}

			usages.Emplace( Utf8String<>( usage->Name()->c_str(), { allocator, "SPIR-V Graphics Pipeline Usage Name Allocator" } ), Usage( eastl::move( *createPipelineResult ) ) );
		}

	//	Combine everything into the final pipeline view object.
		auto	result( MakeUnique<SpirVShaderPipelineView>( allocator, name, eastl::move( *shader ), eastl::move( usages ) ) );
		if( !result ) {
			return Error::OutOfMemory;
		}

		return eastl::move( result );
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal SpirVShaderPipelineView::GetExtension() {
		return Utf8Literal( FlatBuffers::SpirVShaderPipelineExtension() );
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2