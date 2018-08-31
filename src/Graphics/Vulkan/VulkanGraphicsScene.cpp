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
#include <Graphics/Vulkan/GraphicsPipelineBuilder.hpp>
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		ETInlineHint ETForceInlineHint GraphicsPipelineBuilder& BuildShadowPipeline(GraphicsPipelineBuilder& pipeline, VkFormat depthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT) {
			pipeline.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPassScale { /*width =*/1.0f, /*height =*/1.0f }, VK_SAMPLE_COUNT_1_BIT, "DepthOnly");
			pipeline.SetDepthStencilBuffer(/*attachment =*/pipeline.DefineAttachment(depthFormat));

			pipeline.Finish(/*andOptimize =*/false);

			return pipeline;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint GraphicsPipelineBuilder& BuildOpaqueLitPipeline(GraphicsPipelineBuilder& pipeline, VkFormat depthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT) {
			const uint32 depthBufferId(pipeline.DefineAttachment(depthFormat));

			pipeline.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPassScale { /*width =*/1.0f, /*height =*/1.0f }, VK_SAMPLE_COUNT_1_BIT, "DepthOnly");
			pipeline.SetDepthStencilBuffer(depthBufferId, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

			pipeline.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPassScale { /*width =*/1.0f, /*height =*/1.0f }, VK_SAMPLE_COUNT_1_BIT, "OpaqueLit");
			pipeline.AppendColorOutput(/*attachment =*/pipeline.DefineAttachment(VK_FORMAT_R8G8B8A8_SRGB), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			pipeline.SetDepthStencilBuffer(depthBufferId, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);

			pipeline.Finish(/*andOptimize =*/false);

			return pipeline;
		}

	} // anonymous namespace

	VulkanGraphicsScene::VulkanGraphicsScene() :
		GraphicsScene(/*geometryCellExtent =*/Vector(10.0f, 10.0f, 10.0f, 1.0f), /*lightCellExtent =*/Vector(10.0f, 10.0f, 10.0f, 1.0f)),
		_rootViews(MallocAllocator("Vulkan Graphics Scene Player View List Allocator")),
		_transforms(),
		_opaqueBatches() {
	}

	// ---------------------------------------------------

	VkResult VulkanGraphicsScene::BindResources(Gpu& gpu, VkExtent2D shadowResolution, VkDeviceSize transformArenaSize) {
		using Eldritch2::Swap;

		UniformBuffer transforms;
		ET_ABORT_UNLESS(transforms.BindResources(gpu, transformArenaSize));
		ET_AT_SCOPE_EXIT(transforms.FreeResources(gpu));

		BatchCoordinator batchCoordinator;
		ET_ABORT_UNLESS(batchCoordinator.BindResources(gpu, /*parameterBufferSize =*/16384u, /*instanceBufferSize =*/16u * 1024u * 1024u));
		ET_AT_SCOPE_EXIT(batchCoordinator.FreeResources(gpu));

		GraphicsPipelineBuilder shadowPipelineBuilder;
		GraphicsPipeline        shadowPipeline;
		ET_ABORT_UNLESS(shadowPipeline.BindResources(gpu, BuildShadowPipeline(shadowPipelineBuilder)));
		ET_AT_SCOPE_EXIT(shadowPipeline.FreeResources(gpu));

		Framebuffer shadowAtlas;
		ET_ABORT_UNLESS(shadowAtlas.BindResources(gpu, shadowPipeline, shadowResolution, /*layers =*/16u));
		ET_AT_SCOPE_EXIT(shadowAtlas.FreeResources(gpu));

		GraphicsPipelineBuilder opaqueLitPipelineBuilder;
		GraphicsPipeline        opaqueLitPipeline;
		ET_ABORT_UNLESS(opaqueLitPipeline.BindResources(gpu, BuildOpaqueLitPipeline(opaqueLitPipelineBuilder)));
		ET_AT_SCOPE_EXIT(opaqueLitPipeline.FreeResources(gpu));

		DescriptorTable shaderResources;
		ET_ABORT_UNLESS(shaderResources.BindResources(gpu, opaqueLitPipeline.GetDescriptorLayout(), /*poolCount =*/0u, /*pools =*/nullptr));
		ET_AT_SCOPE_EXIT(shaderResources.FreeResources(gpu));

		Swap(_transforms, transforms);
		Swap(_opaqueBatches, batchCoordinator);
		Swap(_shadowPipeline, shadowPipeline);
		Swap(_shadowAtlas, shadowAtlas);
		Swap(_opaqueLitPipeline, opaqueLitPipeline);
		Swap(_shaderResources, shaderResources);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void VulkanGraphicsScene::FreeResources(Gpu& gpu) {
		_shaderResources.FreeResources(gpu);
		_opaqueLitPipeline.FreeResources(gpu);
		_shadowAtlas.FreeResources(gpu);
		_shadowPipeline.FreeResources(gpu);
		_opaqueBatches.FreeResources(gpu);
		_transforms.FreeResources(gpu);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
