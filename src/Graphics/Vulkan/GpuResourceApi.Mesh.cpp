/*==================================================================*\
  GpuResourceApi.Mesh.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		enum : VkDeviceSize {
			InvalidOffset = ~VkDeviceSize(0u),
			InvalidSize   = VkDeviceSize(0u),
		};

	} // anonymous namespace

	Mesh::Mesh() ETNoexceptHint : _vertices(nullptr), _indices(nullptr), _verticesOffset(InvalidOffset), _indicesOffset(InvalidOffset), _verticesSize(InvalidSize), _indicesSize(InvalidSize) {}

	// ---------------------------------------------------

	Mesh::Mesh(Mesh&& mesh) ETNoexceptHint : Mesh() {
		Swap(*this, mesh);
	}

	// ---------------------------------------------------

	VkResult Mesh::BindResources(GeometryCache& cache, const MeshDescriptor& description) {
		using ::Eldritch2::Swap;

		VkDeviceSize indexOffset, indicesSize(description.streamSizes[0]);
		ET_ABORT_UNLESS(cache.AllocateIndices(indexOffset, indicesSize));
		ET_AT_SCOPE_EXIT(cache.DeallocateIndices(indexOffset, indicesSize));

		VkDeviceSize vertexOffset, verticesSize(description.streamSizes[1]);
		ET_ABORT_UNLESS(cache.AllocateVertices(vertexOffset, verticesSize));
		ET_AT_SCOPE_EXIT(cache.DeallocateVertices(vertexOffset, verticesSize));

		Swap(_indicesOffset, indexOffset);
		Swap(_verticesOffset, vertexOffset);
		Swap(_indicesSize, indicesSize);
		Swap(_verticesSize, verticesSize);
		_vertices = cache.GetVertexBuffer();
		_indices  = cache.GetIndexBuffer();

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Mesh::FreeResources(GeometryCache& cache) {
		cache.DeallocateVertices(Exchange(_verticesOffset, InvalidOffset), Exchange(_verticesSize, InvalidSize));
		cache.DeallocateIndices(Exchange(_indicesOffset, InvalidOffset), Exchange(_indicesSize, InvalidSize));
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
