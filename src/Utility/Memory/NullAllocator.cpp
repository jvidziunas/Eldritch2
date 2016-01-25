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

using namespace ::Eldritch2;

namespace Eldritch2 {

	NullAllocator::NullAllocator() : Allocator( UTF8L("<Null Allocator>" ) ) {}

// ---------------------------------------------------

	ETRestrictHint void* NullAllocator::Allocate( const SizeType /*sizeInBytes*/, const AllocationOptions /*options*/ ) {
		return nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* NullAllocator::Allocate( const SizeType /*sizeInBytes*/, const SizeType /*alignmentInBytes*/, const AllocationOptions /*options*/ ) {
		return nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* NullAllocator::Reallocate( void* const /*address*/, const SizeType /*sizeInBytes*/, const ReallocationOptions /*options*/ ) {
		return nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* NullAllocator::Reallocate( void* const /*address*/, const SizeType /*newSizeInBytes*/, const SizeType /*alignmentInBytes*/, const ReallocationOptions /*options*/ ) {
		return nullptr;
	}

// ---------------------------------------------------

	void NullAllocator::Deallocate( void* /*address*/ ) {}

// ---------------------------------------------------

	void NullAllocator::Deallocate( void* /*address*/, const AlignedDeallocationSemanticsTag ) {}

// ---------------------------------------------------

	NullAllocator::SizeType NullAllocator::EstimateActualAllocationSizeInBytes( const SizeType /*allocationSizeInBytes*/, const SizeType /*alignmentInBytes*/ ) const {
		return 0u;
	}

// ---------------------------------------------------

	NullAllocator& NullAllocator::GetInstance() {
		static	NullAllocator	nullAllocator;

		return nullAllocator;
	}

}	// namespace Eldritch2