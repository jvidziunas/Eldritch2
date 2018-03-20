/*==================================================================*\
  OutputWindow.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/OutputWindow.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			VkResult CreateSurface( Vulkan& vulkan, Window& window, VkSurfaceKHR* surface );
		}
	}
}

ETInlineHint bool operator==( VkSurfaceFormatKHR format0, VkSurfaceFormatKHR format1 ) {
	return (format0.format == format1.format) && (format0.colorSpace == format1.colorSpace);
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace {

	template <size_t requestedModeCount>
	VkResult FindBestPresentMode( VkPhysicalDevice device, VkSurfaceKHR surface, VkPresentModeKHR& selectedMode, const VkPresentModeKHR (&requestedModes)[requestedModeCount] ) {
		uint32_t modeCount;

		ET_FAIL_UNLESS( vkGetPhysicalDeviceSurfacePresentModesKHR( device, surface, &modeCount, nullptr ) );
		VkPresentModeKHR* const modes( static_cast<VkPresentModeKHR*>(_alloca( modeCount * sizeof(VkPresentModeKHR) )) );
		VkPresentModeKHR* const end( modes + modeCount );
		ET_FAIL_UNLESS( vkGetPhysicalDeviceSurfacePresentModesKHR( device, surface, &modeCount, modes ) );

		for (VkPresentModeKHR mode : requestedModes) {
			const VkPresentModeKHR* const candidate( Find( modes, end, mode ) );
			if (candidate != end) {
				selectedMode = *candidate;
				return VK_SUCCESS;
			}
		}

		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

// ---------------------------------------------------

	template <size_t requestedFormatCount>
	VkResult FindBestSurfaceFormat( VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceFormatKHR& selectedFormat, VkColorSpaceKHR colorSpace, const VkFormat (&requestedFormats)[requestedFormatCount] ) {
		uint32_t formatCount;

		ET_FAIL_UNLESS( vkGetPhysicalDeviceSurfaceFormatsKHR( device, surface, &formatCount, nullptr ) );
		VkSurfaceFormatKHR* const formats( static_cast<VkSurfaceFormatKHR*>(_alloca( formatCount * sizeof(VkSurfaceFormatKHR) )) );
		VkSurfaceFormatKHR* const end( formats + formatCount );
		ET_FAIL_UNLESS( vkGetPhysicalDeviceSurfaceFormatsKHR( device, surface, &formatCount, formats ) );

	/*	Vulkan returns a 1-element array with format VK_FORMAT_UNDEFINED to indicate that the presentation layer does not impose any restriction on image format.
	 *	Thus, we default to the most-preferred format as specified by the caller. */
		if (formatCount == 1u && formats[0].format == VK_FORMAT_UNDEFINED) {
			selectedFormat = VkSurfaceFormatKHR{ requestedFormats[0], colorSpace };
			return VK_SUCCESS;
		}

		for (VkFormat format : requestedFormats) {
			const VkSurfaceFormatKHR surfaceFormat{ format, colorSpace };

			const VkSurfaceFormatKHR* const candidate( Find( formats, end, surfaceFormat ) );
			if (candidate != end) {
				selectedFormat = *candidate;
				return VK_SUCCESS;
			}
		}

		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

// ---------------------------------------------------

	ETInlineHint VkSwapchainCreateInfoKHR GetCreateInfo(
		const VkSurfaceCapabilitiesKHR& capabilities,
		const VkSurfaceFormatKHR& format,
		VkPresentModeKHR presentMode,
		VkSurfaceKHR surface,
		VkSwapchainKHR oldSwapchain
	) {
		return VkSwapchainCreateInfoKHR{
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			nullptr,				// No extension data.
			0u,
			surface,
			capabilities.maxImageCount,
			format.format,
			format.colorSpace,
			capabilities.currentExtent,
			1u,						// Single layer.
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0u, nullptr,			// No queue families for sharing.
			capabilities.currentTransform,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			presentMode,
			VK_TRUE,				//	We don't read back swapchain images, so allow discard optimizations.
			oldSwapchain
		};
	}

}	// anonymous namespace

	OutputWindow::OutputWindow(
		OutputWindow&& window
	) : _referenceCount( 1u ),
		_window( eastl::move( window._window ) ),
		_surface( eastl::exchange( window._surface, nullptr ) ),
		_swapchain( eastl::exchange( window._swapchain, nullptr ) ),
		_imageCount( eastl::exchange( window._imageCount, 0 ) ),
		_imageViews(),
		_imageAvailable( eastl::move( window._imageAvailable ) ),
		_canComposite( eastl::move( window._canComposite ) ) {
	}

// ---------------------------------------------------

	OutputWindow::OutputWindow(
	) : _referenceCount( 1u ),
		_window(),
		_surface( nullptr ),
		_imageCount( 0u ),
		_imageViews{ nullptr },
		_imageAvailable(),
		_canComposite() {
	}

// ---------------------------------------------------

	OutputWindow::~OutputWindow() {
		ET_ASSERT( _surface == nullptr,        "Leaking Vulkan surface!" );
		ET_ASSERT( _imageAvailable == nullptr, "Leaking Vulkan fence!" );
		ET_ASSERT( _canComposite == nullptr,   "Leaking Vulkan event!" );
	}

// ---------------------------------------------------

	VkResult OutputWindow::AcquireImage( Gpu& gpu ) {
		enum : uint64 { InfiniteTimeout = ULONG_MAX };
		
		VkResult result( ET_LIKELY( _swapchain != nullptr ) ? vkAcquireNextImageKHR( gpu, _swapchain, InfiniteTimeout, nullptr, nullptr, &_presentableIndex ) : VK_ERROR_OUT_OF_DATE_KHR );

		while (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			VkSurfaceFormatKHR format{ VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
			ET_FAIL_UNLESS( FindBestSurfaceFormat( gpu, _surface, format, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, {VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_R8G8B8A8_SRGB, VK_FORMAT_A8B8G8R8_SRGB_PACK32} ) );

			VkPresentModeKHR presentMode;
			ET_FAIL_UNLESS( FindBestPresentMode( gpu, _surface, presentMode, {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR, VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR} ) );

			VkSurfaceCapabilitiesKHR capabilities;
			ET_FAIL_UNLESS( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( gpu, _surface, &capabilities ) );
			if (capabilities.minImageCount > MaxQueueDepth) {
				return VK_ERROR_FEATURE_NOT_PRESENT;
			}

		//	The Vulkan runtime indicates that a swapchain does not impose a hard limit on the number of images by returning 0 in the maxImageCount field.
			if (capabilities.maxImageCount == 0u) {
				capabilities.maxImageCount = MaxQueueDepth;
			}

			const VkSwapchainCreateInfoKHR	createInfo( GetCreateInfo( capabilities, format, presentMode, _surface, _swapchain ) );
			VkSwapchainKHR					swapchain;

			result = vkCreateSwapchainKHR( gpu, &createInfo, gpu.GetAllocationCallbacks(), &swapchain );

			vkDestroySwapchainKHR( gpu, eastl::exchange( _swapchain, swapchain ), gpu.GetAllocationCallbacks() );

			if (Succeeded( result )) {
				result = vkAcquireNextImageKHR( gpu, _swapchain, InfiniteTimeout, nullptr, nullptr, &_presentableIndex );
			}
		}

		return result;
	}

// ---------------------------------------------------

	VkResult OutputWindow::BindResources( Vulkan& vulkan, Gpu& gpu ) {
		using ::Eldritch2::Swap;

		Window window;
		ET_FAIL_UNLESS( Succeeded( window.BindResources() ) ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED );
		ET_AT_SCOPE_EXIT( window.FreeResources() );

		VkSurfaceKHR surface;
		ET_FAIL_UNLESS( CreateSurface( vulkan, window, &surface ) );
		ET_AT_SCOPE_EXIT( vkDestroySurfaceKHR( vulkan, surface, nullptr ) );

		VkFence imageAvailable;
		const VkFenceCreateInfo imageAvailableInfo{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			nullptr,
			VK_FENCE_CREATE_SIGNALED_BIT
		};

		ET_FAIL_UNLESS( vkCreateFence( gpu, &imageAvailableInfo, gpu.GetAllocationCallbacks(), &imageAvailable ) );
		ET_AT_SCOPE_EXIT( vkDestroyFence( gpu, imageAvailable, gpu.GetAllocationCallbacks() ) );

		VkEvent canComposite;
		const VkEventCreateInfo canCompositeInfo{
			VK_STRUCTURE_TYPE_EVENT_CREATE_INFO,
			nullptr,
			0u
		};

		ET_FAIL_UNLESS( vkCreateEvent( gpu, &canCompositeInfo, gpu.GetAllocationCallbacks(), &canComposite ) );
		ET_AT_SCOPE_EXIT( vkDestroyEvent( gpu, canComposite, gpu.GetAllocationCallbacks() ) );

		Swap( _surface,			surface );
		Swap( _imageAvailable,	imageAvailable );
		Swap( _canComposite,	canComposite );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	VkResult OutputWindow::BindImages( Gpu& gpu ) {
		using ::Eldritch2::Swap;

		VkImage	images[MaxQueueDepth];
		uint32	imageCount( _countof(images) );
		ET_FAIL_UNLESS( vkGetSwapchainImagesKHR( gpu, _swapchain, &imageCount, images ) );

		VkImageViewCreateInfo viewInfo{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			nullptr,	// No extensions.
			0u,			// No create flags.
			nullptr,	// Image will be set below.
			VK_IMAGE_VIEW_TYPE_2D,
			VK_FORMAT_UNDEFINED,
			MakeIdentitySwizzle(),
			VkImageSubresourceRange{
				VK_IMAGE_ASPECT_COLOR_BIT,
				0u,		//	'Identity' MIP level. 
				1u,		//	Single-level MIP chain.
				0u,		//	'Identity' array region.
				1u
			}
		};

		VkImageView views[_countof(images)];
		ET_AT_SCOPE_EXIT( for (VkImageView view : views) vkDestroyImageView( gpu, view, gpu.GetAllocationCallbacks() ) );
		for (uint32 image( 0 ); image < imageCount; ++image) {
			if (images[image] == nullptr) {
				break;
			}

			viewInfo.image = images[image];

			ET_FAIL_UNLESS( vkCreateImageView( gpu, &viewInfo, gpu.GetAllocationCallbacks(), views + image ) );
		}

		Swap( _imageViews, views );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void OutputWindow::FreeResources( Vulkan& vulkan, Gpu& gpu ) {
		vkDestroyEvent( gpu, eastl::exchange( _canComposite, nullptr ), gpu.GetAllocationCallbacks() );
		vkDestroyFence( gpu, eastl::exchange( _imageAvailable, nullptr ), gpu.GetAllocationCallbacks() );

		for (VkImageView& view : _imageViews) {
			vkDestroyImageView( gpu, eastl::exchange( view, nullptr ), gpu.GetAllocationCallbacks() );
		}

		vkDestroySurfaceKHR( vulkan, eastl::exchange( _surface, nullptr ), nullptr );
	}

// ---------------------------------------------------

	void Swap( OutputWindow& window0, OutputWindow& window1 ) {
		using ::Eldritch2::Swap;

		Swap( window0._window,           window1._window );
		Swap( window0._surface,          window1._surface );
		Swap( window0._imageCount,       window1._imageCount );
		Swap( window0._presentableIndex, window1._presentableIndex );
		Swap( window0._imageViews,       window1._imageViews );
		Swap( window0._imageAvailable,   window1._imageAvailable );
		Swap( window0._canComposite,     window1._canComposite );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2