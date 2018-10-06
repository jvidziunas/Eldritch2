/*==================================================================*\
  VulkanWorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.hpp>
#include <Graphics/Vulkan/VulkanWorldComponent.hpp>
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/DisplayLocator.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

#define ET_TERMINATE_WORLD_UNLESS(cond)                  \
	if (!(cond)) {                                       \
		FindService<Core::World>()->SetShouldShutDown(); \
		return;                                          \
	}

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Core;

	namespace {

		ETInlineHint ETForceInlineHint GraphicsPipelineBuilder& BuildShadowPipeline(GraphicsPipelineBuilder& pipeline, VkFormat depthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT, uint32 samplesPerPixel = 1u) {
			pipeline.BeginPass(PassType::Draw, { /*width =*/1.0f, /*height =*/1.0f }, samplesPerPixel, "DepthOnly");
			pipeline.SetDepthStencilBuffer(/*attachment =*/pipeline.DefineAttachment(depthFormat, /*mips =*/1u, /*isPersistent =*/true));

			return pipeline.Optimize(/*passes =*/);
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint GraphicsPipelineBuilder& BuildLitPipeline(GraphicsPipelineBuilder& pipeline, VkFormat sceneColorFormat = VK_FORMAT_R8G8B8A8_SRGB, VkFormat depthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT, uint32 samplesPerPixel = 1u) {
			static ETConstexpr PassDimensions UnitDimensions { /*width =*/1.0f, /*height =*/1.0f };
			const uint32                      depthBufferId(pipeline.DefineAttachment(depthFormat, /*mips =*/1u, /*isPersistent =*/false));
			const uint32                      sceneColorId(pipeline.DefineAttachment(sceneColorFormat, /*mips =*/1u, /*isPersistent =*/false));
			const uint32

				pipeline.BeginPass(PassType::Draw, UnitDimensions, samplesPerPixel, "DepthOnly");
			pipeline.SetDepthStencilBuffer(/*attachment =*/depthBufferId);

			pipeline.BeginPass(PassType::Draw, UnitDimensions, samplesPerPixel, "OpaqueLit");
			pipeline.AppendColorOutput(/*attachment =*/sceneColorId);
			pipeline.SetDepthStencilBuffer(depthBufferId);

			pipeline.BeginPass(PassType::Draw, UnitDimensions, /*samplesPerPixel =*/1u, "ClusteredTranslucent");
			pipeline.AppendColorOutput(/*attachment =*/sceneColorId);
			// pipeline.AppendInput(/*attachment =*/sceneColorId);
			pipeline.SetDepthStencilBuffer(depthBufferId);

			return pipeline.Optimize(/*passes =*/);
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ArrayList<View> BuildShadowViewList(const VulkanGraphicsScene& scene) {
			return ArrayList<View>(MallocAllocator("Frame Shadow View List Allocator"));
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ArrayList<View> BuildViewList(const VulkanGraphicsScene& scene, View&& root) {
			ArrayList<View> views(MallocAllocator("Frame View List Allocator"));
			views.Append(eastl::move(root));

			/*  We cannot use iterators for traversal since list resize operations invalidate iterators and it's almost
			 *	guaranteed there will be at least one resize. */
			for (size_t viewId(0u); viewId < views.GetSize(); ++viewId) {
				const View& view(views[viewId]);
				for (const PortalView* portal : scene.GetConcept(PortalViews)) {
					portal->portalPlane.Reflect(Transformation::MakeIdentity());
				}
			}

			return eastl::move(views);
		}

	} // anonymous namespace

	VulkanWorldComponent::Frame::Frame() :
		_drawsConsumed(nullptr),
		_waits(MallocAllocator("Vulkan Frame Wait Semaphore List Allocator")),
		_signals(MallocAllocator("Vulkan Frame Signal Semaphore List Allocator")) {
	}

	// ---------------------------------------------------

	bool VulkanWorldComponent::Frame::IsConsumed(Gpu& gpu) const ETNoexceptHint {
		return _drawsConsumed == VK_NULL_HANDLE || vkGetFenceStatus(gpu, _drawsConsumed) != VK_NOT_READY;
	}

	// ---------------------------------------------------

	VkResult VulkanWorldComponent::Frame::RecordCommands(Gpu& gpu, const VulkanGraphicsScene& scene) {
		ET_ABORT_UNLESS(_drawCommands.BeginRecording(gpu, /*resetFlags =*/0u, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));

		_drawCommands.ExecutePipeline(scene.GetShadowAtlas(), scene.GetShadowPipeline(), BuildShadowViewList(scene));
		for (const PlayerView* view : scene.GetRootViews()) {
			_drawCommands.ExecutePipeline(view->GetTarget(), scene.GetLitPipeline(), BuildViewList(scene, View {}));
		}

		return _drawCommands.FinishRecording();
	}

	// ---------------------------------------------------

	VkResult VulkanWorldComponent::Frame::SubmitCommands(Gpu& gpu) const {
		ET_ABORT_UNLESS(vkResetFences(gpu, /*fenceCount =*/1u, ETAddressOf(_drawsConsumed)));
		return gpu.SubmitAsync(Drawing, _drawsConsumed, { // clang-format off
			VkSubmitInfo { 
				VK_STRUCTURE_TYPE_SUBMIT_INFO,
				/*pNext =*/nullptr,
				uint32(_waits.GetSize()),
				_waits.GetData<VkSemaphore>(),
				_waits.GetData<VkPipelineStageFlags>(),
				uint32(ETCountOf(_drawCommands.Get())),
				_drawCommands.Get(),
				uint32(_signals.GetSize()),
				_signals.GetData() }}); // clang-format on
	}

	// ---------------------------------------------------

	VkResult VulkanWorldComponent::Frame::BindResources(Gpu& gpu) {
		using ::Eldritch2::Swap;

		CommandList drawCommands;
		ET_ABORT_UNLESS(drawCommands.BindResources(gpu, Drawing, /*poolFlags =*/0u));
		ET_AT_SCOPE_EXIT(drawCommands.FreeResources(gpu));

		VkFence                 drawsConsumed;
		const VkFenceCreateInfo drawsConsumedInfo {
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			/*pNext =*/nullptr,
			VK_FENCE_CREATE_SIGNALED_BIT
		};
		ET_ABORT_UNLESS(vkCreateFence(gpu, ETAddressOf(drawsConsumedInfo), gpu.GetAllocationCallbacks(), ETAddressOf(drawsConsumed)));
		ET_AT_SCOPE_EXIT(vkDestroyFence(gpu, drawsConsumed, gpu.GetAllocationCallbacks()));

		Swap(_drawCommands, drawCommands);
		Swap(_drawsConsumed, drawsConsumed);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void VulkanWorldComponent::Frame::FreeResources(Gpu& gpu) {
		vkDestroyFence(gpu, eastl::exchange(_drawsConsumed, nullptr), gpu.GetAllocationCallbacks());
		_drawCommands.FreeResources(gpu);
	}

	// ---------------------------------------------------

	VulkanWorldComponent::VulkanWorldComponent(const ObjectLocator& services) :
		WorldComponent(services),
		_scene(nullptr),
		_displays(nullptr) {}

	// ---------------------------------------------------

	void VulkanWorldComponent::BindResources(JobExecutor& executor) {
		ET_PROFILE_SCOPE("World/LateInitialization", "Bind Vulkan scene resources", 0x0C11F0);
		using ::Eldritch2::Swap;

		executor.AwaitWork([this](JobExecutor& executor) { // clang-format off
			GraphicsPipelineBuilder litPipeline, shadowPipeline;
			Device&                 gpu(FindService<Vulkan>()->GetDevice());

			ET_TERMINATE_WORLD_UNLESS(_displays && Succeeded(_displays->BindResources(gpu.GetDisplays(), gpu.GetDisplaysMutex())));
			ET_TERMINATE_WORLD_UNLESS(_scene && Succeeded(_scene->BindResources(gpu, BuildLitPipeline(litPipeline), BuildShadowPipeline(shadowPipeline), /*shadowResolution =*/ { 512u, 512u }, /*transformArenaSize =*/16384u)));
		}, [this](JobExecutor& executor) {
			Device& gpu(FindService<Vulkan>()->GetDevice());
			Frame   frames[ETCountOf(_frames)];

			ET_AT_SCOPE_EXIT(for (Frame& frame
								  : frames) frame.FreeResources(gpu));
			for (Frame& frame : frames) {
				ET_TERMINATE_WORLD_UNLESS(Succeeded(frame.BindResources(gpu)));
			}

			Swap(_frames, frames);
		}); // clang-format on
	}

	// ---------------------------------------------------

	void VulkanWorldComponent::FreeResources(JobExecutor& executor) {
		Device& gpu(FindService<Vulkan>()->GetDevice());
		for (Frame& frame : _frames) {
			executor.AwaitCondition(frame.IsConsumed(gpu));
			frame.FreeResources(gpu);
		}

		if (VulkanGraphicsScene* const scene = eastl::exchange(_scene, nullptr)) {
			_scene->FreeResources(gpu);
		}

		if (DisplayLocator* const displays = eastl::exchange(_displays, nullptr)) {
			_displays->FreeResources();
		}
	}

	// ---------------------------------------------------

	void VulkanWorldComponent::OnVariableRateTick(JobExecutor& executor, MicrosecondTime /*tickDuration*/, float32 /*residualFraction*/) {
		ET_PROFILE_SCOPE("World/VariableTick", "Draw scene", 0x0C11F0);

		Device& gpu(FindService<Vulkan>()->GetDevice());
		Frame&  frame(_frames[0]);

		executor.AwaitCondition(frame.IsConsumed(gpu));
		if (Succeeded(frame.RecordCommands(gpu, *_scene))) {
			frame.SubmitCommands(gpu);
		}
	}

}}} // namespace Eldritch2::Graphics::Vulkan
