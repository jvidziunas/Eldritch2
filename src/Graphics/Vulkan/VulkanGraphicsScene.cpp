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
#include <Graphics/Vulkan/FrameTransferBus.hpp>
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

		ETInlineHint ArrayList<View> BuildViewList(const VulkanGraphicsScene& scene, View&& root) {
			ArrayList<View> views(MallocAllocator("Frame View Stack Allocator"));

			views.Append(eastl::forward<View>(root));
			/*  We cannot use iterators for traversal since we may append one or more child views to the end of the list.
			 *	List resize operations invalidate iterators and we cannot guarantee the list will not be resized. */
			for (size_t viewId(0u); viewId < views.GetSize(); ++viewId) {
				const View& view(views[viewId]);

				for (const PortalView* portal : scene.GetConcept(PortalViewType::PortalViews)) {
				}
			}

			return eastl::move(views);
		}

		// ---------------------------------------------------

		ETInlineHint GraphicsPipelineBuilder& ConstructShadowPipeline(GraphicsPipelineBuilder& pipeline) {
			pipeline.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, 1.0f, 1.0f, "DepthOnly");

			pipeline.SetDepthStencilBuffer(/*attachment =*/pipeline.DefineAttachment(VK_FORMAT_D32_SFLOAT_S8_UINT, VK_SAMPLE_COUNT_1_BIT));

			pipeline.Finish(/*andOptimize =*/false);

			return pipeline;
		}

		// ---------------------------------------------------

		ETInlineHint GraphicsPipelineBuilder& BuildOpaqueLitPipeline(GraphicsPipelineBuilder& pipeline) {
			const uint32 backbufferId(pipeline.DefineAttachment(VK_FORMAT_R8G8B8A8_SRGB, VK_SAMPLE_COUNT_1_BIT));
			const uint32 depthBufferId(pipeline.DefineAttachment(VK_FORMAT_D32_SFLOAT_S8_UINT, VK_SAMPLE_COUNT_1_BIT));

			pipeline.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, 1.0f, 1.0f, "DepthOnly");
			pipeline.SetDepthStencilBuffer(depthBufferId, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

			pipeline.BeginPass(VK_PIPELINE_BIND_POINT_GRAPHICS, 1.0f, 1.0f, "OpaqueLit");
			pipeline.AppendColorOutput(backbufferId, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			pipeline.SetDepthStencilBuffer(depthBufferId, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);

			pipeline.Finish(/*andOptimize =*/false);

			return pipeline;
		}

	} // anonymous namespace

	VulkanGraphicsScene::Frame::Frame() :
		_drawsConsumed(nullptr),
		_startExecution(nullptr),
		_finishExecution(nullptr) {
	}

	// ---------------------------------------------------

	bool VulkanGraphicsScene::Frame::CheckCommandsConsumed(Gpu& gpu) const {
		return vkGetFenceStatus(gpu, _drawsConsumed) != VK_NOT_READY;
	}

	// ---------------------------------------------------

	VkResult VulkanGraphicsScene::Frame::SubmitCommands(Gpu& gpu, const VulkanGraphicsScene& scene) {
		constexpr VkPipelineStageFlags AllGraphics = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;

		ET_FAIL_UNLESS(_commands.ResetPool(gpu));
		ET_FAIL_UNLESS(_commands.BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
		_commands.ExecutePipeline(scene._shadowPipeline, scene._resourceDescriptors, scene._shadowAtlas);
		for (const PlayerView* view : scene._rootViews) {
			const ArrayList<View> views(BuildViewList(scene, View { {}, VkRect2D {}, 0u }));
			//	_commands.ExecutePipeline(scene._mainPipeline, scene._resourceDescriptors, view->);
		}
		ET_FAIL_UNLESS(_commands.FinishRecording());

		ET_FAIL_UNLESS(vkResetFences(gpu, /*fenceCount =*/1u, &_drawsConsumed));
		return gpu.SubmitAsync(QueueConcept::Drawing, _drawsConsumed, { // clang-format off
			AsSubmitInfo({ scene._resourcesReady, _startExecution },
						 { AllGraphics,           AllGraphics },
						 { _commands.Get() },
						 { _finishExecution }) }); // clang-format on
	}

	// ---------------------------------------------------

	VkResult VulkanGraphicsScene::Frame::BindResources(Gpu& gpu) {
		using ::Eldritch2::Swap;

		CommandList commands;
		ET_FAIL_UNLESS(commands.BindResources(gpu, QueueConcept::Drawing, /*flags =*/0u));
		ET_AT_SCOPE_EXIT(commands.FreeResources(gpu));

		VkFence                 drawsConsumed;
		const VkFenceCreateInfo drawsConsumedInfo {
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			/*pNext =*/nullptr,
			VK_FENCE_CREATE_SIGNALED_BIT
		};
		ET_FAIL_UNLESS(vkCreateFence(gpu, &drawsConsumedInfo, gpu.GetAllocationCallbacks(), &drawsConsumed));
		ET_AT_SCOPE_EXIT(vkDestroyFence(gpu, drawsConsumed, gpu.GetAllocationCallbacks()));

		Swap(_commands, commands);
		Swap(_drawsConsumed, drawsConsumed);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void VulkanGraphicsScene::Frame::FreeResources(Gpu& gpu) {
		vkDestroyFence(gpu, eastl::exchange(_drawsConsumed, nullptr), gpu.GetAllocationCallbacks());
		_commands.FreeResources(gpu);
	}

	// ---------------------------------------------------

	VulkanGraphicsScene::VulkanGraphicsScene() :
		GraphicsScene(GeometryCellDimensions, LightCellDimensions),
		_rootViews(MallocAllocator("Vulkan Graphics Scene Player View List Allocator")),
		_resourcesReady(nullptr),
		_transforms(),
		_opaqueBatches(),
		_frameId(InitialFrameId),
		_queuedFrames {} {
	}

	// ---------------------------------------------------

	VkResult VulkanGraphicsScene::SubmitViewIndependentCommands(JobExecutor& executor, Gpu& /*gpu*/) {
		BuildAccelerators(executor);

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

	VkResult VulkanGraphicsScene::BindResources(JobExecutor& /*executor*/, Gpu& gpu, FrameTransferBus& bus, VkDeviceSize transformArenaSize) {
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
		GraphicsPipeline        shadowPipeline;
		ET_FAIL_UNLESS(shadowPipeline.BindResources(gpu, ConstructShadowPipeline(shadowPipelineBuilder)));
		ET_AT_SCOPE_EXIT(shadowPipeline.FreeResources(gpu));

		Framebuffer shadowAtlas;
		ET_FAIL_UNLESS(shadowAtlas.BindResources(gpu, shadowPipeline, VkExtent2D { 1024u, 1024u }));
		ET_AT_SCOPE_EXIT(shadowAtlas.FreeResources(gpu));

		GraphicsPipelineBuilder mainPipelineBuilder;
		GraphicsPipeline        mainPipeline;
		ET_FAIL_UNLESS(mainPipeline.BindResources(gpu, BuildOpaqueLitPipeline(mainPipelineBuilder)));
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

	void VulkanGraphicsScene::FreeResources(JobExecutor& executor, Gpu& gpu, FrameTransferBus& bus) {
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
		Swap(lhs._drawsConsumed, rhs._drawsConsumed);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
