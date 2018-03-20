/*==================================================================*\
  SpirVShaderPackageAsset.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/SpirVShaderPackageAsset.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/SpirVShaderPackage_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace AssetViews {
namespace {

	using namespace ::Eldritch2::Graphics::Vulkan::FlatBuffers;
	using namespace ::Eldritch2::Graphics::FlatBuffers;

	static ETInlineHint ETPureFunctionHint VkStencilOp AsStencilOp( StencilOperation operation ) {
		switch (operation) {
			case StencilOperation::KeepExisting:			return VK_STENCIL_OP_KEEP;
			case StencilOperation::SetZero:					return VK_STENCIL_OP_ZERO;
			case StencilOperation::ReplaceExisting:			return VK_STENCIL_OP_REPLACE;
			case StencilOperation::IncrementAndSaturate:	return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			case StencilOperation::DecrementAndSaturate:	return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			case StencilOperation::InvertExisting:			return VK_STENCIL_OP_INVERT;
			case StencilOperation::IncrementWithWrap:		return VK_STENCIL_OP_INCREMENT_AND_WRAP;
			case StencilOperation::DecrementWithWrap:		return VK_STENCIL_OP_DECREMENT_AND_WRAP;
			ET_NO_DEFAULT_CASE;
		};	// switch( operation )
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint VkCompareOp AsComparisonOp( ComparisonFunction function ) {
		switch (function) {
			case ComparisonFunction::NeverPass:				return VK_COMPARE_OP_NEVER;
			case ComparisonFunction::PassIfLess:			return VK_COMPARE_OP_LESS;
			case ComparisonFunction::PassIfEqual:			return VK_COMPARE_OP_EQUAL;
			case ComparisonFunction::PassIfLessOrEqual:		return VK_COMPARE_OP_LESS_OR_EQUAL;
			case ComparisonFunction::PassIfGreater:			return VK_COMPARE_OP_GREATER;
			case ComparisonFunction::PassIfNotEqual:		return VK_COMPARE_OP_NOT_EQUAL;
			case ComparisonFunction::PassIfGreaterOrEqual:	return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case ComparisonFunction::AlwaysPass:			return VK_COMPARE_OP_ALWAYS;
			ET_NO_DEFAULT_CASE;
		};
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint VkBlendOp AsBlendOp( BlendOperator operation ) {
		switch (operation) {
			case BlendOperator::Add:				return VK_BLEND_OP_ADD;
			case BlendOperator::Subtract:			return VK_BLEND_OP_SUBTRACT;
			case BlendOperator::ReverseSubtract:	return VK_BLEND_OP_REVERSE_SUBTRACT;
			case BlendOperator::Minimum:			return VK_BLEND_OP_MIN;
			case BlendOperator::Maximum:			return VK_BLEND_OP_MAX;
			ET_NO_DEFAULT_CASE;
		};	// switch( operation )
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint VkBlendFactor AsBlendFactor( ColorBlendFactor factor ) {
		switch (factor) {
			case ColorBlendFactor::Zero:						return VK_BLEND_FACTOR_ZERO;
			case ColorBlendFactor::One:							return VK_BLEND_FACTOR_ONE;
			case ColorBlendFactor::SourceColor:					return VK_BLEND_FACTOR_SRC_COLOR;
			case ColorBlendFactor::OneMinusSourceColor:			return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case ColorBlendFactor::SourceAlpha:					return VK_BLEND_FACTOR_SRC_ALPHA;
			case ColorBlendFactor::OneMinusSourceAlpha:			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case ColorBlendFactor::DestinationAlpha:			return VK_BLEND_FACTOR_DST_ALPHA;
			case ColorBlendFactor::OneMinusDestinationAlpha:	return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case ColorBlendFactor::DestinationColor:			return VK_BLEND_FACTOR_DST_COLOR;
			case ColorBlendFactor::OneMinusDestinationColor:	return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			case ColorBlendFactor::SaturatedSourceAlpha:		return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			case ColorBlendFactor::BlendFactor:					return VK_BLEND_FACTOR_CONSTANT_COLOR;
			case ColorBlendFactor::OneMinusBlendFactor:			return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			case ColorBlendFactor::BlendFactorAlpha:			return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case ColorBlendFactor::OneMinusBlendFactorAlpha:	return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
			case ColorBlendFactor::DualSourceColor:				return VK_BLEND_FACTOR_SRC1_COLOR;
			case ColorBlendFactor::OneMinusDualSourceColor:		return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
			case ColorBlendFactor::DualSourceAlpha:				return VK_BLEND_FACTOR_SRC1_ALPHA;
			case ColorBlendFactor::OneMinusDualSourceAlpha:		return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
			ET_NO_DEFAULT_CASE;
		};	// switch( factor )
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint VkBlendFactor AsBlendFactor( AlphaBlendFactor factor ) {
		switch (factor) {
			case AlphaBlendFactor::Zero:						return VK_BLEND_FACTOR_ZERO;
			case AlphaBlendFactor::One:							return VK_BLEND_FACTOR_ONE;
			case AlphaBlendFactor::SourceAlpha:					return VK_BLEND_FACTOR_SRC_ALPHA;
			case AlphaBlendFactor::OneMinusSourceAlpha:			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case AlphaBlendFactor::DestinationAlpha:			return VK_BLEND_FACTOR_DST_ALPHA;
			case AlphaBlendFactor::OneMinusDestinationAlpha:	return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case AlphaBlendFactor::SaturatedSourceAlpha:		return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			case AlphaBlendFactor::BlendFactorAlpha:			return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case AlphaBlendFactor::OneMinusBlendFactorAlpha:	return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
			case AlphaBlendFactor::DualSourceAlpha:				return VK_BLEND_FACTOR_SRC1_ALPHA;
			case AlphaBlendFactor::OneMinusDualSourceAlpha:		return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
			ET_NO_DEFAULT_CASE;
		};	// switch( factor )
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint VkColorComponentFlags AsWriteMask( ChannelWriteMask mask ) {
		using FlagBits = eastl::underlying_type<ChannelWriteMask>::type;

	// ---

		VkColorComponentFlags	flags( 0u );

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

	static ETInlineHint ETPureFunctionHint VkPipelineColorBlendAttachmentState AsColorBlendState( const RenderTargetState* definition ) {
		return VkPipelineColorBlendAttachmentState{
			VK_TRUE,
			AsBlendFactor( definition->SourceColorBlendFactor() ),
			AsBlendFactor( definition->DestinationColorBlendFactor() ),
			AsBlendOp( definition->ColorBlendOperator() ),
			AsBlendFactor( definition->SourceAlphaBlendFactor() ),
			AsBlendFactor( definition->DestinationAlphaBlendFactor() ),
			AsBlendOp( definition->AlphaBlendOperator() ),
			AsWriteMask( definition->WriteMask() )
		};
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint VkStencilOpState AsStencilOpState( const StencilBehavior* const definition, uint8 readMask, uint8 writeMask, uint8 stencilReference ) {
		return VkStencilOpState{
			AsStencilOp( definition->StencilAndDepthFailBehavior() ),
			AsStencilOp( definition->StencilAndDepthPassBehavior() ),
			AsStencilOp( definition->StencilPassDepthFailBehavior() ),
			AsComparisonOp( definition->StencilComparisonFunction() ),
			readMask,
			writeMask,
			stencilReference
		};
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint VkPolygonMode AsPolygonMode( PolygonFillMode mode ) {
		switch (mode) {
			case PolygonFillMode::Wireframe:	return VK_POLYGON_MODE_LINE;
			case PolygonFillMode::Solid:		return VK_POLYGON_MODE_FILL;
			ET_NO_DEFAULT_CASE;
		};	// switch( mode )
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint VkCullModeFlags AsCullMode( FaceCullMode mode ) {
		switch (mode) {
			case FaceCullMode::FrontFace:	return VK_CULL_MODE_FRONT_BIT;
			case FaceCullMode::BackFace:	return VK_CULL_MODE_BACK_BIT;
			case FaceCullMode::None:		return VK_CULL_MODE_NONE;
			ET_NO_DEFAULT_CASE;
		};	// switch( mode )
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint VkPipelineDepthStencilStateCreateInfo ParseDepthStencilState( const DepthStencilState* const definition ) {
		using FlagBits = eastl::underlying_type<DepthStencilFlags>::type;

	// ---

		return VkPipelineDepthStencilStateCreateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			nullptr,	// extension structure(s)
			0,			// flags
			VkBool32( static_cast<FlagBits>(definition->Flags()) & static_cast<FlagBits>(DepthStencilFlags::DepthTestEnable) ? VK_TRUE : VK_FALSE ),
			VkBool32( static_cast<FlagBits>(definition->Flags()) & static_cast<FlagBits>(DepthStencilFlags::DepthWriteEnable) ? VK_TRUE : VK_FALSE ),
			AsComparisonOp( definition->DepthComparisonFunction() ),
			VK_FALSE,	// depthBoundsTestEnable
			VkBool32( static_cast<FlagBits>(definition->Flags()) & static_cast<FlagBits>(DepthStencilFlags::StencilTestEnable) ? VK_TRUE : VK_FALSE ),
			AsStencilOpState( definition->FrontFaceBehavior(), 0xFF, 0xFF, 0xFF ),
			AsStencilOpState( definition->BackFaceBehavior(), 0xFF, 0xFF, 0xFF ),
			0.0f,
			1.0f
		};
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint VkPipelineRasterizationStateCreateInfo ParseRasterizationState( const RasterizationState* const definition ) {
		return VkPipelineRasterizationStateCreateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			nullptr,	// extension structure(s)
			0,			// flags
			VK_FALSE,	// depthClampEnable
			VK_FALSE,	// rasterizerDiscardEnable
			AsPolygonMode( definition->FillMode() ),
			AsCullMode( definition->CullMode() ),
			VK_FRONT_FACE_CLOCKWISE,
			VkBool32( definition->DepthBias() != 0 ? VK_TRUE : VK_FALSE ),
			static_cast<const float&>(definition->DepthBias()),
			definition->DepthBiasClamp(),
			definition->SlopeScaledDepthBias(),
			1.0f		// lineWidth
		};
	}

}	// anonymous namespace

	using namespace ::Eldritch2::Graphics::Vulkan::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	SpirVShaderPackageAsset::Usage::Usage(
		const Utf8Char* name,
		const VkPipelineColorBlendAttachmentState (&colorBlends)[MaxColorAttachments],
		VkShaderStageFlagBits enabledStages,
		const VkPipelineMultisampleStateCreateInfo& multisample,
		const VkPipelineRasterizationStateCreateInfo& rasterization,
		const VkPipelineDepthStencilStateCreateInfo& depthStencil
	) : enabledStages( enabledStages ),
		multisample( multisample ),
		rasterization( rasterization ),
		depthStencil( depthStencil ) {
		CopyString( this->name, name, _countof( this->name ) );
		CopyArrayNonTemporal( this->colorBlends, colorBlends );
	}

// ---------------------------------------------------

	ETPureFunctionHint bool SpirVShaderPackageAsset::UsageEqual::operator()( const Usage& usage, const Utf8Char* name ) const {
		return StringsEqual( usage.name, name );
	}

// ---------------------------------------------------

	ETPureFunctionHint bool SpirVShaderPackageAsset::UsageEqual::operator()( const Usage& usage0, const Usage& usage1 ) const {
		return (*this)( usage0, usage1.name );
	}

// ---------------------------------------------------

	ETPureFunctionHint size_t SpirVShaderPackageAsset::UsageHash::operator()( const Utf8Char* name, size_t seed ) const {
		return HashMemory( name, StringLength( name ), seed );
	}

// ---------------------------------------------------

	ETPureFunctionHint size_t SpirVShaderPackageAsset::UsageHash::operator()( const Usage& usage, size_t seed ) const {
		return (*this)( usage.name, seed );
	}

// ---------------------------------------------------

	SpirVShaderPackageAsset::SpirVShaderPackageAsset( const Utf8Char* const path ) : Asset( path ), _usages( MallocAllocator( "SPIR-V Shader Package Usage Set Allocator" ) ) {}

// ---------------------------------------------------

	ErrorCode SpirVShaderPackageAsset::BindResources( const Builder& builder ) {
		enum : VkShaderModuleCreateFlags { ShaderCreateFlags = 0u };

		const Range<const char*>&	bytes( builder.GetRawBytes() );

	//	Ensure we're working with data that can plausibly represent a shader package.
		Verifier verifier( reinterpret_cast<const uint8_t*>(bytes.Begin()), bytes.GetSize() );
		if (!VerifySpirVShaderPackageBuffer( verifier )) {
			builder.WriteLog( MessageType::Error, "{} is malformed." UTF8_NEWLINE, GetPath() );
			return Error::InvalidParameter;
		}
			
		UsageSet<Usage>	usages( _usages.GetAllocator() );

		for (const UsageDescriptor* usage : *GetSpirVShaderPackage( bytes.Begin() )->Usages()) {
			VkPipelineColorBlendAttachmentState	colorBlendStates[Usage::MaxColorAttachments];

			usages.Emplace(
				usage->Name()->c_str(),
				colorBlendStates,
				VK_SHADER_STAGE_ALL_GRAPHICS,
				VkPipelineMultisampleStateCreateInfo{},
				ParseRasterizationState( usage->RasterizationBehavior() ),
				ParseDepthStencilState( usage->DepthStencilBehavior() )
			);
		}

		Swap( _usages,	usages );

		return Error::None;
	}

// ---------------------------------------------------

	void SpirVShaderPackageAsset::FreeResources() {
		_usages.Clear();
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2