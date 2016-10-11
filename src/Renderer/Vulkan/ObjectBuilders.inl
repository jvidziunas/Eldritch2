/*==================================================================*\
  ObjectBuilders.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	ETInlineHint MipRegion::MipRegion( uint32_t firstMip, uint32_t mipCount ) : firstMip( firstMip ), mipCount( mipCount ) {}

// ---------------------------------------------------

	ETInlineHint ArrayRegion::ArrayRegion( uint32_t firstSlice, uint32_t sliceCount ) : firstSlice( firstSlice ), sliceCount( sliceCount ) {}

// ---------------------------------------------------

	ETInlineHint CubeRegion::CubeRegion( uint32_t firstCube, uint32_t cubeCount ) : ArrayRegion( 6u * firstCube, ( cubeCount != VK_REMAINING_ARRAY_LAYERS ? 6u * cubeCount : VK_REMAINING_ARRAY_LAYERS ) ) {}

// ---------------------------------------------------

	ETInlineHint InstanceBuilder& InstanceBuilder::EnableExtension( const char* const name ) {
		_enabledExtensions.Insert( name );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint InstanceBuilder& InstanceBuilder::DisableExtension( const char* const name ) {
		_enabledExtensions.Erase( name );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint InstanceBuilder& InstanceBuilder::EnableLayer( const char* const name ) {
		_enabledLayers.Insert( name );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint InstanceBuilder& InstanceBuilder::DisableLayer( const char* const name ) {
		_enabledLayers.Erase( name );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint LogicalDeviceBuilder& LogicalDeviceBuilder::EnableExtension( const char* const name ) {
		_enabledExtensions.Insert( name );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint LogicalDeviceBuilder& LogicalDeviceBuilder::DisableExtension( const char* const name ) {
		_enabledExtensions.Erase( name );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint LogicalDeviceBuilder& LogicalDeviceBuilder::EnableLayer( const char* const name ) {
		_enabledLayers.Insert( name );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint LogicalDeviceBuilder& LogicalDeviceBuilder::DisableLayer( const char* const name ) {
		_enabledLayers.Erase( name );

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ImageBuilder& ImageBuilder::SetTiling( VkImageTiling tiling ) {
		_createInfo.tiling = tiling;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ImageBuilder& ImageBuilder::SetFormat( VkFormat format ) {
		_createInfo.format = format;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ImageBuilder& ImageBuilder::SetMipCount( uint32_t mipCount ) {
		_createInfo.mipLevels = mipCount;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ImageViewBuilder& ImageViewBuilder::SetIdentitySwizzle() {
		return SetSwizzle( VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY, VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY, VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY, VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY );
	}

// ---------------------------------------------------

	ETInlineHint ImageViewBuilder& ImageViewBuilder::SetSwizzle( VkComponentSwizzle r, VkComponentSwizzle g, VkComponentSwizzle b, VkComponentSwizzle a ) {
		_createInfo.components = { r, g, b, a };

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ImageViewBuilder& ImageViewBuilder::SetFormat( VkFormat format ) {
		_createInfo.format = format;
		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ImageViewBuilder& ImageViewBuilder::SetMipRegion( MipRegion region ) {
		_createInfo.subresourceRange.baseMipLevel	= region.firstMip;
		_createInfo.subresourceRange.levelCount		= region.mipCount;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ImageViewBuilder& ImageViewBuilder::SetArrayRegion( ArrayRegion region ) {
		_createInfo.subresourceRange.baseArrayLayer = region.firstSlice;
		_createInfo.subresourceRange.layerCount		= region.sliceCount;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ShaderModuleBuilder& ShaderModuleBuilder::SetBytecode( const void* bytecodeBegin, size_t bytecodeLengthInBytes ) {
		_createInfo.pCode		= static_cast<const uint32_t*>( bytecodeBegin );
		_createInfo.codeSize	= bytecodeLengthInBytes / sizeof(uint32_t);
		return *this;
	}

// ---------------------------------------------------

#if defined( VK_KHR_swapchain )
	ETInlineHint SwapChainBuilder& SwapChainBuilder::SetOldSwapchain( VkSwapchainKHR oldSwapchain ) {
		_createInfo.oldSwapchain = oldSwapchain;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint SwapChainBuilder& SwapChainBuilder::SetExtent( ::VkExtent2D extent ) {
		_createInfo.imageExtent = extent;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint SwapChainBuilder& SwapChainBuilder::SetUsage( VkImageUsageFlags usageFlags ) {
		_createInfo.imageUsage = usageFlags;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint SwapChainBuilder& SwapChainBuilder::SetImageCount( uint32_t bufferCount ) {
		_createInfo.minImageCount = bufferCount;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint SwapChainBuilder& SwapChainBuilder::SetSlicesPerImage( uint32_t sliceCount ) {
		_createInfo.imageArrayLayers = sliceCount;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint SwapChainBuilder& SwapChainBuilder::SetFormatAndColorSpace( VkFormat imageFormat, VkColorSpaceKHR imageColorSpace ) {
		_createInfo.imageFormat		= imageFormat;
		_createInfo.imageColorSpace	= imageColorSpace;

		return *this;
	}
#endif	// defined( VK_KHR_swapchain )

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2