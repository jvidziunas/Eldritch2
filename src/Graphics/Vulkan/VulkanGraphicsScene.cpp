/*==================================================================*\
  VulkanGraphicsScene.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	VulkanGraphicsScene::VulkanGraphicsScene() ETNoexceptHint : GraphicsScene(/*geometryCellExtent =*/Vector(10.0f, 10.0f, 10.0f, 1.0f), /*lightCellExtent =*/Vector(10.0f, 10.0f, 10.0f, 1.0f)) {}

	// ---------------------------------------------------

	VkResult VulkanGraphicsScene::BindResources(Gpu& gpu, const PipelineBuilder& litPipelineBuilder_, const PipelineBuilder& shadowMapPipelineBuilder_, VkExtent2D shadowResolution) {
		using Eldritch2::Swap;

		PipelineBuilder  shadowMapPipelineBuilder(shadowMapPipelineBuilder_);
		GraphicsPipeline shadowMapPipeline;
		ET_ABORT_UNLESS(shadowMapPipeline.BindResources(gpu, shadowMapPipelineBuilder, /*commandPoolCount =*/17u));
		ET_AT_SCOPE_EXIT(shadowMapPipeline.FreeResources(gpu));

		Framebuffer shadowMapAtlas;
		ET_ABORT_UNLESS(shadowMapAtlas.BindResources(gpu, shadowMapPipelineBuilder, shadowResolution, /*layers =*/16u));
		ET_AT_SCOPE_EXIT(shadowMapAtlas.FreeResources(gpu));

		PipelineBuilder  litPipelineBuilder(litPipelineBuilder_);
		GraphicsPipeline litPipeline;
		ET_ABORT_UNLESS(litPipeline.BindResources(gpu, litPipelineBuilder, /*commandPoolCount =*/17u));
		ET_AT_SCOPE_EXIT(litPipeline.FreeResources(gpu));

		Swap(_shadowMapPipelineBuilder, shadowMapPipelineBuilder);
		Swap(_shadowMapPipeline, shadowMapPipeline);
		Swap(_shadowMapAtlas, shadowMapAtlas);
		Swap(_litPipelineBuilder, litPipelineBuilder);
		Swap(_litPipeline, litPipeline);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void VulkanGraphicsScene::FreeResources(Gpu& gpu) ETNoexceptHint {
		_litPipeline.FreeResources(gpu);
		_shadowMapAtlas.FreeResources(gpu);
		_shadowMapPipeline.FreeResources(gpu);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
