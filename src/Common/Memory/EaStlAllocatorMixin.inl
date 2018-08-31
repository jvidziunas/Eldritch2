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
ETInlineHint ETForceInlineHint EaStlAllocatorMixin<EldritchAllocator>::EaStlAllocatorMixin(Arguments&&... arguments) noexcept :
	EldritchAllocator(eastl::forward<Arguments>(arguments)...) {}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint ETRestrictHint void* EaStlAllocatorMixin<EldritchAllocator>::allocate(size_t sizeInBytes, size_t alignmentInBytes, size_t offsetInBytes, int /*flags*/) {
	return EldritchAllocator::Allocate(sizeInBytes, alignmentInBytes, offsetInBytes);
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint ETRestrictHint void* EaStlAllocatorMixin<EldritchAllocator>::allocate(size_t sizeInBytes, int /*flags*/) {
	return EldritchAllocator::Allocate(sizeInBytes);
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint void EaStlAllocatorMixin<EldritchAllocator>::deallocate(void* pointer, size_t sizeInBytes) {
	EldritchAllocator::Deallocate(pointer, sizeInBytes);
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint const char* EaStlAllocatorMixin<EldritchAllocator>::get_name() const {
	return EldritchAllocator::GetName();
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint void EaStlAllocatorMixin<EldritchAllocator>::set_name(const char* /*name*/) {
	//	Eldritch allocators do not support renaming.
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint bool operator==(const EaStlAllocatorMixin<EldritchAllocator>& /*lhs*/, const EaStlAllocatorMixin<EldritchAllocator>& /*rhs*/) {
	return false;
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint bool operator!=(const EaStlAllocatorMixin<EldritchAllocator>& /*lhs*/, const EaStlAllocatorMixin<EldritchAllocator>& /*rhs*/) {
	return true;
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint void Swap(EaStlAllocatorMixin<EldritchAllocator>& lhs, EaStlAllocatorMixin<EldritchAllocator>& rhs) {
	Swap(static_cast<EldritchAllocator&>(lhs), static_cast<EldritchAllocator&>(rhs));
}

} // namespace Eldritch2

namespace eastl {

template <typename T>
ETInlineHint ETForceInlineHint void swap(::Eldritch2::EaStlAllocatorMixin<T>& a, ::Eldritch2::EaStlAllocatorMixin<T>& b) {
	using ::Eldritch2::Swap;
	Swap(a, b);
}

} // namespace eastl
