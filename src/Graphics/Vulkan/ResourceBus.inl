/*==================================================================*\
  ResourceBus.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint ETForceInlineHint void ResourceBus::BindRegion(VkImage target, const VkSparseImageMemoryBind& bind) {
		_sparseBinds.imageBinds.Emplace(target, bind);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResourceBus::BindRegion(VkImage target, const VkSparseMemoryBind& bind) {
		_sparseBinds.imageOpaqueBinds.Emplace(target, bind);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResourceBus::BindRegion(VkBuffer target, const VkSparseMemoryBind& bind) {
		_sparseBinds.bufferBinds.Emplace(target, bind);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const ShaderImage* ResourceBus::Find(const ImageSource& image) const ETNoexceptHint {
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const Mesh* ResourceBus::Find(const MeshSource& mesh) const ETNoexceptHint {
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResourceBus::InsertSignal(uint32 frame, GpuIoPhase phase, VkSemaphore signal) {
		Lock _(_semaphoresMutex);
		_frames[frame].phases[phase].signals.Append(signal);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResourceBus::InsertWait(uint32 frame, GpuIoPhase phase, VkSemaphore wait) {
		Lock _(_semaphoresMutex);
		_frames[frame].phases[phase].waits.Append(wait, VK_PIPELINE_STAGE_TRANSFER_BIT);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResourceBus::EraseSignal(uint32 frame, GpuIoPhase phase, VkSemaphore signal) {
		Lock _(_semaphoresMutex);
		_frames[frame].phases[phase].signals.EraseFirst(signal);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResourceBus::EraseWait(uint32 frame, GpuIoPhase phase, VkSemaphore wait) {
		Lock       _(_semaphoresMutex);
		auto&      waits(_frames[frame].phases[phase].waits);
		const auto candidate(FindIf(waits.Begin(), waits.End(), [wait](Phase::WaitList::Reference i) {
			return eastl::get<VkSemaphore&>(i) == wait;
		}));

		if (ET_UNLIKELY(candidate == waits.End())) {
			return;
		}

		waits.EraseUnordered(candidate);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
