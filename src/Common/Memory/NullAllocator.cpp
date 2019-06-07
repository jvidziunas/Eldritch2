/*==================================================================*\
  NullAllocator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

NullAllocator::NullAllocator() ETNoexceptHint : Allocator("<Null Allocator>") {}

// ---------------------------------------------------

ETRestrictHint void* NullAllocator::Allocate(SizeType /*sizeInBytes*/, SizeType /*alignmentInBytes*/, SizeType /*offsetInBytes*/, AllocationDuration /*duration*/) ETNoexceptHint {
	return nullptr;
}

// ---------------------------------------------------

ETRestrictHint void* NullAllocator::Allocate(SizeType /*sizeInBytes*/, AllocationDuration /*duration*/) ETNoexceptHint {
	return nullptr;
}

// ---------------------------------------------------

void NullAllocator::Deallocate(void* /*address*/, SizeType /*sizeInBytes*/) ETNoexceptHint {}

// ---------------------------------------------------

NullAllocator& NullAllocator::GetInstance() ETNoexceptHint {
	static NullAllocator nullAllocator;
	return nullAllocator;
}

} // namespace Eldritch2
