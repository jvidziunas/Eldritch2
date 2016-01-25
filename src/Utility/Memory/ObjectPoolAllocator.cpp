/*==================================================================*\
  ObjectPoolAllocator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ObjectPoolAllocator.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ObjectPoolAllocator::ObjectPoolAllocator( const UTF8Char* const name, const SizeType pageSizeInObjects, const SizeType objectSizeInBytes, Allocator& hostingAllocator ) : ChildAllocator( hostingAllocator, name ),
																																											  _elementSizeInBytes( objectSizeInBytes ),
																																											  _objectsPerPage( pageSizeInObjects ) {}

// ---------------------------------------------------

	ObjectPoolAllocator::~ObjectPoolAllocator() {
		ChildAllocator&	allocator( *this );

		_allocations.ClearAndDispose( [&allocator] ( TrackedAllocation& allocation ) {
			allocator.Delete( allocation, ::Eldritch2::AlignedDeallocationSemantics );
		} );
	}

// ---------------------------------------------------

	ETRestrictHint void* ObjectPoolAllocator::Allocate( const SizeType sizeInBytes, const AllocationOptions /*options*/ ) {
		ETRuntimeAssert( sizeInBytes <= _elementSizeInBytes );

	// ---

		if( empty() ) {
			const auto	allocationSize( sizeof(TrackedAllocation) + (_elementSizeInBytes * _objectsPerPage) );

			if( void* ETRestrictPtrHint const newPage = ChildAllocator::Allocate( allocationSize, alignof( TrackedAllocation ), AllocationDuration::Normal ) ) {
				const auto	trackedAllocation( new(newPage) TrackedAllocation );

				// Add to the internal list for deletion on allocator destruction.
				_allocations.PushBack( *trackedAllocation );
				add_ordered_block( trackedAllocation->arena, allocationSize, _elementSizeInBytes );
			} else {
				return nullptr;
			}
		}

		return malloc();
	}

// ---------------------------------------------------

	ETRestrictHint void* ObjectPoolAllocator::Allocate( const SizeType sizeInBytes, const SizeType /*alignmentInBytes*/, const AllocationOptions options ) {
		return ObjectPoolAllocator::Allocate( sizeInBytes, options );
	}

// ---------------------------------------------------

	ETRestrictHint void* ObjectPoolAllocator::Reallocate( void* const address, const SizeType sizeInBytes, const ReallocationOptions options ) {
		ETRuntimeAssert( sizeInBytes <= _elementSizeInBytes );

	// ---
		
		return address ? address : ObjectPoolAllocator::Allocate( sizeInBytes, options );
	}

// ---------------------------------------------------

	ETRestrictHint void* ObjectPoolAllocator::Reallocate( void* const address, const SizeType sizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) {
		ETRuntimeAssert( sizeInBytes <= _elementSizeInBytes );

	// ---

		return address ? address : ObjectPoolAllocator::Allocate( sizeInBytes, alignmentInBytes, options );
	}

// ---------------------------------------------------

	void ObjectPoolAllocator::Deallocate( void* const address ) {
		return free( address );
	}

// ---------------------------------------------------

	void ObjectPoolAllocator::Deallocate( void* const address, const AlignedDeallocationSemanticsTag ) {
		return ObjectPoolAllocator::Deallocate( address );
	}

// ---------------------------------------------------

	ObjectPoolAllocator::SizeType ObjectPoolAllocator::EstimateActualAllocationSizeInBytes( const SizeType allocationSizeInBytes, const SizeType /*alignmentInBytes*/ ) const {
		return _elementSizeInBytes;
	}

}	// namespace Eldritch2