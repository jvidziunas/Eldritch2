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

ETInlineHint MallocAllocator::MallocAllocator(const Utf8Char* name) ETNoexceptHint : Allocator(name) {}

// ---------------------------------------------------

ETInlineHint MallocAllocator::MallocAllocator(const MallocAllocator& allocator) ETNoexceptHint : Allocator(allocator) {}

// ---------------------------------------------------

ETInlineHint MallocAllocator& MallocAllocator::operator=(const MallocAllocator& /*allocator*/) ETNoexceptHint {
	return *this;
}

// ---------------------------------------------------

ETInlineHint void Swap(MallocAllocator& /*lhs*/, MallocAllocator& /*rhs*/) ETNoexceptHint {}

} // namespace Eldritch2
