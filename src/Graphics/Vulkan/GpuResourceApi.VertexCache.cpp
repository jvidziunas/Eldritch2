/*==================================================================*\
  GpuResourceApi.VertexCache.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	VertexCache::VertexCache() :
		_vertices(),
		_vertexRanges(MallocAllocator("Vulkan Vertex Cache Vertex Free List Allocator")),
		_indices(),
		_indexRanges(MallocAllocator("Vulkan Vertex Cache Index Free List Allocator")) {}

	// ---------------------------------------------------

	VkResult VertexCache::BindResources(Gpu& gpu, VkDeviceSize vertexCacheSize, VkDeviceSize indexCacheSize) {
		using ::Eldritch2::Swap;

		VertexBuffer vertices;
		ET_ABORT_UNLESS(vertices.BindResources(gpu, vertexCacheSize));
		ET_AT_SCOPE_EXIT(vertices.FreeResources(gpu));

		IndexBuffer indices;
		ET_ABORT_UNLESS(indices.BindResources(gpu, indexCacheSize));
		ET_AT_SCOPE_EXIT(indices.FreeResources(gpu));

		Swap(_vertices, vertices);
		_vertexRanges.Assign(/*begin =*/0u, /*end =*/vertexCacheSize);
		Swap(_indices, indices);
		_indexRanges.Assign(/*begin =*/0u, /*end =*/indexCacheSize);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult VertexCache::AllocateVertices(VkDeviceSize& outOffset, VkDeviceSize size) {
		const auto result(_vertexRanges.Allocate(size));
		if (ET_UNLIKELY(!result.second)) {
			return VK_ERROR_OUT_OF_POOL_MEMORY;
		}

		outOffset = result.first;
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult VertexCache::AllocateIndices(VkDeviceSize& outOffset, VkDeviceSize size) {
		const auto result(_indexRanges.Allocate(size));
		if (ET_UNLIKELY(!result.second)) {
			return VK_ERROR_OUT_OF_POOL_MEMORY;
		}

		outOffset = result.first;
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void VertexCache::DeallocateVertices(VkDeviceSize offset, VkDeviceSize size) {
		if (ET_UNLIKELY(size == 0u)) {
			return;
		}

		_vertexRanges.Deallocate(/*range =*/ { offset, offset + size });
	}

	// ---------------------------------------------------

	void VertexCache::DeallocateIndices(VkDeviceSize offset, VkDeviceSize size) {
		if (ET_UNLIKELY(size == 0u)) {
			return;
		}

		_indexRanges.Deallocate(/*range =*/ { offset, offset + size });
	}

	// ---------------------------------------------------

	void VertexCache::FreeResources(Gpu& gpu) {
		_indexRanges.Clear();
		_indices.FreeResources(gpu);
		_vertexRanges.Clear();
		_vertices.FreeResources(gpu);
	}

	// ---------------------------------------------------

	void Swap(VertexCache& lhs, VertexCache& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._vertices, rhs._vertices);
		Swap(lhs._vertexRanges, rhs._vertexRanges);
		Swap(lhs._indices, rhs._indices);
		Swap(lhs._indexRanges, rhs._indexRanges);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
