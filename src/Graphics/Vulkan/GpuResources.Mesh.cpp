/*==================================================================*\
  GpuResources.Mesh.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	Mesh::Mesh() ETNoexceptHint : _vertices(VK_NULL_HANDLE), _indices(VK_NULL_HANDLE), _verticesOffset(0u), _indicesOffset(0u), _verticesSize(0u), _indicesSize(0u) {}

	// ---------------------------------------------------

	Mesh::Mesh(Mesh&& mesh) :
		Mesh() {
		Swap(*this, mesh);
	}

	// ---------------------------------------------------

	VkResult Mesh::BindResources(VertexCache& cache, uint32 verticesSize, uint32 indicesSize) {
		using ::Eldritch2::Swap;

		VkDeviceSize vertexOffset;
		ET_ABORT_UNLESS(cache.AllocateVertices(vertexOffset, verticesSize));
		ET_AT_SCOPE_EXIT(cache.DeallocateVertices(vertexOffset, verticesSize));

		VkDeviceSize indexOffset;
		ET_ABORT_UNLESS(cache.AllocateIndices(indexOffset, indicesSize));
		ET_AT_SCOPE_EXIT(cache.DeallocateIndices(indexOffset, indicesSize));

		Swap(_verticesOffset, vertexOffset);
		Swap(_indicesOffset, indexOffset);
		Swap(_verticesSize, verticesSize);
		Swap(_indicesSize, indicesSize);
		_vertices = cache.GetVertexBuffer();
		_indices  = cache.GetIndexBuffer();

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Mesh::FreeResources(VertexCache& cache) {
		cache.DeallocateVertices(eastl::exchange(_verticesOffset, 0u), eastl::exchange(_verticesSize, 0u));
		cache.DeallocateIndices(eastl::exchange(_indicesOffset, 0u), eastl::exchange(_indicesSize, 0u));
		_vertices = VK_NULL_HANDLE;
		_indices  = VK_NULL_HANDLE;
	}

	// ---------------------------------------------------

	void Swap(Mesh& lhs, Mesh& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._verticesOffset, rhs._verticesOffset);
		Swap(lhs._indicesOffset, rhs._indicesOffset);
		Swap(lhs._verticesSize, rhs._verticesSize);
		Swap(lhs._indicesSize, rhs._indicesSize);
		Swap(lhs._vertices, rhs._vertices);
		Swap(lhs._indices, rhs._indices);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
