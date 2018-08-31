/*==================================================================*\
  MallocAllocator.cpp
  ------------------------------------------------------------------
  Purpose:
  Definition of an allocator object using the standard C library
  malloc/free functions with special variants for aligned operations
  (preferring the platform's standard implementation, if available)

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/MallocAllocator.hpp>
//------------------------------------------------------------------//
#include <memory>
#if ET_COMPILER_IS_MSVC || ET_COMPILER_IS_ICC
#	include <malloc.h>
#elif ET_COMPILER_IS_GCC
#	include <stdlib.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {

ETRestrictHint void* MallocAllocator::Allocate(SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration) {
	if (((offsetInBytes % alignmentInBytes) == 0) && (alignmentInBytes <= 16u)) {
		return Allocate(sizeInBytes, duration);
	}

	return nullptr;
}

// ---------------------------------------------------

ETRestrictHint void* MallocAllocator::Allocate(SizeType sizeInBytes, AllocationDuration /*duration*/) {
	return malloc(sizeInBytes);
}

// ---------------------------------------------------

void MallocAllocator::Deallocate(void* const address, SizeType /*sizeInBytes*/) {
	free(address);
}

} // namespace Eldritch2
