/*==================================================================*\
  VulkanEngineComponent.WorldComponent.cpp
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
#include <Graphics/Vulkan/VulkanEngineComponent.hpp>
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/DisplayLocator.hpp>
#include <Graphics/Vulkan/CommandList.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Core/Profiler.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	namespace {

		static ETConstexpr StageResolution StandardResolution(/*width =*/1.0f, /*height =*/1.0f);

		// ---

		ETForceInlineHint PipelineBuilder& BuildShadowMapPipeline(PipelineBuilder& pipeline, GpuFormat depthFormat = GpuFormat::D32_Float_S8_Uint, uint32 samplesPerPixel = 1u) {
			enum : PipelineBuilder::AttachmentIndex {
				DepthAttachment,
			};

			pipeline.DefineSharedAttachment(depthFormat, /*mips =*/1u, /*isPersistent =*/true);

			pipeline.DefineDrawStage("DepthOnly", StandardResolution, samplesPerPixel, StageAttachment(DepthAttachment, /*mip =*/0u), {}, {});

			return pipeline.Optimize(/*passes =*/);
		}

		// ---------------------------------------------------

		ETForceInlineHint PipelineBuilder& BuildLitPipeline(PipelineBuilder& pipeline, GpuFormat sceneColorFormat = GpuFormat::R8G8B8A8_Srgb, GpuFormat depthFormat = GpuFormat::D32_Float_S8_Uint, uint32 samplesPerPixel = 1u) {
			enum : PipelineBuilder::AttachmentIndex {
				DepthAttachment,
				SceneColorAttachment
			};

			pipeline.DefineSharedAttachment(depthFormat, /*mips =*/1u, /*isPersistent =*/false);
			pipeline.DefineSharedAttachment(sceneColorFormat, /*mips =*/1u, /*isPersistent =*/false);

			pipeline.DefineDrawStage("DepthOnly", StandardResolution, samplesPerPixel, StageAttachment(DepthAttachment, /*mip =*/0u), {}, {});

			static ETConstexpr StageAttachment interactionAttachments[] = { StageAttachment(SceneColorAttachment, /*mip =*/0u), };
			pipeline.DefineDrawStage("OpaqueInteraction", StandardResolution, samplesPerPixel, StageAttachment(DepthAttachment, /*mip =*/0u), {}, { Begin(interactionAttachments), End(interactionAttachments) });

			pipeline.DefineDrawStage("TranslucentInteraction", StandardResolution, /*samplesPerPixel =*/1u, StageAttachment(DepthAttachment, /*mip =*/0u), { Begin(interactionAttachments), End(interactionAttachments) }, { Begin(interactionAttachments), End(interactionAttachments) });

			return pipeline.Optimize(/*passes =*/);
		}

	} // anonymous namespace

	VulkanEngineComponent::FrameBarrier::FrameBarrier() ETNoexceptHint : _consumed(NullVulkanHandle), _waits(WaitList::AllocatorType("Vulkan Frame Wait Semaphore List Allocator")) {}

	// ---------------------------------------------------

	bool VulkanEngineComponent::FrameBarrier::IsConsumed(Gpu& gpu) const ETNoexceptHint {
		return _consumed == NullVulkanHandle || vkGetFenceStatus(gpu, _consumed) != VK_NOT_READY;
	}

	// ---------------------------------------------------

	VkResult VulkanEngineComponent::FrameBarrier::BindResources(Gpu& gpu) {
		using ::Eldritch2::Swap;

		VkFence                 consumed;
		const VkFenceCreateInfo consumedInfo{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			/*pNext =*/nullptr,
			VK_FENCE_CREATE_SIGNALED_BIT
		};
		ET_ABORT_UNLESS(vkCreateFence(gpu, ETAddressOf(consumedInfo), gpu.GetAllocationCallbacks(), ETAddressOf(consumed)));
		ET_AT_SCOPE_EXIT(vkDestroyFence(gpu, consumed, gpu.GetAllocationCallbacks()));

		WaitList waits(_waits.GetAllocator());

		Swap(_consumed, consumed);
		Swap(_waits, waits);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::FrameBarrier::FreeResources(Gpu& gpu) ETNoexceptHint {
		vkDestroyFence(gpu, Exchange(_consumed, NullVulkanHandle), gpu.GetAllocationCallbacks());
		_waits.Clear();
	}

	// ---------------------------------------------------

	VulkanEngineComponent::WorldComponent::WorldComponent(const ObjectInjector& services) ETNoexceptHint : Core::WorldComponent(services), _scene(nullptr), _displays(nullptr) {}

	// ---------------------------------------------------

	void VulkanEngineComponent::WorldComponent::BindResources(JobExecutor& executor) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/LateInitialization", "Bind Vulkan scene resources", 0x0C11F0);

		executor.AwaitWork(
			[this](JobExecutor& /*executor*/) ETNoexceptHint {
				ET_TERMINATE_WORLD_IF(_displays == nullptr);

				ET_TERMINATE_WORLD_IF_FAILED(_displays->BindResources(*Inject<Gpu>(), *Inject<DisplayBus>()));
			},
			[this](JobExecutor& /*executor*/) ETNoexceptHint {
				ET_TERMINATE_WORLD_IF(_scene == nullptr);

				PipelineBuilder litPipeline, shadowMapPipeline;
				ET_TERMINATE_WORLD_IF_FAILED(_scene->BindResources(*Inject<Gpu>(), BuildLitPipeline(litPipeline), BuildShadowMapPipeline(shadowMapPipeline), /*shadowResolution =*/{ 512u, 512u }));
			},
			[this](JobExecutor& /*executor*/) ETNoexceptHint {
				using ::Eldritch2::Swap;

				FrameBarrier frames[ETCountOf(_frames)];
				Gpu&         gpu(*Inject<Gpu>());
				ET_FOREACH_AT_SCOPE_EXIT(frame, frames, frame.FreeResources(gpu));
				for (FrameBarrier& frame : frames) {
					ET_TERMINATE_WORLD_IF_FAILED(frame.BindResources(gpu));
				}

				Swap(_frames, frames);
			});
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::WorldComponent::FreeResources(JobExecutor& executor) ETNoexceptHint {
		Gpu& gpu(*Inject<Gpu>());
		for (FrameBarrier& frame : _frames) {
			executor.AwaitCondition(frame.IsConsumed(gpu));
			frame.FreeResources(gpu);
		}

		if (VulkanGraphicsScene* const scene = Exchange(_scene, nullptr)) {
			_scene->FreeResources(gpu);
		}

		if (DisplayLocator* const displays = Exchange(_displays, nullptr)) {
			_displays->FreeResources(gpu);
		}
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::WorldComponent::OnVariableRateTick(JobExecutor& executor, MicrosecondTime /*tickDuration*/, float32 /*residualFraction*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/VariableTick", "Draw scene", 0x0C11F0);

		executor.AwaitWork(
			[& lights = _scene->GetInstances(DynamicLight)](JobExecutor& executor) ETNoexceptHint {
				ET_PROFILE_SCOPE("World/VariableTick", "Update Accelerators (Dynamic Lights)", 0xFFAAFF);

				if (lights.ShouldRebuildHierarchy()) {
					lights.Sort(executor);
				}
			},
			[& meshes = _scene->GetInstances(DynamicMesh)](JobExecutor& executor) ETNoexceptHint {
				ET_PROFILE_SCOPE("World/VariableTick", "Update Accelerators (Dynamic Meshes)", 0xFFAAFF);

				if (meshes.ShouldRebuildHierarchy()) {
					meshes.Sort(executor);
				}
			});

		ArrayList<RenderView> sceneViews(ArrayList<RenderView>::AllocatorType("Scene View List Allocator"));

		BuildViewList(sceneViews, _scene->GetInstances(DynamicMesh));
		BuildViewList(sceneViews, _scene->GetInstances(WorldMesh));

		BuildShadowViewList(sceneViews, _scene->GetInstances(DynamicLight));
		BuildShadowViewList(sceneViews, _scene->GetInstances(WorldLight));

		GraphicsCommandList drawCommands;
		Gpu*                gpu(Inject<Gpu>());

		ET_TERMINATE_WORLD_IF_FAILED(drawCommands.BindResources(*gpu, QueueClass::Drawing));
		drawCommands.ExecutePipeline(_scene->GetShadowAtlas(), _scene->GetShadowMapPipeline(), sceneViews);

		ET_TERMINATE_WORLD_IF_FAILED(drawCommands.Submit(*gpu, _frames[0]._consumed, uint32(_frames[0]._waits.GetSize()), _frames[0]._waits.GetData<VkSemaphore>(), _frames[0]._waits.GetData<VkPipelineStageFlags>()));
		ET_TERMINATE_WORLD_IF_FAILED(_displays->PresentImages());
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::WorldComponent::OnFixedRateTickLate(JobExecutor& executor, MicrosecondTime /*duration*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/FixedTick", "Update GPU descriptors", 0x0C11F0);

		executor.AwaitWork(
			[this, gpu = Inject<Device>()](JobExecutor& executor) ETNoexceptHint {
				ET_PROFILE_SCOPE("World/FixedTick", "Update GPU Descriptors (Shadow Maps)", 0xFFAAFF);

				GraphicsPipeline& pipeline(_scene->GetShadowMapPipeline());

				// New resource views can be created w/o a stall to wait for command consumption.
				ET_TERMINATE_WORLD_IF_FAILED(pipeline.BindResourceViews(*gpu));
				executor.AwaitCondition(_frames[0].IsConsumed(*gpu));
				pipeline.PushToGpu(*gpu);
			},
			[this, gpu = Inject<Device>()](JobExecutor& executor) ETNoexceptHint {
				ET_PROFILE_SCOPE("World/FixedTick", "Update GPU Descriptors", 0xFFAAFF);

				GraphicsPipeline& pipeline(_scene->GetLitPipeline());

				// New resource views can safely be created w/o a stall to wait for command consumption.
				ET_TERMINATE_WORLD_IF_FAILED(pipeline.BindResourceViews(*gpu));
				executor.AwaitCondition(_frames[0].IsConsumed(*gpu));
				pipeline.PushToGpu(*gpu);
			});
	}

}}} // namespace Eldritch2::Graphics::Vulkan
