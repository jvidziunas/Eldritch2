/*==================================================================*\
  NullAllocator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/NullAllocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	NullAllocator::NullAllocator() : Allocator( "<Null Allocator>" ) {}

// ---------------------------------------------------

	ETRestrictHint void* NullAllocator::Allocate( SizeType /*sizeInBytes*/, SizeType /*alignmentInBytes*/, SizeType /*offsetInBytes*/, AllocationDuration /*duration*/ ) {
		return nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* NullAllocator::Allocate( SizeType /*sizeInBytes*/, AllocationDuration /*duration*/ ) {
		return nullptr;
	}

// ---------------------------------------------------

	void NullAllocator::Deallocate( void* /*address*/, SizeType	/*sizeInBytes*/ ) {}

// ---------------------------------------------------

	NullAllocator& NullAllocator::GetInstance() {
		static	NullAllocator	nullAllocator;

		return nullAllocator;
	}

}	// namespace Eldritch2