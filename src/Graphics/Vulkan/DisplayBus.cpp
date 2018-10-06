/*==================================================================*\
  DisplayBus.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/DisplayBus.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		ETInlineHint ETForceInlineHint GraphicsPipelineBuilder& BuildCompositorPipeline(GraphicsPipelineBuilder& pipeline, VkFormat backbufferFormat) {
			pipeline.BeginPass(PassType::Compute, { /*width =*/1.0f, /*height =*/1.0f }, /*samplesPerPixel =*/1u, "FramebufferComposite");
			pipeline.AppendColorOutput(/*attachment =*/pipeline.DefineAttachment(backbufferFormat, /*mips =*/1u, /*isPersistent =*/false));

			return pipeline.Optimize();
		}

	} // anonymous namespace

	DisplayBus::DisplayBus() :
		_compositorByFormat(MallocAllocator("Display Compositor By Format Allocator")),
		_displays(MallocAllocator("Display List Allocator")) {
	}

	// ---------------------------------------------------

	VkResult DisplayBus::AcquireSwapchainImages(Gpu& gpu) {
		Lock _(_displayMutex);

		for (DisplayList::Reference current : _displays) {
			if (eastl::get<VkResult&>(current) != VK_SUCCESS) {
				ET_ABORT_UNLESS(eastl::get<Display&>(current).BindSwapchain(gpu, eastl::get<VkSwapchainKHR&>(current), /*old =*/eastl::get<VkSwapchainKHR&>(current)));

				for (Viewport& viewport : eastl::get<Display&>(current).GetViewports()) {
					ET_ABORT_UNLESS(viewport.Resize(gpu, VkExtent2D {}, /*layers =*/1u));
				}
			}

			const VkAcquireNextImageInfoKHR acquireInfo {
				VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR,
				/*pNext =*/nullptr,
				eastl::get<VkSwapchainKHR&>(current),
				~uint64(0),
				/*semaphore =*/VK_NULL_HANDLE,
				/*fence =*/VK_NULL_HANDLE,
				gpu.GetFrameAfrDeviceMask()
			};

			ET_ABORT_UNLESS(vkAcquireNextImage2KHR(gpu, ETAddressOf(acquireInfo), ETAddressOf(eastl::get<uint32&>(current))));
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult DisplayBus::PresentSwapchainImages(Gpu& gpu) {
		ReadLock _(_displayMutex);
		return gpu.PresentAsync({ // clang-format off
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			/*pNext=*/nullptr,
			/*waitSemaphoreCount =*/uint32(_displays.GetSize()),
			/*pWaitSemaphores =*/_displays.GetData<VkSemaphore>(),
			/*swapchainCount =*/uint32(_displays.GetSize()),
			/*pSwapchains =*/_displays.GetData<VkSwapchainKHR>(),
			/*pImageIndices =*/_displays.GetData<uint32>(),
			/*pResults =*/_displays.GetData<VkResult>() }); // clang-format on
	}

	// ---------------------------------------------------

	VkResult DisplayBus::BindResources(Gpu& gpu) {
		static ETConstexpr VkFormat formats[] = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_R8G8B8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM };

		HashMap<VkFormat, GraphicsPipeline> compositorByFormat(_compositorByFormat.GetAllocator());
		ET_AT_SCOPE_EXIT(for (Pair<const VkFormat, GraphicsPipeline>& compositor
							  : compositorByFormat) compositor.second.FreeResources(gpu));
		for (VkFormat format : formats) {
			GraphicsPipelineBuilder compositorBuilder;
			ET_ABORT_UNLESS(compositorByFormat[format].BindResources(gpu, BuildCompositorPipeline(compositorBuilder, format), /*commandPoolCount =*/2u));
		}

		Swap(_compositorByFormat, compositorByFormat);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void DisplayBus::FreeResources(Gpu& gpu) {
		for (DisplayList::Reference display : _displays) {
			for (ImageList::Reference image : eastl::get<ImageList&>(display)) {
				eastl::get<CommandList&>(image).FreeResources(gpu);
				eastl::get<Framebuffer&>(image).FreeResources(gpu);
				vkDestroyImageView(gpu, eastl::get<VkImageView&>(image), gpu.GetAllocationCallbacks());
			}

			vkDestroySemaphore(gpu, eastl::get<VkSemaphore&>(display), gpu.GetAllocationCallbacks());
			eastl::get<Display&>(display).FreeSwapchain(gpu, eastl::get<VkSwapchainKHR&>(display));
			eastl::get<Display&>(display).FreeResources(gpu);
		}

		for (auto& compositor : _compositorByFormat) {
			compositor.second.FreeResources(gpu);
		}

		_displays.Clear();
		_compositorByFormat.Clear();
	}

}}} // namespace Eldritch2::Graphics::Vulkan
