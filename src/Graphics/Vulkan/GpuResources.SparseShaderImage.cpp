/*==================================================================*\
  ResidencyCoordinator.SparseImage.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/GpuHeap.hpp>
#include <Graphics/ImageSource.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	SparseShaderImage::SparseShaderImage(
	) : _residentTilesByCoordinate( MallocAllocator( "Sparse Image GPU Resident Set Allocator" ) ),
		_cachedTilesByCoordinate( MallocAllocator( "Sparse Image Cached Set Allocator" ) ),
		_pageManager(),
		_cache() {
	}

// ---------------------------------------------------

	SparseShaderImage::SparseShaderImage(
		SparseShaderImage&& image
	) : _residentTilesByCoordinate( eastl::move( image._residentTilesByCoordinate ) ),
		_cachedTilesByCoordinate( eastl::move( image._cachedTilesByCoordinate ) ),
		_pageManager( eastl::move( image._pageManager ) ),
		_cache( eastl::move( image._cache ) ) {}

// ---------------------------------------------------

	VkResult SparseShaderImage::Upload( IoBuilder& ioBuilder ) {
		return VK_SUCCESS;
	}

// ---------------------------------------------------

	bool SparseShaderImage::MakeResident( Tile tile ) {
		if (_residentTilesByCoordinate.ContainsKey( tile )) {
			return false;
		}

		PhysicalTile backing( _pageManager.ReserveTile() );

		if (backing == SparsePageManager::InvalidTile) {
		//	Find LRU tile and acquire backing.
		//	backing = 
			_residentTilesByCoordinate.Erase( candidate );
		}

		_residentTilesByCoordinate.Insert( { tile, backing } );

		return true;
	}

// ---------------------------------------------------

	VkResult SparseShaderImage::BindResources( GpuHeap& heap, VkExtent3D tileExtent, const ImageSource& source ) {
		using ::Eldritch2::Swap;

		const VkDeviceSize	tileSizeInBytes( tileExtent.width * tileExtent.height * tileExtent.depth );

		SparsePageManager pageManager;
		ET_FAIL_UNLESS( pageManager.BindResources( heap, tileExtent, source ) );
		ET_AT_SCOPE_EXIT( pageManager.FreeResources( heap ) );

		TransferBuffer cache;
		ET_FAIL_UNLESS( cache.BindResources( heap, tileSizeInBytes * 128u ) );
		ET_AT_SCOPE_EXIT( cache.FreeResources( heap ) );

		Swap( _pageManager,	pageManager );
		Swap( _cache,		cache );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void SparseShaderImage::FreeResources( GpuHeap& heap ) {
		_cache.FreeResources( heap );
		_pageManager.FreeResources( heap );
	}

// ---------------------------------------------------

	void Swap( SparseShaderImage& image0, SparseShaderImage& image1 ) {
		using ::Eldritch2::Swap;

		Swap( image0._residentTilesByCoordinate,	image1._residentTilesByCoordinate );
		Swap( image0._cachedTilesByCoordinate,		image1._cachedTilesByCoordinate );
		Swap( image0._pageManager,					image1._pageManager );
		Swap( image0._cache,						image1._cache );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2