/*==================================================================*\
  MallocAllocator.inl
  ------------------------------------------------------------------
  Purpose:
  Definition of an allocator object using the standard C library
  malloc/free functions with special variants for aligned operations
  (preferring the platform's standard implementation, if available)

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

ETConstexpr ETForceInlineHint MallocAllocator::MallocAllocator(const Utf8Char* name) ETNoexceptHint : Allocator(name) {}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint MallocAllocator& MallocAllocator::operator=(const MallocAllocator& /*allocator*/) ETNoexceptHint {
	return *this;
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint bool operator==(const MallocAllocator&, const MallocAllocator&) ETNoexceptHint {
	// Malloc-based allocators are stateless and always refer to the global heap.
	return true;
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint void Swap(MallocAllocator&, MallocAllocator&) ETNoexceptHint {
	// Malloc-based allocators are stateless and always refer to the global heap.
}

} // namespace Eldritch2
