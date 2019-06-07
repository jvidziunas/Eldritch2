/*==================================================================*\
  DisplayBus.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/DisplayBus.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

ETConstexpr ETForceInlineHint size_t GetHashCode(VkFormat format, size_t seed = 0u) ETNoexceptHint {
	return size_t(format) + seed;
}

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	DisplayBus::DisplayBus() ETNoexceptHint : _compositorByFormat(MallocAllocator("Display Compositor By Format Allocator")),
											  _displays(DisplayList::AllocatorType("Display List Allocator")),
											  _presentableImages(PresentList::AllocatorType("Presentable Image List Allocator")) {}

	// ---------------------------------------------------

	VkResult DisplayBus::FlipSwapchainImages(Gpu& gpu) {
		ET_LOCK_SCOPE(_presentableImagesMutex);

		ET_ABORT_UNLESS(gpu.GetQueue(Presentation).Present({ // clang-format off
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			/*pNext=*/nullptr,
			/*waitSemaphoreCount =*/uint32(_presentableImages.GetSize()),
			/*pWaitSemaphores =*/_presentableImages.GetData<VkSemaphore>(),
			/*swapchainCount =*/uint32(_presentableImages.GetSize()),
			/*pSwapchains =*/_presentableImages.GetData<VkSwapchainKHR>(),
			/*pImageIndices =*/_presentableImages.GetData<uint32>(),
			/*pResults =*/_presentableImages.GetData<VkResult>() })); // clang-format on

		// Invoke resize delegates for any swap chains that changed dimensions/formats/attributes.
		_presentableImages.ClearAndDispose([&](VkResult result, VkSemaphore /*ready*/, VkSwapchainKHR /*swapchain*/, uint32 /*imageIndex*/, ResizeDelegate& onResize) {
			if (result == VK_SUBOPTIMAL_KHR || Failed(result)) {
				ET_ABORT_UNLESS(onResize(gpu));
			}

			return VK_SUCCESS;
		});

		// ET_PROFILE_FRAME_BEGIN_GPU(nullptr);
		return VK_SUCCESS;
		// End of lock scope.
	}

	// ---------------------------------------------------

	VkResult DisplayBus::BindResources(Gpu& gpu) {
		static ETConstexpr auto BuildCompositorPipeline([](PipelineBuilder& builder, GpuFormat format) -> PipelineBuilder& {
			enum : PipelineBuilder::AttachmentIndex { Backbuffer };

			builder.DefineSharedAttachment(format, /*smallestMip =*/0u, /*isPersistent =*/false);

			static ETConstexpr StageAttachment attachments[] = { StageAttachment(Backbuffer, /*mip =*/0u), };
			builder.DefineComputeStage("FramebufferComposite", StageResolution(/*width =*/1.0f, /*height =*/1.0f), { Begin(attachments), End(attachments) } );

			return builder.Optimize();
		});

		// ---

		HashMap<VkFormat, GraphicsPipeline> compositorByFormat(_compositorByFormat.GetAllocator());
		ET_AT_SCOPE_EXIT(compositorByFormat.ClearAndDispose([&](GraphicsPipeline& pipeline) ETNoexceptHint {
			pipeline.FreeResources(gpu);
		}));

		for (GpuFormat format : { GpuFormat::R8G8B8A8_Srgb, GpuFormat::R8G8B8A8_Unorm }) {
			PipelineBuilder builder;
			ET_ABORT_UNLESS(compositorByFormat[AsVkFormat(format)].BindResources(gpu, BuildCompositorPipeline(builder, format), /*commandPoolCount =*/2u));
		}

		Swap(_compositorByFormat, compositorByFormat);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void DisplayBus::FreeResources(Gpu& gpu) ETNoexceptHint {
		_presentableImages.Clear();
		_compositorByFormat.ClearAndDispose([&gpu](GraphicsPipeline& pipeline) ETNoexceptHint {
			pipeline.FreeResources(gpu);
		});
	}

}}} // namespace Eldritch2::Graphics::Vulkan
