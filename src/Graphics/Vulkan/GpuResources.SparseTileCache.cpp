/*==================================================================*\
  GpuResources.SparseTileCache.cpp
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

	SparseTileCache::SparseTileCache() :
		_cachedPagesByTile(MallocAllocator("Tile Cache Cached Page Set Allocator")),
		_loadingTiles(MallocAllocator("Tile Cache Loading Tile Set Allocator")) {}

	// ---------------------------------------------------

	SparseTileCache::SparseTileCache(SparseTileCache&& cache) :
		SparseTileCache() {
		Swap(*this, cache);
	}

	// ---------------------------------------------------

	bool SparseTileCache::ShouldCacheTile(Tile tile, VkDeviceSize& reservedOffset) {
		if (IsLoading(tile)) {
			return false;
		}

		_loadingTiles.Insert(tile);
		reservedOffset = 0u;

		return true;
	}

	// ---------------------------------------------------

	void SparseTileCache::NotifyCached(Tile tile, VkDeviceSize offset) {
		_loadingTiles.Erase(tile);
		_cachedPagesByTile.Emplace(tile, offset);
	}

	// ---------------------------------------------------

	VkResult SparseTileCache::BindResources(Gpu& gpu, VkDeviceSize tileSizeInBytes, uint32_t cachedTileLimit) {
		enum : VkMemoryPropertyFlags { InferFromUsage = 0u };

		ET_FAIL_UNLESS(AbstractBuffer::BindResources(gpu, VkBufferCreateInfo{
															  VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
															  /*pNext =*/nullptr,
															  /*flags =*/0u, tileSizeInBytes * cachedTileLimit, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE,
															  /*queueFamilyIndexCount =*/0u,   // Exclusive sharing.
															  /*pQueueFamilyIndices =*/nullptr // Exclusive sharing.
														  },
													 VmaAllocationCreateInfo{ VMA_ALLOCATION_CREATE_MAPPED_BIT, VMA_MEMORY_USAGE_CPU_ONLY,
																			  /*requiredFlags =*/InferFromUsage,
																			  /*preferredFlags =*/InferFromUsage,
																			  /*memoryTypeBits =*/0u,
																			  /*pool =*/VK_NULL_HANDLE,
																			  /*pUserData =*/this }));

		HashMap<Tile, VkDeviceSize> cachedPagesByTile(_cachedPagesByTile.GetAllocator());
		HashSet<Tile>               loadingTiles(_loadingTiles.GetAllocator());

		Swap(_cachedPagesByTile, cachedPagesByTile);
		Swap(_loadingTiles, loadingTiles);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void SparseTileCache::FreeResources(Gpu& gpu) {
		AbstractBuffer::FreeResources(gpu);
		_cachedPagesByTile.Clear();
		_loadingTiles.Clear();
	}

	// ---------------------------------------------------

	void Swap(SparseTileCache& lhs, SparseTileCache& rhs) {
		Detail::Swap(lhs, rhs);
		Swap(lhs._cachedPagesByTile, rhs._cachedPagesByTile);
		Swap(lhs._loadingTiles, rhs._loadingTiles);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
