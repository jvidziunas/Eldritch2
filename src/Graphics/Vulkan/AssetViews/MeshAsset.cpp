/*==================================================================*\
  MeshAsset.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/MeshAsset.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/Mesh_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace AssetViews {

	using namespace ::Eldritch2::Graphics::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	MeshAsset::MeshAsset(
		const Utf8Char* const path
	) : Asset( path ),
		_attributes( MallocAllocator( "Mesh Asset Attribute Collection Allocator" ) ),
		_surfaces( MallocAllocator( "Mesh Asset Element Collection Allocator" ) ) {}

// ---------------------------------------------------

	MeshSource::Dimensions MeshAsset::GetDimensions() const {
		return { 0u, 0u };
	}

// ---------------------------------------------------

	void MeshAsset::Stream( const VertexStreamRequest& /*vertices*/, const IndexStreamRequest& /*indices*/ ) const {}

// ---------------------------------------------------

	ErrorCode MeshAsset::BindResources( const Builder& asset ) {
	//	Verify the data we're working with can plausibly represent a mesh.
		Verifier verifier( reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize() );
		if (!VerifyMeshBuffer( verifier )) {
			asset.WriteLog( MessageType::Error, "Mesh {} is malformed!" UTF8_NEWLINE, GetPath() );
			return Error::Unspecified;
		}

		ArrayList<Attribute>	attributes( _attributes.GetAllocator() );
		ArrayList<Surface>		surfaces( _surfaces.GetAllocator() );

		Swap( _attributes,	attributes );
		Swap( _surfaces,	surfaces );

		return Error::None;
	}

// ---------------------------------------------------

	void MeshAsset::FreeResources() {
		_attributes.Clear( ReleaseMemorySemantics() );
		_surfaces.Clear( ReleaseMemorySemantics() );
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2