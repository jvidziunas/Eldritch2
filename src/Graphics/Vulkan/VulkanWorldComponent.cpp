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
#include <Graphics/Vulkan/VulkanWorldComponent.hpp>
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Core;

	namespace {

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
				}
			}

			return eastl::move(views);
		}

	} // anonymous namespace

	Frame::Frame() :
		_drawsConsumed(nullptr),
		_waits(MallocAllocator("Vulkan Frame Wait Semaphore List Allocator")),
		_signals(MallocAllocator("Vulkan Frame Signal Semaphore List Allocator")) {
	}

	// ---------------------------------------------------

	bool Frame::IsConsumed(Gpu& gpu) const ETNoexceptHint {
		return _drawsConsumed == VK_NULL_HANDLE || vkGetFenceStatus(gpu, _drawsConsumed) != VK_NOT_READY;
	}

	// ---------------------------------------------------

	VkResult Frame::RecordCommands(Gpu& gpu, const VulkanGraphicsScene& scene) {
		ET_ABORT_UNLESS(_drawCommands.BeginRecording(gpu, /*resetFlags =*/0u, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));

		_drawCommands.ExecutePipeline(scene.GetShadowAtlas(), scene.GetShadowPipeline(), scene.GetShaderResources(), BuildShadowViewList(scene));
		for (const PlayerView* view : scene.GetRootViews()) {
			_drawCommands.ExecutePipeline(view->GetTarget(), scene.GetOpaqueLitPipeline(), scene.GetShaderResources(), BuildViewList(scene, View {}));
		}

		return _drawCommands.FinishRecording();
	}

	// ---------------------------------------------------

	VkResult Frame::SubmitCommands(Gpu& gpu) {
		const VkCommandBuffer commandBuffers[] = { _drawCommands.Get() };
		ET_ABORT_UNLESS(vkResetFences(gpu, /*fenceCount =*/1u, ETAddressOf(_drawsConsumed)));
		return gpu.SubmitAsync(Drawing, _drawsConsumed, { // clang-format off
			VkSubmitInfo { 
				VK_STRUCTURE_TYPE_SUBMIT_INFO,
				/*pNext =*/nullptr,
				uint32(_waits.GetSize()),
				_waits.Get<VkSemaphore>(),
				_waits.Get<VkPipelineStageFlags>(),
				uint32(ETCountOf(commandBuffers)),
				commandBuffers,
				uint32(_signals.GetSize()),
				_signals.GetData() }}); // clang-format on
	}

	// ---------------------------------------------------

	VkResult Frame::BindResources(Gpu& gpu) {
		using ::Eldritch2::Swap;

		CommandList drawCommands;
		ET_ABORT_UNLESS(drawCommands.BindResources(gpu, Drawing, /*flags =*/0u));
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

	void Frame::FreeResources(Gpu& gpu) {
		vkDestroyFence(gpu, eastl::exchange(_drawsConsumed, nullptr), gpu.GetAllocationCallbacks());
		_drawCommands.FreeResources(gpu);
	}

	// ---------------------------------------------------

	VulkanWorldComponent::VulkanWorldComponent(const ObjectLocator& services) :
		WorldComponent(services),
		_scene(nullptr),
		_displays(nullptr) {}

	// ---------------------------------------------------

	void VulkanWorldComponent::BindResources(JobExecutor& /*executor*/) {
		using ::Eldritch2::Swap;

		static ETConstexpr uint32 ShadowAxisResolution = 512u;
		static ETConstexpr VkExtent2D ShadowMapResolution { ShadowAxisResolution, ShadowAxisResolution };

		ET_PROFILE_SCOPE("World/LateInitialization", "Bind Vulkan scene resources", 0x0C11F0);
		Vulkan::Device& gpu(FindService<Vulkan>()->GetPrimaryDevice());

		if (ET_UNLIKELY(Failed(_scene->BindResources(gpu, ShadowMapResolution, 16384u)))) {
			return FindService<World>()->SetShouldShutDown();
		}

		Frame frames[ETCountOf(_frames)];
		ET_AT_SCOPE_EXIT(for (Frame& frame
							  : frames) frame.FreeResources(gpu));
		for (Frame& frame : frames) {
			if (ET_UNLIKELY((frame.BindResources(gpu)))) {
				return FindService<World>()->SetShouldShutDown();
			}
		}

		Swap(_frames, frames);
	}

	// ---------------------------------------------------

	void VulkanWorldComponent::FreeResources(JobExecutor& executor) {
		Vulkan::Device& gpu(FindService<Vulkan>()->GetPrimaryDevice());

		for (Frame& frame : _frames) {
			executor.AwaitCondition(frame.IsConsumed(gpu));
			frame.FreeResources(gpu);
		}

		if (_scene) {
			_scene->FreeResources(gpu);
		}
	}

	// ---------------------------------------------------

	void VulkanWorldComponent::OnVariableRateTick(JobExecutor& executor, MicrosecondTime /*tickDuration*/, float32 /*residualFraction*/) {
		ET_PROFILE_SCOPE("World/VariableTick", "Draw scene", 0x0C11F0);
		Vulkan::Device& gpu(FindService<Vulkan>()->GetPrimaryDevice());
		Frame&          frame(_frames[0]);

		executor.AwaitCondition(frame.IsConsumed(gpu));
		frame.RecordCommands(gpu, *_scene);
		frame.SubmitCommands(gpu);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
