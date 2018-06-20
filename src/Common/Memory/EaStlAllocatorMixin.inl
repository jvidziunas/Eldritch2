/*==================================================================*\
  EaStlAllocatorAdapterMixin.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <eastl/utility.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename EldritchAllocator>
template <typename... Arguments>
ETInlineHint EaStlAllocatorMixin<EldritchAllocator>::EaStlAllocatorMixin(Arguments&&... arguments) noexcept :
	EldritchAllocator(eastl::forward<Arguments>(arguments)...) {}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETRestrictHint void* EaStlAllocatorMixin<EldritchAllocator>::allocate(size_t allocationSizeInBytes, size_t alignmentInBytes, size_t offsetInBytes, int /*flags*/) {
	return EldritchAllocator::Allocate(allocationSizeInBytes, alignmentInBytes, offsetInBytes);
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETRestrictHint void* EaStlAllocatorMixin<EldritchAllocator>::allocate(size_t allocationSizeInBytes, int /*flags*/) {
	return EldritchAllocator::Allocate(allocationSizeInBytes);
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint void EaStlAllocatorMixin<EldritchAllocator>::deallocate(void* pointer, size_t allocationSizeInBytes) {
	EldritchAllocator::Deallocate(pointer, allocationSizeInBytes);
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint const char* EaStlAllocatorMixin<EldritchAllocator>::get_name() const {
	return EldritchAllocator::GetName();
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint void EaStlAllocatorMixin<EldritchAllocator>::set_name(const char* /*name*/) {
	//	Eldritch allocators do not support renaming.
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint bool operator==(const EaStlAllocatorMixin<EldritchAllocator>& /*operand0*/, const EaStlAllocatorMixin<EldritchAllocator>& /*operand1*/) {
	return false;
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint bool operator!=(const EaStlAllocatorMixin<EldritchAllocator>& /*operand0*/, const EaStlAllocatorMixin<EldritchAllocator>& /*operand1*/) {
	return true;
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint void Swap(EaStlAllocatorMixin<EldritchAllocator>& allocator0, EaStlAllocatorMixin<EldritchAllocator>& allocator1) {
	Swap(static_cast<EldritchAllocator&>(allocator0), static_cast<EldritchAllocator&>(allocator1));
}

} // namespace Eldritch2

namespace eastl {

template <typename T>
ETInlineHint void swap(::Eldritch2::EaStlAllocatorMixin<T>& a, ::Eldritch2::EaStlAllocatorMixin<T>& b) {
	::Eldritch2::Swap(a, b);
}

} // namespace eastl
