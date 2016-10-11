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

	ObjectPoolAllocator::ObjectPoolAllocator( const Utf8Char* const name, SizeType pageSizeInObjects, SizeType objectSizeInBytes, Allocator& hostingAllocator ) : ChildAllocator( hostingAllocator, name ),
																																								  _elementSizeInBytes( objectSizeInBytes ),
																																								  _objectsPerPage( pageSizeInObjects ) {}

// ---------------------------------------------------

	ObjectPoolAllocator::~ObjectPoolAllocator() {
		_allocations.ClearAndDispose( [&] ( TrackedAllocation& allocation ) {
			ChildAllocator::Deallocate( &allocation, sizeof(TrackedAllocation) + (_elementSizeInBytes * _objectsPerPage) );
		} );
	}

// ---------------------------------------------------

	ETRestrictHint void* ObjectPoolAllocator::Allocate( SizeType sizeInBytes, SizeType /*alignmentInBytes*/, SizeType offsetInBytes, AllocationDuration duration ) {
		return ObjectPoolAllocator::Allocate( sizeInBytes, duration );
	}

// ---------------------------------------------------

	ETRestrictHint void* ObjectPoolAllocator::Allocate( SizeType sizeInBytes, AllocationDuration /*duration*/ ) {
		ETRuntimeAssert( sizeInBytes <= _elementSizeInBytes );

	// ---

		if( empty() ) {
			const auto						allocationSize( sizeof(TrackedAllocation) + (_elementSizeInBytes * _objectsPerPage) );
			void* ETRestrictPtrHint const	newPage( ChildAllocator::Allocate( allocationSize, AllocationDuration::Normal ) );

			if( !newPage ) {
				return nullptr;
			}

			const auto	trackedAllocation( new(newPage) TrackedAllocation );

		//	Add to the internal list for deletion on allocator destruction.
			_allocations.PushBack( *trackedAllocation );
			boost::simple_segregated_storage<SizeType>::add_ordered_block( trackedAllocation->arena, allocationSize, _elementSizeInBytes );
		}

		return boost::simple_segregated_storage<SizeType>::malloc();
	}

// ---------------------------------------------------

	void ObjectPoolAllocator::Deallocate( void* const address, SizeType sizeInBytes ) {
		ETRuntimeAssert( sizeInBytes <= _elementSizeInBytes );

	// ---

		boost::simple_segregated_storage<SizeType>::free( address );
	}

}	// namespace Eldritch2