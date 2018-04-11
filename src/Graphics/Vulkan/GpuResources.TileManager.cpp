/*==================================================================*\
  GpuResources.TileManager.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace {

	static constexpr VkExtent3D DefaultTileExtent{ 1u, 1u, 1u };
	static constexpr VkExtent3D DefaultExtent{ 0u, 0u, 0u };

}	// anonymous namespace

	TileManager::TileManager(
		VkExtent3D tileExtent,
		VkExtent3D extent
	) : _logTileExtent( tileExtent ),
		_extent( extent ),
		_residentTiles( MallocAllocator( "Tile Manager GPU Resident Tile Set Allocator" ) ),
		_cachedPagesByTile( MallocAllocator( "Tile Manager Cached Page Set Allocator" ) ),
		_loadingTiles( MallocAllocator( "Tile Manager Loading Tile Set Allocator" ) ) {
	}

// ---------------------------------------------------

	TileManager::TileManager(
		TileManager&& tileManager
	) :	_logTileExtent( eastl::exchange( tileManager._logTileExtent, DefaultTileExtent ) ),
		_extent( eastl::exchange( tileManager._extent, DefaultExtent ) ),
		_residentTiles( eastl::move( tileManager._residentTiles ) ),
		_cachedPagesByTile( eastl::move( tileManager._cachedPagesByTile ) ),
		_loadingTiles( eastl::move( tileManager._loadingTiles ) ) {
	}

// ---------------------------------------------------

	TileManager::TileManager() : TileManager( DefaultTileExtent, DefaultExtent ) {}

// ---------------------------------------------------

	TileManager::CacheResult TileManager::MakeResident( Tile tile ) {
	//	Attempt to locate the tile in the GPU resident tile set.
		if (IsResident( tile ) || IsLoading( tile )) {
			return CacheResult::Hit;
		}

	//	Attempt to locate the tile in the host cache of recently-accessed pages.
		{	TileMap<CachedPage>::Iterator candidate( _cachedPagesByTile.Find( tile ) );
			if (candidate != _cachedPagesByTile.End()) {
			//	_residentTiles.Evict( tile );
			//	Schedule copy operation from cache -> GPU image.
				return CacheResult::L0Miss;
			}
		}

	//	To consider: Should we evict least- or most-recently-used for the L1 page cache?
	//	Load( tile, _cachedPagesByTile.Evict( tile ) );
		_loadingTiles.Insert( tile );

		return CacheResult::L1Miss;
	}

// ---------------------------------------------------

	void TileManager::NotifyCached( Tile tile, CachedPage page ) {
		_cachedPagesByTile.Emplace( tile, page );
		_loadingTiles.Erase( tile );
	}

// ---------------------------------------------------

	void Swap( TileManager& lhs, TileManager& rhs ) {
		using ::Eldritch2::Swap;

		Swap( lhs._logTileExtent,     rhs._logTileExtent );
		Swap( lhs._extent,            rhs._extent );
		Swap( lhs._residentTiles,     rhs._residentTiles );
		Swap( lhs._cachedPagesByTile, rhs._cachedPagesByTile );
		Swap( lhs._loadingTiles,      rhs._loadingTiles );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2