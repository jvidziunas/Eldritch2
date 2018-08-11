/*==================================================================*\
  Display.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Display.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	VkResult CreateSurface(Vulkan& vulkan, Window& window, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface);
}}} // namespace Eldritch2::Graphics::Vulkan

ETInlineHint bool operator==(VkSurfaceFormatKHR lhs, VkSurfaceFormatKHR rhs) {
	return (lhs.format == rhs.format) && (lhs.colorSpace == rhs.colorSpace);
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
	namespace {

		constexpr VkPresentModeKHR immediateModes[] = { VK_PRESENT_MODE_IMMEDIATE_KHR };
		constexpr VkPresentModeKHR vsyncModes[]     = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR, VK_PRESENT_MODE_FIFO_KHR };

		ETInlineHint VkResult BindFramebuffer(Gpu& gpu, VkImage image, VkFormat format, VkExtent2D extent, VkImageView& outView, VkFramebuffer& outFramebuffer) {
			const VkImageViewCreateInfo viewInfo {
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u,
				image,
				VK_IMAGE_VIEW_TYPE_2D,
				format,
				MakeIdentitySwizzle(),
				VkImageSubresourceRange {
					VK_IMAGE_ASPECT_COLOR_BIT,
					0u,
					1u,
					0u,
					1u }
			};
			ET_FAIL_UNLESS(vkCreateImageView(gpu, &viewInfo, gpu.GetAllocationCallbacks(), &outView));
			const VkFramebufferCreateInfo framebufferInfo {
				VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u,
				nullptr,
				/*attachmentCount =*/1u,
				&outView,
				extent.width,
				extent.height,
				1u
			};
			return vkCreateFramebuffer(gpu, &framebufferInfo, gpu.GetAllocationCallbacks(), &outFramebuffer);
		}

		// ---------------------------------------------------

		ETInlineHint VkResult CheckSurfacePresentationSupport(Gpu& gpu, VkSurfaceKHR surface) {
			VkBool32 canPresent;
			ET_FAIL_UNLESS(vkGetPhysicalDeviceSurfaceSupportKHR(gpu, gpu.GetQueueFamilyByConcept(QueueConcept::Presentation), surface, &canPresent));

			return canPresent == VK_TRUE ? VK_SUCCESS : VK_ERROR_FEATURE_NOT_PRESENT;
		}

		// ---------------------------------------------------

		template <size_t count>
		ETInlineHint VkResult FindBestPresentMode(VkPhysicalDevice device, VkSurfaceKHR surface, VkPresentModeKHR& selectedMode, const VkPresentModeKHR (&requestedModes)[count]) {
			uint32_t modeCount;

			ET_FAIL_UNLESS(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, nullptr));
			VkPresentModeKHR* const modes(ETStackAlloc(VkPresentModeKHR, modeCount));
			VkPresentModeKHR* const end(modes + modeCount);
			ET_FAIL_UNLESS(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, modes));

			for (VkPresentModeKHR mode : requestedModes) {
				const VkPresentModeKHR* const candidate(Find(modes, end, mode));
				if (candidate != end) {
					selectedMode = *candidate;
					return VK_SUCCESS;
				}
			}

			return VK_ERROR_FEATURE_NOT_PRESENT;
		}

		// ---------------------------------------------------

		template <size_t count>
		ETInlineHint VkResult FindBestSurfaceFormat(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceFormatKHR& selectedFormat, VkColorSpaceKHR colorSpace, const VkFormat (&requestedFormats)[count]) {
			uint32_t formatCount;

			ET_FAIL_UNLESS(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr));
			VkSurfaceFormatKHR* const formats(ETStackAlloc(VkSurfaceFormatKHR, formatCount));
			VkSurfaceFormatKHR* const end(formats + formatCount);
			ET_FAIL_UNLESS(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats));

			/*	Vulkan returns a 1-element array with format VK_FORMAT_UNDEFINED to indicate that the presentation layer does not impose any restriction on image format.
				Thus, we default to the most-preferred format as specified by the caller. */
			if (formatCount == 1u && formats[0].format == VK_FORMAT_UNDEFINED) {
				selectedFormat = VkSurfaceFormatKHR { requestedFormats[0], colorSpace };
				return VK_SUCCESS;
			}

			for (VkFormat format : requestedFormats) {
				const VkSurfaceFormatKHR        surfaceFormat { format, colorSpace };
				const VkSurfaceFormatKHR* const candidate(Find(formats, end, surfaceFormat));
				if (candidate != end) {
					selectedFormat = *candidate;
					return VK_SUCCESS;
				}
			}

			return VK_ERROR_FEATURE_NOT_PRESENT;
		}

	} // anonymous namespace

	DisplaySource::DisplaySource(DisplaySource&& source) :
		DisplaySource() {
		Swap(*this, source);
	}

	// ---------------------------------------------------

	Display::Display() :
		_window(),
		_surface(nullptr),
		_format { VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR },
		_presentMode(VK_PRESENT_MODE_FIFO_KHR),
		_swapchain(nullptr),
		_swapchainIndex(0u),
		_compositeInfos {},
		_sources { nullptr } {
	}

	// ---------------------------------------------------

	Display::Display(Display&& output) :
		Display() {
		Swap(*this, output);
	}

	// ---------------------------------------------------

	Display::~Display() {
		ET_ASSERT(_surface == nullptr, "Leaking Vulkan surface!");
		ET_ASSERT(_swapchain == nullptr, "Leaking Vulkan swapchain!");
	}

	// ---------------------------------------------------

	VkResult Display::AcquireImage(Vulkan& vulkan, Gpu& gpu) {
		if (ET_UNLIKELY(!_window.Exists())) {
			ET_FAIL_UNLESS(BindResources(vulkan, gpu));

			_window.SetDisplayMode({ FullscreenMode::WindowedFullscreen, 0u, 0u });
			_window.EnsureVisible();
		}

		CompositeInfo& compositeInfo(_compositeInfos[_presentCount % _countof(_compositeInfos)]);

		return vkAcquireNextImageKHR(gpu, _swapchain, ULONG_MAX, compositeInfo.imageReady, nullptr, &_swapchainIndex);
	}

	// ---------------------------------------------------

	VkResult Display::PresentImage(Gpu& gpu) {
		CompositeInfo& compositeInfo(_compositeInfos[_presentCount++ % _countof(_compositeInfos)]);
		VkResult       result;

		ET_FAIL_UNLESS(gpu.PresentAsync(QueueConcept::Presentation, VkPresentInfoKHR { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
																					   /*pNext=*/nullptr,
																					   /*waitSemaphoreCount =*/0u,
																					   /*pWaitSemaphores =*/nullptr,
																					   /*swapchainCount =*/1u,
																					   /*pSwapchains =*/&_swapchain,
																					   /*pImageIndices =*/&_swapchainIndex,
																					   /*pResults =*/&result }));
		if (VK_ERROR_OUT_OF_DATE_KHR == result || VK_SUBOPTIMAL_KHR == result) {
			result = ResizeSwapchain(gpu);
		}

		return result;
	}

	// ---------------------------------------------------

	VkResult Display::ResizeSwapchain(Gpu& gpu) {
		using ::Eldritch2::Swap;

		VkSurfaceCapabilitiesKHR capabilities;
		ET_FAIL_UNLESS(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, _surface, &capabilities));

		VkSwapchainKHR                 swapchain;
		const VkSwapchainCreateInfoKHR swapchainInfo {
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			/*pNext =*/nullptr, // No extension data.
			/*flags =*/0u,
			_surface,
			Max(capabilities.minImageCount, 3u),
			_format.format,
			_format.colorSpace,
			capabilities.currentExtent,
			/*imageArrayLayers =*/1u,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			/*queueFamilyIndexCount =*/0u,
			/*pQueueFamilyIndices =*/nullptr,
			capabilities.currentTransform,
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			_presentMode,
			/*clipped =*/VK_TRUE, // Allow discard optimizations.
			_swapchain
		};
		ET_AT_SCOPE_EXIT(vkDestroySwapchainKHR(gpu, swapchain, gpu.GetAllocationCallbacks()));
		ET_FAIL_UNLESS(vkCreateSwapchainKHR(gpu, &swapchainInfo, gpu.GetAllocationCallbacks(), &swapchain));

		VkImage  images[_countof(_compositeInfos)] = { nullptr };
		uint32_t count;
		ET_FAIL_UNLESS(vkGetSwapchainImagesKHR(gpu, swapchain, &count, nullptr));
		if (ET_UNLIKELY(_countof(images) < count)) {
			return VK_ERROR_FEATURE_NOT_PRESENT;
		}
		ET_FAIL_UNLESS(vkGetSwapchainImagesKHR(gpu, swapchain, &count, images));

		VkImageView   imageViews[_countof(images)]   = { nullptr };
		VkFramebuffer framebuffers[_countof(images)] = { nullptr };
		ET_AT_SCOPE_EXIT(for (VkImageView view
							  : imageViews) vkDestroyImageView(gpu, view, gpu.GetAllocationCallbacks()));
		ET_AT_SCOPE_EXIT(for (VkFramebuffer framebuffer
							  : framebuffers) vkDestroyFramebuffer(gpu, framebuffer, gpu.GetAllocationCallbacks()));
		for (uint32_t image(0u); image < count; ++image) {
			if (images[image] == nullptr) {
				break;
			}

			ET_FAIL_UNLESS(BindFramebuffer(gpu, images[image], _format.format, capabilities.currentExtent, imageViews[image], framebuffers[image]));
		}

		Swap(_swapchain, swapchain);
		Swap(_imageViews, imageViews);
		Swap(_framebuffers, framebuffers);
		Swap(_framebuffers, framebuffers);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult Display::BindResources(Vulkan& vulkan, Gpu& gpu) {
		using ::Eldritch2::Swap;

		Window window;
		ET_FAIL_UNLESS(Succeeded(window.BindResources()) ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED);
		ET_AT_SCOPE_EXIT(window.FreeResources());

		VkSurfaceKHR surface;
		ET_FAIL_UNLESS(CreateSurface(vulkan, window, nullptr, &surface));
		ET_AT_SCOPE_EXIT(vkDestroySurfaceKHR(vulkan, surface, nullptr));
		ET_FAIL_UNLESS(CheckSurfacePresentationSupport(gpu, surface));

		VkSurfaceFormatKHR format;
		ET_FAIL_UNLESS(FindBestSurfaceFormat(gpu, surface, format, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_R8G8B8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM }));

		VkPresentModeKHR presentMode;
		ET_FAIL_UNLESS(FindBestPresentMode(gpu, surface, presentMode, vsyncModes));

		VkSurfaceCapabilitiesKHR capabilities;
		ET_FAIL_UNLESS(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &capabilities));
		if (MaxQueueDepth < capabilities.minImageCount) {
			return VK_ERROR_FEATURE_NOT_PRESENT;
		}

		CompositeInfo compositeInfos[_countof(_compositeInfos)];
		ET_AT_SCOPE_EXIT(for (CompositeInfo& info
							  : compositeInfos) {
			if (info.blitPool) {
				vkFreeCommandBuffers(gpu, info.blitPool, /*commandBufferCount =*/1u, &info.blitCommands);
				vkDestroyCommandPool(gpu, info.blitPool, gpu.GetAllocationCallbacks());
			}
			vkDestroySemaphore(gpu, info.imageReady, gpu.GetAllocationCallbacks());
		});
		for (CompositeInfo& info : compositeInfos) {
			const VkCommandPoolCreateInfo blitPoolInfo {
				VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
				/*pNext =*/nullptr, // No extensions.
				/*flags =*/0u,
				gpu.GetQueueFamilyByConcept(QueueConcept::Presentation)
			};
			ET_FAIL_UNLESS(vkCreateCommandPool(gpu, &blitPoolInfo, gpu.GetAllocationCallbacks(), &info.blitPool));
			const VkCommandBufferAllocateInfo blitCommandsInfo {
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				/*pNext =*/nullptr, // No extensions.
				info.blitPool,
				VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				/*commandBufferCount =*/1u
			};
			ET_FAIL_UNLESS(vkAllocateCommandBuffers(gpu, &blitCommandsInfo, &info.blitCommands));
			const VkSemaphoreCreateInfo imageReadyInfo {
				VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u
			};
			ET_FAIL_UNLESS(vkCreateSemaphore(gpu, &imageReadyInfo, gpu.GetAllocationCallbacks(), &info.imageReady));
		}

		Swap(_window, window);
		Swap(_surface, surface);
		Swap(_format, format);
		Swap(_compositeInfos, compositeInfos);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Display::FreeResources(Vulkan& vulkan, Gpu& gpu) {
		for (CompositeInfo& info : _compositeInfos) {
			vkDestroySemaphore(gpu, eastl::exchange(info.imageReady, nullptr), gpu.GetAllocationCallbacks());
			if (VkCommandPool pool = eastl::exchange(info.blitPool, nullptr)) {
				vkFreeCommandBuffers(gpu, pool, /*commandBufferCount =*/1u, &info.blitCommands);
				vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks());
			}
		}
		for (VkImageView& view : _imageViews) {
			vkDestroyImageView(gpu, eastl::exchange(view, nullptr), gpu.GetAllocationCallbacks());
		}
		for (VkFramebuffer& framebuffer : _framebuffers) {
			vkDestroyFramebuffer(gpu, eastl::exchange(framebuffer, nullptr), gpu.GetAllocationCallbacks());
		}

		vkDestroySwapchainKHR(gpu, eastl::exchange(_swapchain, nullptr), gpu.GetAllocationCallbacks());
		_presentMode = VK_PRESENT_MODE_FIFO_KHR;
		_format      = VkSurfaceFormatKHR { VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		vkDestroySurfaceKHR(vulkan, eastl::exchange(_surface, nullptr), nullptr);
		_window.FreeResources();
	} // namespace Vulkan

	// ---------------------------------------------------

	void Swap(Display& lhs, Display& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._window, rhs._window);
		Swap(lhs._surface, rhs._surface);
		Swap(lhs._format, rhs._format);
		Swap(lhs._presentMode, rhs._presentMode);
		Swap(lhs._swapchain, rhs._swapchain);
		Swap(lhs._swapchainIndex, rhs._swapchainIndex);
		Swap(lhs._compositeInfos, rhs._compositeInfos);
		Swap(lhs._sources, rhs._sources);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
