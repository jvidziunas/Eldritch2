/*==================================================================*\
  SparseImage.HardwareSparseImagePageManager.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SparseImage.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	HardwareSparseImagePageManager::HardwareSparseImagePageManager( VkExtent3D					tileExtent,
																	uint32						maximumResidentTileCount,
																	UniquePointer<VkImage>		image,
																	UniquePointer<VkImageView>	imageView,
																	Allocator&					allocator ) : SparseImagePageManager( tileExtent, 32768u ),
																											  _image( eastl::move( image ) ),
																											  _imageView( eastl::move( imageView ) ),
																											  _bindPool( GetTileSizeInBytes() * maximumResidentTileCount, { allocator, "Vulkan Sparse Image Coordinate Pool Allocator" } ) {
	//	First page is used as a sentinel value, so 'reserve' it here.
		const auto	sentinel( _bindPool.AllocateRangeOfSize( GetTileSizeInBytes() ) );

	//	Test to make sure the sentinel is what we expect.
		ETRuntimeAssert( InvalidPhysicalCoordinate == sentinel.first );
	}

// ---------------------------------------------------

	HardwareSparseImagePageManager::HardwareSparseImagePageManager( HardwareSparseImagePageManager&& pageManager ) : SparseImagePageManager( eastl::move( pageManager ) ),
																													 _memory( eastl::move( pageManager._memory ) ),
																													 _image( eastl::move( pageManager._image ) ),
																													 _imageView( eastl::move( pageManager._imageView ) ),
																													 _bindPool( eastl::move( pageManager._bindPool ) ) {}

// ---------------------------------------------------

	SparseImagePhysicalCoordinate HardwareSparseImagePageManager::Reserve( SparseImageVirtualCoordinate coordinate ) {
		const auto	allocateResult( _bindPool.AllocateRangeOfSize( GetTileSizeInBytes() ) );

		return allocateResult.second ? allocateResult.first : InvalidPhysicalCoordinate;
	}

// ---------------------------------------------------

	void HardwareSparseImagePageManager::Release( SparseImagePhysicalCoordinate coordinate ) {
		_bindPool.ReleaseRange( { coordinate, GetTileSizeInBytes() } );
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2