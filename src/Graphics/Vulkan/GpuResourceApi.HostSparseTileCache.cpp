/*==================================================================*\
  GpuResourceApi.HostSparseTileCache.cpp
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

		ETConstexpr ETForceInlineHint VkDeviceSize GetBytesPerTile(VkExtent3D tileExtent, VkDeviceSize bitsPerTexel) ETNoexceptHint {
			return (tileExtent.width * tileExtent.height * tileExtent.depth * bitsPerTexel) / BitsPerByte;
		}

	} // anonymous namespace

	HostSparseTileCache::HostSparseTileCache() ETNoexceptHint : _cachedPagesByTile(MallocAllocator("Tile Cache Cached Page Set Allocator")), _loadingTiles(MallocAllocator("Tile Cache Loading Tile Set Allocator")) {}

	// ---------------------------------------------------

	HostSparseTileCache::HostSparseTileCache(HostSparseTileCache&& cache) ETNoexceptHint : HostSparseTileCache() {
		Swap(*this, cache);
	}

	// ---------------------------------------------------

	VkResult HostSparseTileCache::BindResources(Gpu& gpu, const TileManager& client, uint32 cachedTiles) {
		enum : VkMemoryPropertyFlags { InferFromUsage = 0u };

		HashMap<TileManager::Tile, VkDeviceSize> cachedPagesByTile(_cachedPagesByTile.GetAllocator());
		HashSet<TileManager::Tile>               loadingTiles(_loadingTiles.GetAllocator());

		ET_ABORT_UNLESS(GpuBuffer::BindResources(gpu, // clang-format off
			VkBufferCreateInfo { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
								/*pNext =*/nullptr,
								/*flags =*/0u,
								/*size =*/cachedTiles * GetBytesPerTile(client.GetTileExtent(), /*bitsPerTexel =*/32u),
								VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
								VK_SHARING_MODE_EXCLUSIVE,
								/*queueFamilyIndexCount =*/0u,
								/*pQueueFamilyIndices =*/nullptr },
			VmaAllocationCreateInfo { VMA_ALLOCATION_CREATE_MAPPED_BIT, VMA_MEMORY_USAGE_CPU_ONLY,
									/*requiredFlags =*/InferFromUsage,
									/*preferredFlags =*/InferFromUsage,
									/*memoryTypeBits =*/0u,
									/*pool =*/VK_NULL_HANDLE,
									/*pUserData =*/this })); // clang-format on
		Swap(_cachedPagesByTile, cachedPagesByTile);
		Swap(_loadingTiles, loadingTiles);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void HostSparseTileCache::FreeResources(Gpu& gpu) {
		GpuBuffer::FreeResources(gpu);
		_cachedPagesByTile.Clear();
		_loadingTiles.Clear();
	}

	// ---------------------------------------------------

	void Swap(HostSparseTileCache& lhs, HostSparseTileCache& rhs) ETNoexceptHint {
		Swap(static_cast<GpuBuffer&>(lhs), static_cast<GpuBuffer&>(rhs));
		Swap(lhs._cachedPagesByTile, rhs._cachedPagesByTile);
		Swap(lhs._loadingTiles, rhs._loadingTiles);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
