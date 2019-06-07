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
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#if ET_COMPILER_IS_MSVC || ET_COMPILER_IS_ICC
#	include <malloc.h>
#elif ET_COMPILER_IS_GCC
#	include <stdlib.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {

ETRestrictHint void* MallocAllocator::Allocate(SizeType byteSize, SizeType byteAlignment, SizeType byteOffset, AllocationDuration duration) ETNoexceptHint {
	if (((byteOffset % byteAlignment) == 0) && (byteAlignment <= 16u)) {
		return Allocate(byteSize, duration);
	}

	return nullptr;
}

// ---------------------------------------------------

ETRestrictHint void* MallocAllocator::Allocate(SizeType byteSize, AllocationDuration /*duration*/) ETNoexceptHint {
	return malloc(byteSize);
}

// ---------------------------------------------------

void MallocAllocator::Deallocate(void* const address, SizeType /*byteSize*/) ETNoexceptHint {
	free(address);
}

} // namespace Eldritch2
