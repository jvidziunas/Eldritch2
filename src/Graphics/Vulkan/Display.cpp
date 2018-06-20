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

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	namespace {

		constexpr VkPresentModeKHR immediateModes[] = { VK_PRESENT_MODE_IMMEDIATE_KHR };
		constexpr VkPresentModeKHR vsyncModes[]     = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR, VK_PRESENT_MODE_FIFO_KHR };

		ETInlineHint VkResult WergImage(Gpu& gpu, VkImage image, VkFormat format, VkExtent2D extent, VkImageView& outView, VkFramebuffer& outFramebuffer) {
			const VkImageViewCreateInfo viewInfo{
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				/*pNext =*/nullptr, // No extension structures.
				/*flags =*/0u,      // No create flags.
				image,
				VK_IMAGE_VIEW_TYPE_2D,
				format,
				MakeIdentitySwizzle(),
				VkImageSubresourceRange{
					VK_IMAGE_ASPECT_COLOR_BIT,
					0u,
					1u,
					0u,
					1u }
			};
			ET_FAIL_UNLESS(vkCreateImageView(gpu, &viewInfo, gpu.GetAllocationCallbacks(), &outView));
			const VkFramebufferCreateInfo framebufferInfo{
				VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				nullptr, // No extension structures.
				0u,      // No create flags,
				nullptr,
				1u,
				&outView,
				extent.width,
				extent.height,
				1u // Single layer
			};
			return vkCreateFramebuffer(gpu, &framebufferInfo, gpu.GetAllocationCallbacks(), &outFramebuffer);
		}

		// ---------------------------------------------------

		ETInlineHint void RecordBlitCommands(const Gpu& gpu, VkCommandBuffer blitCommands, VkImage image, VkFramebuffer framebuffer) {
			using ::Eldritch2::Swap;

			VkImageMemoryBarrier barrier{
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				/*pNext =*/nullptr,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				gpu.GetQueueFamilyByConcept(QueueConcept::Presentation),
				gpu.GetQueueFamilyByConcept(QueueConcept::Drawing),
				image,
				VkImageSubresourceRange{
					VK_IMAGE_ASPECT_COLOR_BIT,
					0u,
					1u,
					0u,
					1u }
			};

			vkCmdPipelineBarrier(blitCommands, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0u, 0u, nullptr, 0u, nullptr, 1u, &barrier);

			Swap(barrier.srcAccessMask, barrier.dstAccessMask);
			Swap(barrier.srcQueueFamilyIndex, barrier.dstQueueFamilyIndex);

			vkCmdPipelineBarrier(blitCommands, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0u, 0u, nullptr, 0u, nullptr, 1u, &barrier);
		}

		// ---------------------------------------------------

		ETInlineHint VkResult CheckSurfacePresentationSupport(Gpu& gpu, VkSurfaceKHR surface) {
			VkBool32 canPresentFromQueue;
			ET_FAIL_UNLESS(vkGetPhysicalDeviceSurfaceSupportKHR(gpu, gpu.GetQueueFamilyByConcept(QueueConcept::Presentation), surface, &canPresentFromQueue));

			return canPresentFromQueue == VK_TRUE ? VK_SUCCESS : VK_ERROR_FEATURE_NOT_PRESENT;
		}

		// ---------------------------------------------------

		template <size_t count>
		ETInlineHint VkResult FindBestPresentMode(VkPhysicalDevice device, VkSurfaceKHR surface, VkPresentModeKHR& selectedMode, const VkPresentModeKHR (&requestedModes)[count]) {
			uint32_t modeCount;

			ET_FAIL_UNLESS(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, nullptr));
			VkPresentModeKHR* const modes(static_cast<VkPresentModeKHR*>(_alloca(modeCount * sizeof(VkPresentModeKHR))));
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
			VkSurfaceFormatKHR* const formats(static_cast<VkSurfaceFormatKHR*>(_alloca(formatCount * sizeof(VkSurfaceFormatKHR))));
			VkSurfaceFormatKHR* const end(formats + formatCount);
			ET_FAIL_UNLESS(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats));

			/*	Vulkan returns a 1-element array with format VK_FORMAT_UNDEFINED to indicate that the presentation layer does not impose any restriction on image format.
				Thus, we default to the most-preferred format as specified by the caller. */
			if (formatCount == 1u && formats[0].format == VK_FORMAT_UNDEFINED) {
				selectedFormat = VkSurfaceFormatKHR{ requestedFormats[0], colorSpace };
				return VK_SUCCESS;
			}

			for (VkFormat format : requestedFormats) {
				const VkSurfaceFormatKHR        surfaceFormat{ format, colorSpace };
				const VkSurfaceFormatKHR* const candidate(Find(formats, end, surfaceFormat));
				if (candidate != end) {
					selectedFormat = *candidate;
					return VK_SUCCESS;
				}
			}

			return VK_ERROR_FEATURE_NOT_PRESENT;
		}

		// ---------------------------------------------------

		ETInlineHint VkSwapchainCreateInfoKHR MakeCreateInfo(
			const VkSurfaceCapabilitiesKHR& capabilities,
			const VkSurfaceFormatKHR&       format,
			VkPresentModeKHR                presentMode,
			VkSurfaceKHR                    surface,
			VkSwapchainKHR                  old) {
			return VkSwapchainCreateInfoKHR{
				VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				nullptr, // No extension data.
				0u,      // No creation flags.
				surface,
				capabilities.minImageCount,
				format.format,
				format.colorSpace,
				capabilities.currentExtent,
				1u, // Single layer.
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0u,      // Exclusive image sharing.
				nullptr, // Exclusive image sharing.
				capabilities.currentTransform,
				VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				presentMode,
				VK_TRUE, //	We don't read swapchain images, so allow discard optimizations.
				old
			};
		}

	} // anonymous namespace

	VkResult DisplaySource::BindResources(Gpu& gpu, VkExtent2D extent, uint32_t arrayLayers) {
		Framebuffer framebuffer;

		ET_FAIL_UNLESS(framebuffer.BindResources(gpu, VK_NULL_HANDLE, GetFramebufferExtent(extent)));
		ET_AT_SCOPE_EXIT(framebuffer.FreeResources(gpu));

		Swap(_framebuffer, framebuffer);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void DisplaySource::FreeResources(Gpu& gpu) {
		_framebuffer.FreeResources(gpu);
	}

	// ---------------------------------------------------

	Display::Display() :
		_window(),
		_surface(nullptr),
		_format{ VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR },
		_presentMode(VK_PRESENT_MODE_FIFO_KHR),
		_swapchain(nullptr),
		_currentImage(0u),
		_blitPool(nullptr),
		_blitCommands{ nullptr },
		_views{ nullptr },
		_framebuffers{ nullptr },
		_imageReady{ nullptr },
		_sources{ nullptr } {
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
		ET_ASSERT(_blitPool == nullptr, "Leaking Vulkan command pool!");
	}

	// ---------------------------------------------------

	VkResult Display::AcquireImage(Vulkan& vulkan, Gpu& gpu) {
		if (ET_UNLIKELY(!_window.Exists())) {
			ET_FAIL_UNLESS(BindResources(vulkan, gpu));

			_window.SetDisplayMode({ FullscreenMode::WindowedFullscreen, 0u, 0u });
			_window.EnsureVisible();
		}

		VkResult result(VK_ERROR_OUT_OF_DATE_KHR);
		if (ET_LIKELY(_swapchain)) {
			result = vkAcquireNextImageKHR(gpu, _swapchain, ULONG_MAX, _imageReady, nullptr, &_currentImage);
		}

		while (ET_UNLIKELY(result == VK_ERROR_OUT_OF_DATE_KHR)) {
			ET_FAIL_UNLESS(ResizeSwapchain(gpu));
			result = vkAcquireNextImageKHR(gpu, _swapchain, ULONG_MAX, _imageReady, nullptr, &_currentImage);
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult Display::SubmitImage(Gpu& gpu) {
		ET_FAIL_UNLESS(gpu.SubmitAsync(QueueConcept::Presentation, VK_NULL_HANDLE, { VkSubmitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO,
																								   /*pNext =*/nullptr, // No extensions.
																								   /*waitSemaphoreCount =*/0u,
																								   /*pWaitSemaphores =*/nullptr,
																								   /*pWaitDstStageMask =*/nullptr,
																								   /*commandBufferCount =*/0u,
																								   /*pCommandBuffers =*/nullptr,
																								   /*signalSemaphoreCount =*/1u,
																								   /*pSignalSemaphores =*/nullptr } }));
		VkResult result(gpu.PresentAsync(Presentation, VkPresentInfoKHR{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
																		 /*pNext =*/nullptr,
																		 /*waitSemaphoreCount =*/1u,
																		 /*pWaitSemaphores =*/nullptr,
																		 /*swapchainCount =*/1u, &_swapchain, &_currentImage,
																		 /*pResults =*/nullptr }));
		return result == VK_ERROR_OUT_OF_DATE_KHR ? ResizeSwapchain(gpu) : result;
	}

	// ---------------------------------------------------

	VkResult Display::ResizeSwapchain(Gpu& gpu) {
		using ::Eldritch2::Swap;

		VkSurfaceCapabilitiesKHR capabilities;
		ET_FAIL_UNLESS(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, _surface, &capabilities));

		VkSwapchainKHR                 swapchain;
		const VkSwapchainCreateInfoKHR swapchainInfo(MakeCreateInfo(capabilities, _format, _presentMode, _surface, _swapchain));
		ET_AT_SCOPE_EXIT(vkDestroySwapchainKHR(gpu, swapchain, gpu.GetAllocationCallbacks()));
		ET_FAIL_UNLESS(vkCreateSwapchainKHR(gpu, &swapchainInfo, gpu.GetAllocationCallbacks(), &swapchain));

		VkImage       images[_countof(_views)]              = { nullptr };
		VkImageView   views[_countof(_views)]               = { nullptr };
		VkFramebuffer framebuffers[_countof(_framebuffers)] = { nullptr };
		uint32_t      count;
		ET_FAIL_UNLESS(vkGetSwapchainImagesKHR(gpu, swapchain, &count, nullptr));
		if (ET_UNLIKELY(_countof(images) < count)) {
			return VK_ERROR_FEATURE_NOT_PRESENT;
		}
		ET_FAIL_UNLESS(vkGetSwapchainImagesKHR(gpu, swapchain, &count, images));

		ET_AT_SCOPE_EXIT(for (VkImageView view
							  : views) vkDestroyImageView(gpu, view, gpu.GetAllocationCallbacks()));
		ET_AT_SCOPE_EXIT(for (VkFramebuffer framebuffer
							  : framebuffers) vkDestroyFramebuffer(gpu, framebuffer, gpu.GetAllocationCallbacks()));
		for (uint32_t image(0u); image < count; ++image) {
			if (images[image] == nullptr) {
				break;
			}

			ET_FAIL_UNLESS(WergImage(gpu, images[image], _format.format, capabilities.currentExtent, views[image], framebuffers[image]));
		}

		Swap(_swapchain, swapchain);
		Swap(_images, images);
		Swap(_views, views);
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
		ET_FAIL_UNLESS(FindBestSurfaceFormat(gpu, surface, format, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_R8G8B8A8_SRGB, VK_FORMAT_A8B8G8R8_SRGB_PACK32 }));

		VkPresentModeKHR presentMode;
		ET_FAIL_UNLESS(FindBestPresentMode(gpu, surface, presentMode, vsyncModes));

		VkSurfaceCapabilitiesKHR capabilities;
		ET_FAIL_UNLESS(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &capabilities));
		if (MaxQueueDepth < capabilities.minImageCount) {
			return VK_ERROR_FEATURE_NOT_PRESENT;
		}

		VkCommandPool                 blitPool;
		const VkCommandPoolCreateInfo blitPoolInfo{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			/*pNext =*/nullptr, // No extensions.
			/*flags =*/0u,
			gpu.GetQueueFamilyByConcept(QueueConcept::Presentation)
		};
		ET_FAIL_UNLESS(vkCreateCommandPool(gpu, &blitPoolInfo, gpu.GetAllocationCallbacks(), &blitPool));
		ET_AT_SCOPE_EXIT(vkDestroyCommandPool(gpu, blitPool, gpu.GetAllocationCallbacks()));

		VkCommandBuffer                   blitCommands[MaxQueueDepth];
		const VkCommandBufferAllocateInfo blitCommandsInfo{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			/*pNext =*/nullptr, // No extensions.
			blitPool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			static_cast<uint32_t>(_countof(blitCommands))
		};
		ET_FAIL_UNLESS(vkAllocateCommandBuffers(gpu, &blitCommandsInfo, blitCommands));
		ET_AT_SCOPE_EXIT(if (blitPool) vkFreeCommandBuffers(gpu, blitPool, blitCommandsInfo.commandBufferCount, blitCommands));

		VkSemaphore                 imageReady[MaxQueueDepth];
		const VkSemaphoreCreateInfo imageReadyInfo{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u
		};
		ET_AT_SCOPE_EXIT(for (VkSemaphore semaphore
							  : imageReady) vkDestroySemaphore(gpu, semaphore, gpu.GetAllocationCallbacks()));
		for (VkSemaphore& semaphore : imageReady) {
			ET_FAIL_UNLESS(vkCreateSemaphore(gpu, &imageReadyInfo, gpu.GetAllocationCallbacks(), &semaphore));
		}

		Swap(_window, window);
		Swap(_surface, surface);
		Swap(_format, format);
		Swap(_blitPool, blitPool);
		Swap(_blitCommands, blitCommands);
		Swap(_imageReady, imageReady);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void Display::FreeResources(Vulkan& vulkan, Gpu& gpu) {
		for (VkSemaphore& semaphore : _imageReady) {
			vkDestroySemaphore(gpu, eastl::exchange(semaphore, nullptr), gpu.GetAllocationCallbacks());
		}
		for (VkFramebuffer& framebuffer : _framebuffers) {
			vkDestroyFramebuffer(gpu, eastl::exchange(framebuffer, nullptr), gpu.GetAllocationCallbacks());
		}
		for (VkImageView& view : _views) {
			vkDestroyImageView(gpu, eastl::exchange(view, nullptr), gpu.GetAllocationCallbacks());
		}
		if (VkCommandPool pool = eastl::exchange(_blitPool, nullptr)) {
			vkFreeCommandBuffers(gpu, pool, _countof(_blitCommands), _blitCommands);
			vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks());
		}
		vkDestroySwapchainKHR(gpu, eastl::exchange(_swapchain, nullptr), gpu.GetAllocationCallbacks());
		_presentMode = VK_PRESENT_MODE_FIFO_KHR;
		_format      = VkSurfaceFormatKHR{ VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		vkDestroySurfaceKHR(vulkan, eastl::exchange(_surface, nullptr), nullptr);
		_window.FreeResources();
	}

	// ---------------------------------------------------

	void Swap(Display& lhs, Display& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._window, rhs._window);
		Swap(lhs._surface, rhs._surface);
		Swap(lhs._format, rhs._format);
		Swap(lhs._presentMode, rhs._presentMode);
		Swap(lhs._swapchain, rhs._swapchain);
		Swap(lhs._currentImage, rhs._currentImage);
		Swap(lhs._imageReady, rhs._imageReady);
		Swap(lhs._blitPool, rhs._blitPool);
		Swap(lhs._blitCommands, rhs._blitCommands);
		Swap(lhs._views, rhs._views);
		Swap(lhs._framebuffers, rhs._framebuffers);
		Swap(lhs._sources, rhs._sources);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
