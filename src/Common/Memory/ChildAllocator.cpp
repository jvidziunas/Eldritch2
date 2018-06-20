/*==================================================================*\
  ChildAllocator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/ChildAllocator.hpp>
#include <Common/Algorithms.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

ChildAllocator::ChildAllocator(Allocator& allocator, const Utf8Char* const name) :
	Allocator(name),
	_parent(&allocator) {}

// ---------------------------------------------------

ETRestrictHint void* ChildAllocator::Allocate(SizeType sizeInBytes, AllocationDuration duration) {
	return ChildAllocator::Allocate(sizeInBytes, 1u, 0u, duration);
}

// ---------------------------------------------------

ETRestrictHint void* ChildAllocator::Allocate(SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration) {
	return _parent->Allocate(sizeInBytes, alignmentInBytes, offsetInBytes, duration);
}

// ---------------------------------------------------

void ChildAllocator::Deallocate(void* const address, SizeType sizeInBytes) {
	_parent->Deallocate(address, sizeInBytes);
}

// ---------------------------------------------------

ChildAllocator& ChildAllocator::operator=(const ChildAllocator& allocator) {
	_parent = allocator._parent;

	return *this;
}

// ---------------------------------------------------

void Swap(ChildAllocator& allocator0, ChildAllocator& allocator1) {
	Swap(allocator0._parent, allocator1._parent);
}

} // namespace Eldritch2
