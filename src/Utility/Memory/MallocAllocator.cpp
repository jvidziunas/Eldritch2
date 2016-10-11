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
#include <Utility/Memory/MallocAllocator.hpp>
//------------------------------------------------------------------//
#include <memory>
#if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_INTEL )
#	include <malloc.h>
#elif( ET_COMPILER_IS_GCC )
#	include <stdlib.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {

	MallocAllocator::MallocAllocator( const Utf8Char* name ) : Allocator( name ), _peakAllocatedAmount( 0u ), _allocatedAmount( 0u ) {}

// ---------------------------------------------------

	MallocAllocator::MallocAllocator( const MallocAllocator& other ) : Allocator( other ), _peakAllocatedAmount( 0u ), _allocatedAmount( 0u ) {}

// ---------------------------------------------------

	ETRestrictHint void* MallocAllocator::Allocate( SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration ) {
		if( ((offsetInBytes % alignmentInBytes) == 0) && (alignmentInBytes < 16u) ) {
			return Allocate( sizeInBytes, duration );
		}

		return nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* MallocAllocator::Allocate( SizeType sizeInBytes, AllocationDuration /*duration*/ ) {
		return malloc( sizeInBytes );
	}

// ---------------------------------------------------

	void MallocAllocator::Deallocate( void* const address, SizeType /*sizeInBytes*/ ) {
		free( address );
	}

// ---------------------------------------------------

	MallocAllocator& MallocAllocator::operator=( const MallocAllocator& /*other*/ ) {
		return *this;
	}

// ---------------------------------------------------

	void MallocAllocator::Swap( MallocAllocator& allocator ) {
		_allocatedAmount.store( _allocatedAmount.exchange( _allocatedAmount.load( std::memory_order_relaxed ), std::memory_order_consume ), std::memory_order_release );
		_peakAllocatedAmount.store( _peakAllocatedAmount.exchange( _peakAllocatedAmount.load( std::memory_order_relaxed ), std::memory_order_consume ), std::memory_order_release );
	}

}	// namespace Eldritch2
