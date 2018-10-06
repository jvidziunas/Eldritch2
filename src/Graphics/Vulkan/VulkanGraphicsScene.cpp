/*==================================================================*\
  VulkanGraphicsScene.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	VulkanGraphicsScene::VulkanGraphicsScene() :
		GraphicsScene(/*geometryCellExtent =*/ { 10.0f, 10.0f, 10.0f, 1.0f }, /*lightCellExtent =*/ { 10.0f, 10.0f, 10.0f, 1.0f }),
		_litPipelineBuilder(nullptr),
		_shadowPipelineBuilder(nullptr),
		_rootViews(MallocAllocator("Vulkan Graphics Scene Player View List Allocator")) {}

	// ---------------------------------------------------

	VkResult VulkanGraphicsScene::BindResources(Gpu& gpu, const GraphicsPipelineBuilder& litPipelineBuilder, const GraphicsPipelineBuilder& shadowPipelineBuilder, VkExtent2D shadowResolution, VkDeviceSize transformArenaSize) {
		using Eldritch2::Swap;

		UniformBuffer uniforms;
		ET_ABORT_UNLESS(uniforms.BindResources(gpu, transformArenaSize));
		ET_AT_SCOPE_EXIT(uniforms.FreeResources(gpu));

		GraphicsPipeline shadowPipeline;
		ET_ABORT_UNLESS(shadowPipeline.BindResources(gpu, shadowPipelineBuilder));
		ET_AT_SCOPE_EXIT(shadowPipeline.FreeResources(gpu));

		Framebuffer shadowAtlas;
		ET_ABORT_UNLESS(shadowAtlas.BindResources(gpu, shadowPipelineBuilder, shadowResolution, /*layers =*/16u));
		ET_AT_SCOPE_EXIT(shadowAtlas.FreeResources(gpu));

		GraphicsPipeline litPipeline;
		ET_ABORT_UNLESS(litPipeline.BindResources(gpu, litPipelineBuilder));
		ET_AT_SCOPE_EXIT(litPipeline.FreeResources(gpu));

		Swap(_uniforms, uniforms);
		Swap(_shadowPipeline, shadowPipeline);
		Swap(_shadowAtlas, shadowAtlas);
		Swap(_litPipeline, litPipeline);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void VulkanGraphicsScene::FreeResources(Gpu& gpu) {
		_litPipeline.FreeResources(gpu);
		_shadowAtlas.FreeResources(gpu);
		_shadowPipeline.FreeResources(gpu);
		_uniforms.FreeResources(gpu);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
