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
			pipeline.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPassScale { /*width =*/1.0f, /*height =*/1.0f }, VK_SAMPLE_COUNT_1_BIT, "FramebufferComposite");
			pipeline.AppendColorOutput(/*attachment =*/pipeline.DefineAttachment(backbufferFormat));
			pipeline.Finish(/*andOptimize =*/false); // Skip optimization, it's assumed we know what we're doing/this is a 'trusted' source

			return pipeline;
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
			if (eastl::get<VkResult&>(current) == VK_SUBOPTIMAL_KHR) {
				ET_ABORT_UNLESS(eastl::get<Display&>(current).CreateSwapchain(gpu, eastl::get<VkSwapchainKHR&>(current)));
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
		return gpu.PresentAsync(QueueConcept::Presentation, { // clang-format off
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			/*pNext=*/nullptr,
			/*waitSemaphoreCount =*/uint32(_displays.GetSize()),
			/*pWaitSemaphores =*/_displays.Get<VkSemaphore>(),
			/*swapchainCount =*/uint32(_displays.GetSize()),
			/*pSwapchains =*/_displays.Get<VkSwapchainKHR>(),
			/*pImageIndices =*/_displays.Get<uint32>(),
			/*pResults =*/_displays.Get<VkResult>() }); // clang-format on
	}

	// ---------------------------------------------------

	VkResult DisplayBus::BindResources(Gpu& gpu) {
		static ETConstexpr VkFormat formats[] = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_R8G8B8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM };

		ArrayMap<VkFormat, GraphicsPipeline> compositorByFormat(_compositorByFormat.GetAllocator());
		ET_AT_SCOPE_EXIT(for (auto& compositor
							  : compositorByFormat) compositor.second.FreeResources(gpu));
		for (VkFormat format : formats) {
			GraphicsPipelineBuilder compositorBuilder;
			ET_ABORT_UNLESS(compositorByFormat[format].BindResources(gpu, BuildCompositorPipeline(compositorBuilder, format)));
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
			vkDestroySwapchainKHR(gpu, eastl::get<VkSwapchainKHR&>(display), gpu.GetAllocationCallbacks());
			eastl::get<Display&>(display).FreeResources(gpu);
		}

		for (auto& compositor : _compositorByFormat) {
			compositor.second.FreeResources(gpu);
		}

		_displays.Clear();
		_compositorByFormat.Clear();
	}

}}} // namespace Eldritch2::Graphics::Vulkan
