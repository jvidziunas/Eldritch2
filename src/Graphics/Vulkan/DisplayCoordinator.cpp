/*==================================================================*\
  DisplayCoordinator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.hpp>
#include <Graphics/Vulkan/DisplayCoordinator.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	DisplayCoordinator::DisplayCoordinator() :
		_displayByName(MallocAllocator("Display Coordinator Display by Name Map Allocator")) {
	}

	// ---------------------------------------------------

	VkResult DisplayCoordinator::AcquireImages(Vulkan& vulkan, Gpu& gpu) {
		ET_FAIL_UNLESS(SubmitImages(gpu));

		ReadLock _(_displayMutex);
		for (DisplayMap<>::Iterator output(_displayByName.Begin()); output != _displayByName.End();) {
			Display& display(output->second);

			if (display.ShouldDestroy()) {
				display.FreeResources(vulkan, gpu);
				output = _displayByName.Erase(output);

				continue;
			}

			ET_FAIL_UNLESS(display.AcquireImage(vulkan, gpu));
			++output;
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult DisplayCoordinator::BindResources(Gpu& gpu) {
		GraphicsPipelineBuilder builder;

		builder.Begin(VK_PIPELINE_BIND_POINT_GRAPHICS, "CompositeFramebuffer");
		builder.DefineAttachment(VK_FORMAT_R8G8B8A8_SRGB, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		builder.AttachColorOutput(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		builder.Finish();

		GraphicsPipeline compositePipeline;
		ET_FAIL_UNLESS(compositePipeline.BindResources(gpu, builder));
		ET_AT_SCOPE_EXIT(compositePipeline.FreeResources(gpu));

		Swap(_compositePipeline, compositePipeline);
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void DisplayCoordinator::FreeResources(Gpu& gpu) {
		_compositePipeline.FreeResources(gpu);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
