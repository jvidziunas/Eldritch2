/*==================================================================*\
  SPIRVPipelineDefinitionResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SPIRVPipelineDefinitionResourceView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <Renderer/Vulkan/SPIRVPipeline_generated.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace {

	class ColorBlendState : public ::VkPipelineColorBlendStateCreateInfo {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		ETInlineHint ColorBlendState( const Renderer::FlatBuffers::RenderTargetState& state ) {
			using Renderer::FlatBuffers::ColorBlendFactor;
			using Renderer::FlatBuffers::ColorBlendFactor;
			using Renderer::FlatBuffers::BlendOperator;
			using Renderer::FlatBuffers::AlphaBlendFactor;
			using Renderer::FlatBuffers::AlphaBlendFactor;
			using Renderer::FlatBuffers::BlendOperator;

			sType = ::VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			pNext = nullptr;
#if 0
			VkPipelineColorBlendStateCreateFlags          flags;
			VkBool32                                      logicOpEnable;
			VkLogicOp                                     logicOp;
			uint32_t                                      attachmentCount;
			const VkPipelineColorBlendAttachmentState*    pAttachments;
			float                                         blendConstants[4];
		}
#endif
	};

// ---

	struct DepthStencilState : public ::VkPipelineDepthStencilStateCreateInfo {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		ETInlineHint DepthStencilState( const Renderer::FlatBuffers::DepthStencilState& state ) {
			using Renderer::FlatBuffers::StencilBehavior;
			using Renderer::FlatBuffers::StencilOperation;
			using Renderer::FlatBuffers::ComparisonFunction;
			using Renderer::FlatBuffers::DepthStencilFlags;
			using DepthStencilFlagBits = uint8_t;

			struct ParsedStencilBehavior : public ::VkStencilOpState {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				ETInlineHint ParsedStencilBehavior( const StencilBehavior& state, const uint8 stencilReadMask, const uint8 stencilWriteMask ) {
					failOp			= StencilOpFromDefinition( state.StencilAndDepthFailBehavior() );
					passOp			= StencilOpFromDefinition( state.StencilAndDepthPassBehavior() );
					depthFailOp		= StencilOpFromDefinition( state.StencilPassDepthFailBehavior() );
					compareOp		= CompareOpFromDefinition( state.StencilComparisonFunction() );
					compareMask		= stencilReadMask;
					writeMask		= stencilWriteMask;
					reference		= 0u;
				}

			// ---------------------------------------------------

				static ::VkStencilOp StencilOpFromDefinition( StencilOperation operation ) {
					static const ::VkStencilOp	conversionTable[] = {
						::VkStencilOp::VK_STENCIL_OP_KEEP					// KeepExisting
						::VkStencilOp::VK_STENCIL_OP_ZERO					// SetZero
						::VkStencilOp::VK_STENCIL_OP_REPLACE				// ReplaceExisting
						::VkStencilOp::VK_STENCIL_OP_INCREMENT_AND_CLAMP	// IncrementAndSaturate
						::VkStencilOp::VK_STENCIL_OP_DECREMENT_AND_CLAMP	// DecrementAndSaturate
						::VkStencilOp::VK_STENCIL_OP_INVERT					// InvertExisting
						::VkStencilOp::VK_STENCIL_OP_INCREMENT_AND_WRAP		// IncrementWithWrap
						::VkStencilOp::VK_STENCIL_OP_DECREMENT_AND_WRAP		// DecrementWithWrap
					};

					static_assert( _countof( conversionTable ) == StencilOperation::MAX, "Vulkan stencil operation conversion table needs update!" );

					return conversionTable[static_cast<size_t>( operation )];
				}

			// ---------------------------------------------------

				static ::VkCompareOp CompareOpFromDefinition( ComparisonFunction operation ) {
					static const ::VkCompareOp	conversionTable[] = {
						::VkCompareOp::VK_COMPARE_OP_NEVER,				// NeverPass
						::VkCompareOp::VK_COMPARE_OP_LESS,				// PassIfLess
						::VkCompareOp::VK_COMPARE_OP_EQUAL,				// PassIfEqual
						::VkCompareOp::VK_COMPARE_OP_LESS_OR_EQUAL,		// PassIfLessOrEqual
						::VkCompareOp::VK_COMPARE_OP_GREATER,			// PassIfGreater
						::VkCompareOp::VK_COMPARE_OP_NOT_EQUAL,			// PassIfNotEqual
						::VkCompareOp::VK_COMPARE_OP_GREATER_OR_EQUAL,	// PassIfGreaterOrEqual
						::VkCompareOp::VK_COMPARE_OP_ALWAYS				// AlwaysPass
					};

					static_assert( _countof( conversionTable ) == ComparisonFunction::MAX, "Vulkan comparison operation conversion table needs update!" );

					return conversionTable[static_cast<size_t>( operation )];
				}
			};

		// ---

			sType				= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			pNext				= nullptr;
			depthTestEnable		= static_cast<DepthStencilFlagBits>( state.Flags() ) & static_cast<DepthStencilFlagBits>( DepthStencilFlags::DepthTestEnable ) ? 1 : 0;
			depthWriteEnable	= static_cast<DepthStencilFlagBits>( state.Flags() ) & static_cast<DepthStencilFlagBits>( DepthStencilFlags::DepthWriteEnable ) ? 1 : 0;
			stencilTestEnable	= static_cast<DepthStencilFlagBits>( state.Flags() ) & static_cast<DepthStencilFlagBits>( DepthStencilFlags::StencilTestEnable ) ? 1 : 0;
			front				= ParsedStencilBehavior( *state.FrontFaceBehavior(), state.StencilReadMask(), state.StencilWriteMask() );
			back				= ParsedStencilBehavior( *state.BackFaceBehavior(), state.StencilReadMask(), state.StencilWriteMask() );
			depthCompareOp		= ParsedStencilBehavior::CompareOpFromDefinition( state.DepthComparisonFunction() );
		}
	};

}	// anonymous namespace 

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	SPIRVPipelineDefinitionResourceView::SPIRVPipelineDefinitionResourceView( const UTF8Char* const name ) : ResourceView( name ), _pipeline( nullptr ) {}

// ---------------------------------------------------

	ErrorCode SPIRVPipelineDefinitionResourceView::AttachToPackage( const Range<const char*> bytes, ContentPackage& /*package*/, ContentLibrary& library ) {
		if( !FlatBuffers::SpirVPipelineBufferHasIdentifier( bytes.first ) ) {
			return Error::InvalidParameter;
		}

		const auto	definition( FlatBuffers::GetSpirVPipeline( bytes.first ) );

		PublishToLibraryAs<SPIRVPipelineDefinitionResourceView>( library );
		return Error::None;
	}

// ---------------------------------------------------

	void SPIRVPipelineDefinitionResourceView::DetachFromPackage( ContentPackage& /*package*/, ContentLibrary& library ) const {
		RemoveFromLibraryAs<SPIRVPipelineDefinitionResourceView>( library );
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const SPIRVPipelineDefinitionResourceView::GetSerializedDataTag() {
		return FlatBuffers::SpirVPipelineExtension();
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2
