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
#if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_INTEL )
#	include <malloc.h>
#elif( ET_COMPILER_IS_GCC )
#	include <stdlib.h>
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2 {

	MallocAllocator::MallocAllocator( const UTF8Char* name ) : Allocator( name ) {}

// ---------------------------------------------------

	MallocAllocator::~MallocAllocator() {}

// ---------------------------------------------------

	ETRestrictHint void* MallocAllocator::Allocate( SizeType sizeInBytes, AllocationOptions ) {
		return malloc( sizeInBytes );
	}

// ---------------------------------------------------

	ETRestrictHint void* MallocAllocator::Reallocate( void* address, SizeType sizeInBytes, ReallocationOptions options ) {
		return nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* MallocAllocator::Reallocate( void* address, SizeType newSizeInBytes, SizeType alignmentInBytes, ReallocationOptions options ) {
		return nullptr;
	}

// ---------------------------------------------------

	void MallocAllocator::Deallocate( void* address ) {
		return free( address );
	}

}	// namespace Eldritch2
