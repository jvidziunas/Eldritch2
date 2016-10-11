/*==================================================================*\
  ObjectBuilders.ImageViewBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ObjectBuilders.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	ImageViewBuilder::ImageViewBuilder() {}

// ---------------------------------------------------

	ImageViewBuilder& ImageViewBuilder::SetImage1D( MipRegion mipRegion, uint32_t sliceInArray ) {
		_createInfo.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_1D;

		return SetMipRegion( mipRegion ).SetArrayRegion( { sliceInArray, 1u } );
	}

// ---------------------------------------------------

	ImageViewBuilder& ImageViewBuilder::SetImage1DArray( MipRegion mipRegion, ArrayRegion arrayRegion ) {
		_createInfo.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_1D_ARRAY;

		return SetMipRegion( mipRegion ).SetArrayRegion( arrayRegion );
	}

// ---------------------------------------------------

	ImageViewBuilder& ImageViewBuilder::SetImage2D( MipRegion mipRegion, uint32_t sliceInArray ) {
		_createInfo.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_2D;

		return SetMipRegion( mipRegion ).SetArrayRegion( { sliceInArray, 1u } );
	}

// ---------------------------------------------------

	ImageViewBuilder& ImageViewBuilder::SetImage2DArray( MipRegion mipRegion, ArrayRegion arrayRegion ) {
		_createInfo.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_2D;

		return SetMipRegion( mipRegion ).SetArrayRegion( arrayRegion );
	}

// ---------------------------------------------------

	ImageViewBuilder& ImageViewBuilder::SetImage3D( MipRegion mipRegion ) {
		_createInfo.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_3D;

		return SetMipRegion( mipRegion ).SetArrayRegion( { 0u, 1u } );
	}

// ---------------------------------------------------

	ImageViewBuilder& ImageViewBuilder::SetImageCube( MipRegion mipRegion, uint32_t firstCubemap ) {
		_createInfo.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE;

		return SetMipRegion( mipRegion ).SetArrayRegion( CubeRegion( firstCubemap, 1u ) );
	}

// ---------------------------------------------------

	ImageViewBuilder& ImageViewBuilder::SetImageCubeArray( MipRegion mipRegion, CubeRegion cubeRegion ) {
		_createInfo.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;

		return SetMipRegion( mipRegion ).SetArrayRegion( cubeRegion );
	}

// ---------------------------------------------------

	VulkanResult<VkImageView> ImageViewBuilder::Create( VkDevice device, VkImage image, const VkAllocationCallbacks* callbacks ) {
		_createInfo.image = image;

		VkImageView	view( VK_NULL_HANDLE );
		const auto	createViewResult( vkCreateImageView( device, &_createInfo, callbacks, &view ) );

		if( createViewResult < VkResult::VK_SUCCESS ) {
			return { createViewResult };
		}

		return Vulkan::UniquePointer<VkImageView>( view, { device, callbacks } );
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2