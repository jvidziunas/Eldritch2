/*==================================================================*\
  ChildAllocator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

ETInlineHint ETForceInlineHint ChildAllocator::ChildAllocator(Allocator& allocator, const Utf8Char* const name) ETNoexceptHint : Allocator(name),
																																 _parent(ETAddressOf(allocator)) {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETRestrictHint void* ChildAllocator::Allocate(SizeType sizeInBytes, AllocationDuration duration) {
	return ChildAllocator::Allocate(sizeInBytes, 1u, 0u, duration);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETRestrictHint void* ChildAllocator::Allocate(SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration) {
	return _parent->Allocate(sizeInBytes, alignmentInBytes, offsetInBytes, duration);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint void ChildAllocator::Deallocate(void* const address, SizeType sizeInBytes) {
	_parent->Deallocate(address, sizeInBytes);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Allocator& ChildAllocator::GetParent() const ETNoexceptHint {
	return *_parent;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint void Swap(ChildAllocator& lhs, ChildAllocator& rhs) ETNoexceptHint {
	Swap(lhs._parent, rhs._parent);
}

} // namespace Eldritch2
