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

ETConstexpr ETForceInlineHint ChildAllocator::ChildAllocator(Allocator& allocator, const Utf8Char* const name) ETNoexceptHint : Allocator(name), _parent(ETAddressOf(allocator)) {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETRestrictHint void* ChildAllocator::Allocate(SizeType byteSize, SizeType byteAlignment, SizeType byteOffset, AllocationDuration duration) ETNoexceptHint {
	return _parent->Allocate(byteSize, byteAlignment, byteOffset, duration);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETRestrictHint void* ChildAllocator::Allocate(SizeType byteSize, AllocationDuration duration) ETNoexceptHint {
	return this->Allocate(byteSize, /*byteAlignment =*/1u, /*byteOffset =*/0u, duration);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint void ChildAllocator::Deallocate(void* const address, SizeType sizeInBytes) ETNoexceptHint {
	_parent->Deallocate(address, sizeInBytes);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint Allocator& ChildAllocator::GetParent() const ETNoexceptHint {
	return *_parent;
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint bool operator==(ChildAllocator& lhs, ChildAllocator& rhs) ETNoexceptHint {
	return ETAddressOf(lhs.GetParent()) == ETAddressOf(rhs.GetParent());
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint void Swap(ChildAllocator& lhs, ChildAllocator& rhs) ETNoexceptHint {
	Swap(lhs._parent, rhs._parent);
}

} // namespace Eldritch2
