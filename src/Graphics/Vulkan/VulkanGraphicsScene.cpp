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
#include <Graphics/Vulkan/GpuResourceBus.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;

	namespace {

		static const Vector GeometryCellDimensions(10.0f, 10.0f, 10.0f, 1.0f);
		static const Vector LightCellDimensions(10.0f, 10.0f, 10.0f, 1.0f);
		enum : uint32 { InitialFrameId = 0u };

		// ---------------------------------------------------

		ETInlineHint ArrayList<View> BuildViewList(const VulkanGraphicsScene& scene, VkRect2D /*renderArea*/) {
			ArrayList<View> views(MallocAllocator("Frame View Stack Allocator"));

			//  We cannot use iterators for traversal since we may append one or more child views to the end of the list.
			//	List resize operations invalidate iterators and we cannot guarantee the list will not be resized.
			for (size_t viewId(0u); viewId < views.GetSize(); ++viewId) {
				const View& view(views[viewId]);

				for (const PortalView* portal : scene.GetConcept(PortalViewType::PortalViews)) {
				}
			}

			return eastl::move(views);
		}

	} // anonymous namespace

	VulkanGraphicsScene::Frame::Frame() :
		_commandsConsumed(nullptr) {
	}

	// ---------------------------------------------------

	bool VulkanGraphicsScene::Frame::CheckCommandsConsumed(Gpu& gpu) const {
		return vkGetFenceStatus(gpu, _commandsConsumed) != VK_NOT_READY;
	}

	// ---------------------------------------------------

	VkResult VulkanGraphicsScene::Frame::SubmitCommands(Gpu& gpu, const VulkanGraphicsScene& scene) {
		constexpr VkPipelineStageFlags AllCommandsStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

		ET_FAIL_UNLESS(_commands.ResetPool(gpu));
		ET_FAIL_UNLESS(_commands.BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));

		const GraphicsPipeline& pipeline(scene._mainPipeline);
		for (const PlayerView* playerView : scene._playerViews) {
			const VkRect2D        renderArea{ VkOffset2D{ 0, 0 }, playerView->GetValidExtent() };
			const ArrayList<View> views(BuildViewList(scene, renderArea));

			for (uint32 pass(0u); pass < pipeline.GetPassCount(); ++pass) {
				const GraphicsPipeline::Pass& source(pipeline[pass]);
				_commands.BeginRendering(VK_SUBPASS_CONTENTS_INLINE, VkRenderPassBeginInfo{
																		 VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
																		 /*pNext =*/nullptr, source.renderPass,
																		 /*framebuffer =*/playerView->GetFramebuffer(), renderArea,
																		 /*clearValueCount =*/0u,  // No clear values.
																		 /*pClearValues =*/nullptr // No clear values.
																	 });
				_commands.SetViewports(0u, { GetViewport(renderArea) });
				for (const View& view : views) {
					//	Draw the things.
					//	_opaqueBatches.RecordDraws(_commands, view, )
				}
				_commands.FinishRendering();
			}
		}

		ET_FAIL_UNLESS(_commands.FinishRecording());

		// clang-format off
		ET_FAIL_UNLESS(vkResetFences(gpu, 1u, &_commandsConsumed));
		return gpu.SubmitAsync(QueueConcept::Drawing, _commandsConsumed, { 
			AsSubmitInfo( { scene._resourcesReady }, { AllCommandsStage }, { _commands.Get() } )
		}); // clang-format on
	}

	// ---------------------------------------------------

	VkResult VulkanGraphicsScene::Frame::BindResources(Gpu& gpu) {
		using ::Eldritch2::Swap;

		CommandList commands;
		ET_FAIL_UNLESS(commands.BindResources(gpu, QueueConcept::Drawing, 0u));
		ET_AT_SCOPE_EXIT(commands.FreeResources(gpu));

		VkFence                 commandsConsumed;
		const VkFenceCreateInfo commandsConsumedInfo{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			nullptr,
			VK_FENCE_CREATE_SIGNALED_BIT
		};
		ET_FAIL_UNLESS(vkCreateFence(gpu, &commandsConsumedInfo, gpu.GetAllocationCallbacks(), &commandsConsumed));
		ET_AT_SCOPE_EXIT(vkDestroyFence(gpu, commandsConsumed, gpu.GetAllocationCallbacks()));

		Swap(_commands, commands);
		Swap(_commandsConsumed, commandsConsumed);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void VulkanGraphicsScene::Frame::FreeResources(Gpu& gpu) {
		vkDestroyFence(gpu, eastl::exchange(_commandsConsumed, nullptr), gpu.GetAllocationCallbacks());
		_commands.FreeResources(gpu);
	}

	// ---------------------------------------------------

	VulkanGraphicsScene::VulkanGraphicsScene() :
		GraphicsScene(GeometryCellDimensions, LightCellDimensions),
		_resourcesReady(nullptr),
		_transforms(),
		_opaqueBatches(),
		_playerViews(MallocAllocator("Vulkan Graphics Scene Player View List Allocator")),
		_frameId(InitialFrameId),
		_queuedFrames{} {
	}

	// ---------------------------------------------------

	VkResult VulkanGraphicsScene::SubmitViewIndependentCommands(JobExecutor& executor, Gpu& gpu) {
		executor.AwaitWork([this](JobExecutor& executor) { BuildAccelerators(executor); }, [this, &gpu](JobExecutor& executor) {
			for (PlayerView* const view : _playerViews) {
				view->BindResources(gpu);
			} });

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult VulkanGraphicsScene::SubmitViewDependentCommands(JobExecutor& executor, Gpu& gpu) {
		/*  We now have spatial acceleration structures set up; it's time to determine the visible set for each camera
		registered with the scene. */
		Frame& frame(_queuedFrames[_frameId++ % _countof(_queuedFrames)]);

		//  Wait for previous frame commands to drain to avoid data hazards.
		executor.AwaitCondition(frame.CheckCommandsConsumed(gpu));

		return frame.SubmitCommands(gpu, *this);
	}

	// ---------------------------------------------------

	VkResult VulkanGraphicsScene::BindResources(JobExecutor& /*executor*/, Gpu& gpu, GpuResourceBus& bus, VkDeviceSize transformArenaSize) {
		using Eldritch2::Swap;

		VkSemaphore resourcesReady;
		ET_FAIL_UNLESS(bus.InsertSignalSemaphore(gpu, resourcesReady));
		ET_AT_SCOPE_EXIT(bus.EraseSignalSemaphore(gpu, resourcesReady));

		UniformBuffer transforms;
		ET_FAIL_UNLESS(transforms.BindResources(gpu, transformArenaSize * MaxQueuedFrames));
		ET_AT_SCOPE_EXIT(transforms.FreeResources(gpu));

		BatchCoordinator batchCoordinator;
		ET_FAIL_UNLESS(batchCoordinator.BindResources(gpu));
		ET_AT_SCOPE_EXIT(batchCoordinator.FreeResources(gpu));

		GraphicsPipelineBuilder shadowPipelineBuilder;
		shadowPipelineBuilder.DefineAttachment(VK_FORMAT_D16_UNORM_S8_UINT, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);

		shadowPipelineBuilder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, "DepthOnly");
		shadowPipelineBuilder.AttachDepthStencilBuffer(0u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

		shadowPipelineBuilder.Finish();

		GraphicsPipeline shadowPipeline;
		ET_FAIL_UNLESS(shadowPipeline.BindResources(gpu, shadowPipelineBuilder));
		ET_AT_SCOPE_EXIT(shadowPipeline.FreeResources(gpu));

		Framebuffer shadowAtlas;
		ET_FAIL_UNLESS(shadowAtlas.BindResources(gpu, shadowPipeline, VkExtent2D{ 1024u, 1024u }));
		ET_AT_SCOPE_EXIT(shadowAtlas.FreeResources(gpu));

		GraphicsPipelineBuilder mainPipelineBuilder;
		mainPipelineBuilder.DefineAttachment(VK_FORMAT_R8G8B8A8_SRGB, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		mainPipelineBuilder.DefineAttachment(VK_FORMAT_D32_SFLOAT_S8_UINT, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);

		mainPipelineBuilder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, "DepthPrepass");
		mainPipelineBuilder.AttachDepthStencilBuffer(0u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

		mainPipelineBuilder.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, "OpaqueLit");
		mainPipelineBuilder.AttachColorOutput(0u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		mainPipelineBuilder.AttachDepthStencilBuffer(1u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);

		mainPipelineBuilder.Finish();

		GraphicsPipeline mainPipeline;
		ET_FAIL_UNLESS(mainPipeline.BindResources(gpu, mainPipelineBuilder));
		ET_AT_SCOPE_EXIT(mainPipeline.FreeResources(gpu));

		DescriptorTable resourceDescriptors;
		ET_FAIL_UNLESS(resourceDescriptors.BindResources(gpu, mainPipeline.GetDescriptorLayout(), 0u, nullptr));
		ET_AT_SCOPE_EXIT(resourceDescriptors.FreeResources(gpu));

		Frame queuedFrames[_countof(_queuedFrames)];
		ET_AT_SCOPE_EXIT(for (Frame& frame
							  : queuedFrames) frame.FreeResources(gpu));
		for (Frame& frame : queuedFrames) {
			ET_FAIL_UNLESS(frame.BindResources(gpu));
		}

		Swap(_resourcesReady, resourcesReady);
		Swap(_transforms, transforms);
		Swap(_opaqueBatches, batchCoordinator);
		Swap(_shadowPipeline, shadowPipeline);
		Swap(_shadowAtlas, shadowAtlas);
		_frameId = InitialFrameId;
		Swap(_queuedFrames, queuedFrames);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void VulkanGraphicsScene::FreeResources(JobExecutor& executor, Gpu& gpu, GpuResourceBus& bus) {
		for (Frame& frame : _queuedFrames) {
			executor.AwaitCondition(frame.CheckCommandsConsumed(gpu));
			frame.FreeResources(gpu);
		}
		_resourceDescriptors.FreeResources(gpu);
		_mainPipeline.FreeResources(gpu);
		_shadowAtlas.FreeResources(gpu);
		_shadowPipeline.FreeResources(gpu);
		_opaqueBatches.FreeResources(gpu);
		_transforms.FreeResources(gpu);

		//	Bus commands need to finish execution before we can destroy our semaphore.
		executor.AwaitCondition(bus.CheckCommandsConsumed(gpu));
		bus.EraseSignalSemaphore(gpu, eastl::exchange(_resourcesReady, nullptr));
	}

	// ---------------------------------------------------

	void Swap(VulkanGraphicsScene::Frame& lhs, VulkanGraphicsScene::Frame& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._commands, rhs._commands);
		Swap(lhs._commandsConsumed, rhs._commandsConsumed);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
