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
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	VkResult CreateSurface(Gpu& gpu, const Window& window, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface);
}}} // namespace Eldritch2::Graphics::Vulkan

ETInlineHint ETForceInlineHint bool operator==(const VkSurfaceFormatKHR& lhs, const VkSurfaceFormatKHR& rhs) {
	return Eldritch2::OrderBuffers(ETAddressOf(lhs), ETAddressOf(rhs), sizeof(lhs)) == 0;
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	namespace {

		ETInlineHint ETForceInlineHint VkResult GetSurfacePresentationSupport(Gpu& gpu, VkSurfaceKHR surface) ETNoexceptHint {
			VkBool32 canPresent;
			ET_ABORT_UNLESS(vkGetPhysicalDeviceSurfaceSupportKHR(gpu, gpu.GetQueueFamilyByConcept(Presentation), surface, ETAddressOf(canPresent)));

			return canPresent == VK_TRUE ? VK_SUCCESS : VK_ERROR_FEATURE_NOT_PRESENT;
		}

		// ---------------------------------------------------

		template <size_t allowableCount>
		ETInlineHint VkResult FindBestPresentMode(VkPresentModeKHR& outMode, VkPhysicalDevice device, VkSurfaceKHR surface, const VkPresentModeKHR (&allowable)[allowableCount]) ETNoexceptHint {
			uint32_t count;

			ET_ABORT_UNLESS(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, ETAddressOf(count), nullptr));
			const auto modes(ETStackAlloc(VkPresentModeKHR, count));
			const auto end(modes + count);
			ET_ABORT_UNLESS(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, ETAddressOf(count), modes));

			for (VkPresentModeKHR mode : allowable) {
				const auto candidate(Find(modes, end, mode));
				if (candidate != end) {
					outMode = *candidate;
					return VK_SUCCESS;
				}
			}

			return VK_ERROR_FEATURE_NOT_PRESENT;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint VkResult FindBestSurfaceFormat(VkSurfaceFormatKHR& outFormat, VkPhysicalDevice device, VkSurfaceKHR surface, VkColorSpaceKHR colorSpace, uint32 formatCount, const VkFormat* allowedFormats) ETNoexceptHint {
			uint32_t count(0u);

			ET_ABORT_UNLESS(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, ETAddressOf(count), nullptr));
			const auto formats(ETStackAlloc(VkSurfaceFormatKHR, count));
			const auto end(formats + count);
			ET_ABORT_UNLESS(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, ETAddressOf(count), formats));

			/*	Vulkan returns a 1-element array with format VK_FORMAT_UNDEFINED to indicate that the presentation layer does not impose any restriction on image format.
			 *	Thus, we default to the most-preferred format as specified by the caller. */
			if (count == 1u && formats[0].format == VK_FORMAT_UNDEFINED) {
				outFormat = VkSurfaceFormatKHR { allowedFormats[0], colorSpace };
				return VK_SUCCESS;
			}

			for (uint32 i(0u); i < formatCount; ++i) {
				const auto candidate(Find(formats, end, VkSurfaceFormatKHR { allowedFormats[i], colorSpace }));
				if (candidate == end) {
					continue;
				}

				outFormat = *candidate;
				return VK_SUCCESS;
			}

			return VK_ERROR_FEATURE_NOT_PRESENT;
		}

	} // anonymous namespace

	Display::Display() :
		_window(),
		_surface(nullptr),
		_format { VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR },
		_presentMode(VK_PRESENT_MODE_FIFO_KHR),
		_viewports {} {
	}

	// ---------------------------------------------------

	Display::~Display() {
		ET_ASSERT(_surface == nullptr, "Leaking Vulkan surface!");
	}

	// ---------------------------------------------------

	VkResult Display::BindSwapchain(Gpu& gpu, VkSwapchainKHR& outSwapchain, VkSwapchainKHR old) ETNoexceptHint {
		using ::Eldritch2::Swap;

		VkSurfaceCapabilitiesKHR capabilities;
		ET_ABORT_UNLESS(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, _surface, ETAddressOf(capabilities)));

		VkSwapchainKHR                 swapchain;
		const VkSwapchainCreateInfoKHR swapchainInfo {
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			/*pNext =*/nullptr, // No extension data.
			/*flags =*/0u,
			_surface,
			capabilities.minImageCount,
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
			/*clipped =*/VK_TRUE,
			old
		};
		ET_AT_SCOPE_EXIT(vkDestroySwapchainKHR(gpu, old, gpu.GetAllocationCallbacks()));
		ET_ABORT_UNLESS(vkCreateSwapchainKHR(gpu, ETAddressOf(swapchainInfo), gpu.GetAllocationCallbacks(), ETAddressOf(swapchain)));

		outSwapchain = swapchain;
		_window.EnsureVisible();

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Display::FreeSwapchain(Gpu& gpu, VkSwapchainKHR swapchain) ETNoexceptHint {
		vkDestroySwapchainKHR(gpu, swapchain, gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	VkResult Display::BindResources(Gpu& gpu, bool preferVerticalSync, uint32 formatCount, const VkFormat* allowedFormats) {
		using ::Eldritch2::Swap;

		Window window;
		ET_ABORT_UNLESS(Succeeded(window.BindResources()) ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED);
		ET_AT_SCOPE_EXIT(window.FreeResources());

		VkSurfaceKHR surface;
		ET_ABORT_UNLESS(CreateSurface(gpu, window, gpu.GetAllocationCallbacks(), ETAddressOf(surface)));
		ET_AT_SCOPE_EXIT(vkDestroySurfaceKHR(gpu, surface, gpu.GetAllocationCallbacks()));
		ET_ABORT_UNLESS(GetSurfacePresentationSupport(gpu, surface));

		VkSurfaceFormatKHR format;
		ET_ABORT_UNLESS(FindBestSurfaceFormat(format, gpu, surface, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, formatCount, allowedFormats));

		VkPresentModeKHR presentMode;
		if (preferVerticalSync) {
			ET_ABORT_UNLESS(FindBestPresentMode(presentMode, gpu, surface, { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR, VK_PRESENT_MODE_FIFO_KHR }));
		} else {
			ET_ABORT_UNLESS(FindBestPresentMode(presentMode, gpu, surface, { VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR }));
		}

		Swap(_window, window);
		Swap(_surface, surface);
		Swap(_format, format);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Display::FreeResources(Gpu& gpu) {
		for (Viewport& viewport : _viewports) {
			viewport.FreeResources(gpu);
		}

		_presentMode = VK_PRESENT_MODE_FIFO_KHR;
		_format      = VkSurfaceFormatKHR { VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		vkDestroySurfaceKHR(gpu, eastl::exchange(_surface, nullptr), nullptr);
		_window.FreeResources();
	}

}}} // namespace Eldritch2::Graphics::Vulkan
