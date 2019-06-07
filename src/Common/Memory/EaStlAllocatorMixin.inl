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
ETInlineHint ETForceInlineHint EaStlAllocatorMixin<EldritchAllocator>::EaStlAllocatorMixin(Arguments&&... arguments) ETNoexceptHintIf(IsNoThrowConstructible<PublicType, Arguments&&...>()) :
	PublicType(Forward<Arguments>(arguments)...) {}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint ETRestrictHint void* EaStlAllocatorMixin<EldritchAllocator>::allocate(size_t byteSize, size_t byteAlignment, size_t byteOffset, int /*flags*/) {
	return PublicType::Allocate(byteSize, byteAlignment, byteOffset);
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint ETRestrictHint void* EaStlAllocatorMixin<EldritchAllocator>::allocate(size_t byteSize, int /*flags*/) {
	return PublicType::Allocate(byteSize);
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint void EaStlAllocatorMixin<EldritchAllocator>::deallocate(void* pointer, size_t byteSize) {
	PublicType::Deallocate(pointer, byteSize);
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint const char* EaStlAllocatorMixin<EldritchAllocator>::get_name() const ETNoexceptHint {
	return PublicType::GetName();
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint void EaStlAllocatorMixin<EldritchAllocator>::set_name(const char* /*name*/) ETNoexceptHint {
	//	Eldritch allocators do not support renaming.
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint bool operator==(const EaStlAllocatorMixin<EldritchAllocator>& lhs, const EaStlAllocatorMixin<EldritchAllocator>& rhs) ETNoexceptHint {
	return static_cast<const typename EaStlAllocatorMixin<EldritchAllocator>::PublicType&>(lhs) == static_cast<const typename EaStlAllocatorMixin<EldritchAllocator>::PublicType&>(rhs);
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint bool operator!=(const EaStlAllocatorMixin<EldritchAllocator>& lhs, const EaStlAllocatorMixin<EldritchAllocator>& rhs) ETNoexceptHint {
	return !(lhs == rhs);
}

// ---------------------------------------------------

template <typename EldritchAllocator>
ETInlineHint ETForceInlineHint void Swap(EaStlAllocatorMixin<EldritchAllocator>& lhs, EaStlAllocatorMixin<EldritchAllocator>& rhs) ETNoexceptHint {
	using namespace ::Eldritch2;

	Swap(static_cast<typename EaStlAllocatorMixin<EldritchAllocator>::PublicType&>(lhs), static_cast<typename EaStlAllocatorMixin<EldritchAllocator>::PublicType&>(rhs));
}

} // namespace Eldritch2

namespace eastl {

template <typename T>
ETInlineHint ETForceInlineHint void swap(Eldritch2::EaStlAllocatorMixin<T>& lhs, Eldritch2::EaStlAllocatorMixin<T>& rhs) {
	Eldritch2::Swap(lhs, rhs);
}

} // namespace eastl
