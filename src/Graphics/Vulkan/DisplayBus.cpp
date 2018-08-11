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

	DisplayBus::DisplayBus() :
		_displayByName(MallocAllocator("Display Bus Display by Name Map Allocator")) {
	}

	// ---------------------------------------------------

	VkResult DisplayBus::AcquireImages(Vulkan& vulkan, Gpu& gpu) {
		Lock _(_displayMutex);
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

	VkResult DisplayBus::BindResources(Gpu& gpu) {
		GraphicsPipelineBuilder builder;
		builder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, 1.0f, 1.0f, "FramebufferComposite");
		builder.AppendColorOutput(/*attachment =*/builder.DefineAttachment(VK_FORMAT_R8G8B8A8_SRGB, VK_SAMPLE_COUNT_1_BIT));
		builder.Finish(/*andOptimize =*/false); // Skip optimization, it's assumed we know what we're doing/this is a 'trusted' source

		GraphicsPipeline compositor;
		ET_FAIL_UNLESS(compositor.BindResources(gpu, builder));
		ET_AT_SCOPE_EXIT(compositor.FreeResources(gpu));

		Swap(_compositor, compositor);
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void DisplayBus::FreeResources(Gpu& gpu) {
		_compositor.FreeResources(gpu);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
