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

	Mesh::Mesh(Mesh&& mesh) :
		Mesh() {
		Swap(*this, mesh);
	}

	// ---------------------------------------------------

	VkResult Mesh::BindResources(Gpu& gpu, uint32_t vertexCount, uint32_t indexCount) {
		const VkDeviceSize indexSize(indexCount < static_cast<uint16>(-1) ? sizeof(uint16) : sizeof(uint32));
		const VkDeviceSize vertexSize(sizeof(__m128));

		VertexBuffer vertices;
		ET_FAIL_UNLESS(vertices.BindResources(gpu, vertexSize * vertexCount));
		ET_AT_SCOPE_EXIT(vertices.FreeResources(gpu));

		IndexBuffer indices;
		ET_FAIL_UNLESS(indices.BindResources(gpu, indexSize * indexCount));
		ET_AT_SCOPE_EXIT(indices.FreeResources(gpu));

		Swap(_indices, indices);
		Swap(_vertices, vertices);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Mesh::FreeResources(Gpu& gpu) {
		_vertices.FreeResources(gpu);
		_indices.FreeResources(gpu);
	}

	// ---------------------------------------------------

	void Swap(Mesh& lhs, Mesh& rhs) {
		Swap(lhs._vertices, rhs._vertices);
		Swap(lhs._indices, rhs._indices);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
