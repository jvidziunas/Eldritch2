/*==================================================================*\
  MeshResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/MeshResourceView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	MeshResourceView::MeshResourceView( const UTF8Char* const name ) : ResourceView( name ), _vertexBuffer( nullptr ) {}

// ---------------------------------------------------

#if 0
	void MeshResourceView::IssueDrawCommands( DrawBatch& batch, uint16 inclusiveMinLod ) {
		for( const auto& submesh : _submeshes ) {
			if( submesh.levelOfDetail < inclusiveMinLod ) {
				continue;
			}

			if( batch.instanceFilterCommandBuffer ) {

			}

			if( batch.drawCommandBuffer ) {
				::vkCmdBindIndexBuffer( batch.drawCommandBuffer, _indexBuffer, submesh.offsetIntoIndexBuffer, submesh.bindAsUInt16 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32 );
				::vkCmdDrawIndexedIndirect( commandBuffer, submesh.indexCount, );
			}
		}
	}
#endif

// ---------------------------------------------------

	ErrorCode MeshResourceView::AttachToPackage( const Range<const char*> /*bytes*/, ContentPackage& /*package*/, ContentLibrary& library ) {
		_submeshes.Clear();

		PublishToLibraryAs<MeshResourceView>( library );
		return Error::None;
	}

// ---------------------------------------------------

	void MeshResourceView::DetachFromPackage( ContentPackage& /*package*/, ContentLibrary& library ) const {
		RemoveFromLibraryAs<MeshResourceView>( library );
	}

// ---------------------------------------------------

	ETNoAliasHint const ::Eldritch2::UTF8Char* const MeshResourceView::GetSerializedDataTag() {
		return UTF8L("E2Mesh");
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

