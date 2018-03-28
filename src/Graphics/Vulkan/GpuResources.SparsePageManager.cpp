/*==================================================================*\
  GpuResources.SparsePageManager.cpp
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

	SparsePageManager::SparsePageManager(
		VkExtent3D tileExtent,
		VkExtent3D extent
	) : _logTileExtent( tileExtent ),
		_extent( extent ),
		_residentTilesByCoordinate( MallocAllocator( "Sparse Image GPU Resident Set Allocator" ) ),
		_cachedTilesByCoordinate( MallocAllocator( "Sparse Image Cached Set Allocator" ) ) {
	}

// ---------------------------------------------------

	SparsePageManager::SparsePageManager(
		SparsePageManager&& pageManager
	) :	_logTileExtent( eastl::exchange( pageManager._logTileExtent, DefaultTileExtent ) ),
		_extent( eastl::exchange( pageManager._extent, DefaultExtent ) ),
		_residentTilesByCoordinate( eastl::move( pageManager._residentTilesByCoordinate ) ),
		_cachedTilesByCoordinate( eastl::move( pageManager._cachedTilesByCoordinate ) ) {
	}

// ---------------------------------------------------

	SparsePageManager::SparsePageManager() : SparsePageManager( DefaultTileExtent, DefaultExtent ) {}

// ---------------------------------------------------

	bool SparsePageManager::MakeResident( SparsePageManager::Tile /*tile*/ ) {
		return false;
	}

// ---------------------------------------------------

	void Swap( SparsePageManager& manager0, SparsePageManager& manager1 ) {
		using ::Eldritch2::Swap;

		Swap( manager0._logTileExtent, manager1._logTileExtent );
		Swap( manager0._extent,        manager1._extent );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2