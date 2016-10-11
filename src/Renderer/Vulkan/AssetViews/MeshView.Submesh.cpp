/*==================================================================*\
  MeshView.Submesh.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/AssetViews/MeshView.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Assets;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace AssetViews {

	MeshView::Submesh::Submesh( VkBuffer buffer,
								VkDeviceSize vertexBufferOffset,
								VkDeviceSize indexBufferOffset,
								uint16 indexCount,
								uint16 indicesPerPrimitive,
								uint32 firstIndex,
								const SpirVShaderPipelineView& pipeline,
								Range<const VkVertexInputBindingDescription*> inputBindings,
								Range<const VkVertexInputAttributeDescription*> inputAttributes ) : _buffer( buffer ),
																									_vertexBufferOffset( vertexBufferOffset ),
																									_indexBufferOffset( indexBufferOffset ),
																									_indexCount( indexCount ),
																									_indicesPerPrimitive( indicesPerPrimitive ),
																									_firstIndex( firstIndex ),
																									_pipeline( &pipeline ),
																									_bindings( inputBindings ),
																									_attributes( inputAttributes ) {}

// ---------------------------------------------------

	void MeshView::Submesh::BindMeshBuffers( VkCommandBuffer commandBuffer ) const {
		enum : uint32_t {
			PerMeshBufferSlot	= 1u
		};

		const VkBuffer	buffersToBind[] = { _buffer };

		vkCmdBindVertexBuffers( commandBuffer, PerMeshBufferSlot, _countof( buffersToBind ), buffersToBind, &_vertexBufferOffset );
		vkCmdBindIndexBuffer( commandBuffer, _buffer, _indexBufferOffset, VkIndexType::VK_INDEX_TYPE_UINT16 );
	}

// ---------------------------------------------------

	void MeshView::Submesh::IssueDraw( VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance ) const {
		BindMeshBuffers( commandBuffer );
		vkCmdDrawIndexed( commandBuffer, _indexCount, instanceCount, _firstIndex, 0u, firstInstance );
	}

} // namespace AssetViews
} // namespace Vulkan
} // namespace Renderer
} // namespace Eldritch2