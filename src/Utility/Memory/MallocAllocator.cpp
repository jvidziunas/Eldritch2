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

using namespace ::Eldritch2;

namespace Eldritch2 {

	MallocAllocator::MallocAllocator( const UTF8Char* name ) : Allocator( name ) {}

// ---------------------------------------------------

	ETRestrictHint void* MallocAllocator::Allocate( const SizeType sizeInBytes, const AllocationOptions /*options*/ ) {
		return malloc( sizeInBytes );
	}

// ---------------------------------------------------

	ETRestrictHint void* MallocAllocator::Reallocate( void* const address, const SizeType sizeInBytes, const ReallocationOptions options ) {
		if( ReallocationOption::FailOnMove == (options & ReallocationOption::FailOnMove) ) {
			return nullptr;
		}

		return realloc( address, sizeInBytes );
	}

// ---------------------------------------------------

	ETRestrictHint void* MallocAllocator::Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) {
		if( ReallocationOption::FailOnMove == (options & ReallocationOption::FailOnMove) ) {
			return nullptr;
		}

		SizeType	allocationSize( EstimateActualAllocationSizeInBytes( newSizeInBytes, alignmentInBytes ) );

		if( const auto allocation = realloc( GetAllocationPointerFromAlignedUserPointer( address ), allocationSize ) ) {
			void*	userPointer( static_cast<void**>(allocation) + 1 );

			if( ::std::align( alignmentInBytes, newSizeInBytes, userPointer, allocationSize ) ) {
				// Store the 'real' pointer that will be internally fed back to the allocator just before what the user sees.
				static_cast<void**>(userPointer)[-1] = allocation;

				return userPointer;
			}
		}

		return nullptr;
	}

// ---------------------------------------------------

	void MallocAllocator::Deallocate( void* const address ) {
		free( address );
	}

}	// namespace Eldritch2
