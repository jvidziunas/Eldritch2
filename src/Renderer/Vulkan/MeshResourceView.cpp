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

	MeshResourceView::MeshResourceView( ContentLibrary& owningLibrary, ContentPackage& package, const UTF8Char* const name, Allocator& nameAllocator ) : ResourceView( owningLibrary, package, name, nameAllocator ), _vertexBuffer( nullptr ) {}

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

	ErrorCode MeshResourceView::UpdateFromByteStream( const Range<const char*> /*bytes*/ ) {
		_submeshes.Clear();

		return Error::UnsupportedOperation;
	}

// ---------------------------------------------------

	ETNoAliasHint const ::Eldritch2::UTF8Char* const MeshResourceView::GetSerializedDataTag() {
		return UTF8L("E2Mesh");
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

