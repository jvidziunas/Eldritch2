/*==================================================================*\
  UsageMixin.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <class BackingAllocator>
template <class /*SFINAE*/>
ETInlineHint UsageMixin<BackingAllocator>::UsageMixin(const UsageMixin<BackingAllocator>& other) :
	BackingAllocator(other),
	_usage(0u),
	_peakUsage(0u) {}

// ---------------------------------------------------

template <class BackingAllocator>
template <typename... Arguments>
ETInlineHint UsageMixin<BackingAllocator>::UsageMixin(Arguments&&... args) :
	BackingAllocator(eastl::forward<Arguments>(args)...),
	_usage(0u),
	_peakUsage(0u) {}

// ---------------------------------------------------

template <class BackingAllocator>
ETInlineHint ETRestrictHint void* UsageMixin<BackingAllocator>::Allocate(typename BackingAllocator::SizeType sizeInBytes, typename BackingAllocator::SizeType alignmentInBytes, typename BackingAllocator::SizeType offsetInBytes, AllocationDuration duration) {
	void* const result(BackingAllocator::Allocate(sizeInBytes, alignmentInBytes, offsetInBytes, duration));

	if (!result) {
		return nullptr;
	}

	//	Atomic maximum operation.
	SizeType   peakSize;
	const auto effectiveSize(_usage.fetch_add(sizeInBytes, std::memory_order_relaxed));

	do {
		peakSize = _peakUsage.load(std::memory_order_relaxed);
	} while (peakSize < effectiveSize && _usage.compare_exchange_weak(peakSize, effectiveSize, std::memory_order_relaxed));

	return result;
}

// ---------------------------------------------------

template <class BackingAllocator>
ETInlineHint ETRestrictHint void* UsageMixin<BackingAllocator>::Allocate(typename BackingAllocator::SizeType sizeInBytes, AllocationDuration duration) {
	void* const result(BackingAllocator::Allocate(sizeInBytes, duration));

	if (!result) {
		return nullptr;
	}

	//	Atomic maximum operation.
	SizeType   peakSize;
	const auto effectiveSize(_usage.fetch_add(sizeInBytes, std::memory_order_relaxed));

	do {
		peakSize = _peakUsage.load(std::memory_order_relaxed);
	} while (peakSize < effectiveSize && _usage.compare_exchange_weak(peakSize, effectiveSize, std::memory_order_relaxed));

	return result;
}

// ---------------------------------------------------

template <class BackingAllocator>
ETInlineHint void UsageMixin<BackingAllocator>::Deallocate(void* const address, typename BackingAllocator::SizeType sizeInBytes) {
	_usage.fetch_sub(sizeInBytes, std::memory_order_release);
	BackingAllocator::Deallocate(address, sizeInBytes);
}

// ---------------------------------------------------

template <class BackingAllocator>
ETInlineHint typename BackingAllocator::SizeType UsageMixin<BackingAllocator>::GetPeakUsageInBytes() const {
	return _peakUsage.load(std::memory_order_consume);
}

// ---------------------------------------------------

template <class BackingAllocator>
ETInlineHint void UsageMixin<BackingAllocator>::Swap(UsageMixin<BackingAllocator>& allocator) {
	//	Forward to the backing allocator.
	BackingAllocator::Swap(allocator);
	//	Exchange the current and peak allocated amounts on the allocators.
	_usage.store(_usage.exchange(_usage.load(std::memory_order_relaxed), std::memory_order_consume), std::memory_order_release);
	_peakUsage.store(_peakUsage.exchange(_peakUsage.load(std::memory_order_relaxed), std::memory_order_consume), std::memory_order_release);
}

} // namespace Eldritch2
