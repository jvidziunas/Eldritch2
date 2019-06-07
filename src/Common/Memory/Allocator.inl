/*==================================================================*\
  Allocator.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <size_t Alignment>
ETInlineHint ETForceInlineHint Allocator::AllocationHeader<Alignment>::AllocationHeader(SizeType userByteSize) ETNoexceptHint : userByteSize(userByteSize) {}

// ---------------------------------------------------

template <size_t Alignment>
ETConstexpr ETForceInlineHint const Allocator::AllocationHeader<Alignment>* Allocator::AllocationHeader<Alignment>::Get(const void* allocation) ETNoexceptHint {
	return allocation ? reinterpret_cast<const AllocationHeader<Alignment>*>(static_cast<const byte*>(allocation) - ETOffsetOf(AllocationHeader<Alignment>, userBytes)) : nullptr;
}

// ---------------------------------------------------

template <size_t Alignment>
ETConstexpr ETForceInlineHint Allocator::AllocationHeader<Alignment>* Allocator::AllocationHeader<Alignment>::Get(void* allocation) ETNoexceptHint {
	return allocation ? reinterpret_cast<AllocationHeader<Alignment>*>(static_cast<byte*>(allocation) - ETOffsetOf(AllocationHeader<Alignment>, userBytes)) : nullptr;
}

// ---------------------------------------------------

template <size_t Alignment>
ETInlineHint ETForceInlineHint ETRestrictHint void* Allocator::AllocationHeader<Alignment>::operator new(size_t thisSize, Allocator& allocator, SizeType userBytes) ETNoexceptHint {
	return allocator.Allocate(thisSize + userBytes, Allocator::SizeType(Alignment), /*byteOffset =*/0u);
}

// ---------------------------------------------------

template <size_t Alignment>
ETInlineHint ETForceInlineHint void Allocator::AllocationHeader<Alignment>::operator delete(void* self, Allocator& allocator, SizeType userBytes)ETNoexceptHint {
	allocator.Deallocate(self, sizeof(AllocationHeader<Alignment>) + userBytes);
}

} // namespace Eldritch2
