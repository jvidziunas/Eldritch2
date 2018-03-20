/*==================================================================*\
  RenderPipelineAsset.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/RenderPipelineAsset.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/RenderPipeline_generated.h>
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

	RenderPipelineAsset::Stage::Stage( const Utf8Char* const shaderUsageName ) {
		CopyString( _shaderUsageName, shaderUsageName );
	}

// ---------------------------------------------------

	RenderPipelineAsset::RenderPipelineAsset(
		const Utf8Char* const assetPath
	) : Asset( assetPath ),
		_attachments( MallocAllocator( "Vulkan Render Pipeline Attachment List Allocator" ) ),
		_stages( MallocAllocator( "Vulkan Render Pipeline Stage List Allocator" ) ) {}

// ---------------------------------------------------

	ErrorCode RenderPipelineAsset::BindResources( const Builder& builder ) {
		const Range<const char*>&	bytes( builder.GetRawBytes() );

	//	Ensure we're working with data that can plausibly represent a render pipeline.
		Verifier verifier( reinterpret_cast<const uint8_t*>(bytes.Begin()), bytes.GetSize() );
		if (!VerifyRenderPipelineBuffer( verifier )) {
			builder.WriteLog( MessageType::Error, "{} is malformed!" UTF8_NEWLINE, GetPath() );
			return Error::InvalidParameter;
		}

		ArrayList<Attachment>	attachments( _attachments.GetAllocator() );
		ArrayList<Stage>		stages( _stages.GetAllocator() );

		Swap( _attachments,	attachments );
		Swap( _stages,		stages );

		return Error::None;
	}

// ---------------------------------------------------

	void RenderPipelineAsset::FreeResources() {
		_attachments.Clear( ReleaseMemorySemantics() );
		_stages.Clear( ReleaseMemorySemantics() );
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2