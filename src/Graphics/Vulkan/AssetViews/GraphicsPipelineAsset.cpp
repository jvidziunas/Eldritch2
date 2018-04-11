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

	GraphicsPipelineAsset::StageAsset::StageAsset( const Utf8Char* const shaderUsageName ) {
		CopyString( _shaderUsageName, shaderUsageName );
	}

// ---------------------------------------------------

	GraphicsPipelineAsset::GraphicsPipelineAsset(
		const Utf8Char* const assetPath
	) : Asset( assetPath ),
		_attachments( MallocAllocator( "Vulkan Graphics Pipeline Attachment List Allocator" ) ),
		_stages( MallocAllocator( "Vulkan Graphics Pipeline Stage List Allocator" ) ) {
	}

// ---------------------------------------------------

	ErrorCode GraphicsPipelineAsset::BindResources( const Builder& asset ) {
	//	Ensure we're working with data that can plausibly represent a graphics pipeline.
		Verifier verifier( reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize() );
		if (!VerifyGraphicsPipelineBuffer( verifier )) {
			asset.WriteLog( MessageType::Error, "{} is malformed!" UTF8_NEWLINE, GetPath() );
			return Error::InvalidParameter;
		}

		ArrayList<AttachmentAsset>	attachments( _attachments.GetAllocator() );
		ArrayList<StageAsset>		stages( _stages.GetAllocator() );
		const auto					stageAssets( GetGraphicsPipeline( asset.Begin() )->Stages() );
		const auto					stageTypes( GetGraphicsPipeline( asset.Begin() )->Stages_type() );

		for (flatbuffers::uoffset_t stageId( 0u ); stageId < stageAssets->size(); ++stageId) {
			switch (static_cast<GraphicsStage>( stageTypes->Get( stageId ) ) ) {
			case GraphicsStage::NONE: default: {
				break;
			}
			case GraphicsStage::DrawStage: {
				const auto source( static_cast<const DrawStage*>( stageAssets->Get( stageId ) ) );
				stages.EmplaceBack( source->ShaderUsageName()->c_str() );
				break;
			}
			case GraphicsStage::CopyStage: {
				break;
			}
			case GraphicsStage::ComputeStage: {
				break;
			}
			}
		}

		Swap( _attachments,	attachments );
		Swap( _stages,		stages );

		return Error::None;
	}

// ---------------------------------------------------

	void GraphicsPipelineAsset::FreeResources() {
		_attachments.Clear( ReleaseMemorySemantics() );
		_stages.Clear( ReleaseMemorySemantics() );
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2