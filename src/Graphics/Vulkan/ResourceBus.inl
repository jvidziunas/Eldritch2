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

	ETInlineHint ETForceInlineHint void ResourceBus::Frame::Bind(VkImage target, const VkSparseImageMemoryBind& bind) {
		Lock _(_bindsMutex);
		_sparseBinds.imageBinds[target].Append(bind);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResourceBus::Frame::Bind(VkImage target, const VkSparseMemoryBind& bind) {
		Lock _(_bindsMutex);
		_sparseBinds.imageOpaqueBinds[target].Append(bind);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResourceBus::Frame::Bind(VkBuffer target, const VkSparseMemoryBind& bind) {
		Lock _(_bindsMutex);
		_sparseBinds.bufferBinds[target].Append(bind);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResourceBus::Frame::InsertSignal(GpuIoPhase phase, VkSemaphore signal) {
		Lock _(_semaphoresMutex);
		_phases[phase].signals.Append(signal);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResourceBus::Frame::InsertWait(GpuIoPhase phase, VkSemaphore wait) {
		Lock _(_semaphoresMutex);
		_phases[phase].waits.Append(wait, VK_PIPELINE_STAGE_TRANSFER_BIT);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResourceBus::Frame::EraseSignal(GpuIoPhase phase, VkSemaphore signal) {
		Lock _(_semaphoresMutex);
		_phases[phase].signals.EraseFirst(signal);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResourceBus::Frame::EraseWait(GpuIoPhase phase, VkSemaphore wait) {
		Lock       _(_semaphoresMutex);
		auto&      waits(_phases[phase].waits);
		const auto candidate(FindIf(waits.Begin(), waits.End(), [wait](typename TransferPhase::WaitList::Reference i) { return eastl::get<VkSemaphore&>(i) == wait; }));

		if (ET_UNLIKELY(candidate == waits.End())) {
			return;
		}

		waits.EraseUnordered(candidate);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint auto ResourceBus::GetFrames() const ETNoexceptHint -> const Frame (&)[2] {
		return _frames;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint auto ResourceBus::GetFrames() ETNoexceptHint -> Frame (&)[2] {
		return _frames;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const ShaderImage* ResourceBus::Find(const ImageSource& image) const ETNoexceptHint {
		const auto candidate(_imagesBySource.Find(ETAddressOf(image)));
		return ET_LIKELY(candidate != _imagesBySource.End()) ? ETAddressOf(candidate->second) : nullptr;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const Mesh* ResourceBus::Find(const MeshSource<SkinnedVertex>& mesh) const ETNoexceptHint {
		const auto candidate(_meshesBySource.Find(ETAddressOf(mesh)));
		return ET_LIKELY(candidate != _meshesBySource.End()) ? ETAddressOf(candidate->second) : nullptr;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
