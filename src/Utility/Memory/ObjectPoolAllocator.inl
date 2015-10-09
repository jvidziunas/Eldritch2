/*==================================================================*\
  ObjectPoolAllocator<Object>.cpp
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Object>
	ObjectPoolAllocator<Object>::ObjectPoolAllocator( const UTF8Char* const name, const SizeType pageSizeInObjects, Allocator& hostingAllocator ) : Allocator( name ), _hostingAllocator( hostingAllocator ), _objectsPerPage( pageSizeInObjects ) {
		if( void* const newPage = hostingAllocator.Allocate( sizeof(Object) * pageSizeInObjects, alignof(Object), AllocationOption::PERMANENT_ALLOCATION ) ) {
			this->add_ordered_block( newPage, sizeof(Object) * pageSizeInObjects, sizeof(Object) );
		}
	}

// ---------------------------------------------------

	template <typename Object>
	ObjectPoolAllocator<Object>::~ObjectPoolAllocator() {
		// Free all allocated pages.
	}

// ---------------------------------------------------

	template <typename Object>
	ETRestrictHint void* ObjectPoolAllocator<Object>::Allocate( const SizeType sizeInBytes, const AllocationOptions /*options*/) {
		ETRuntimeAssert( sizeInBytes <= sizeof(Object) );

		if( !this->first ) {
			const auto	allocationSize( sizeof(Object) * _objectsPerPage );

			if( void* const newPage = _hostingAllocator.Allocate( allocationSize, alignof(Object), AllocationOption::PERMANENT_ALLOCATION ) ) {
				this->add_ordered_block( newPage, allocationSize, sizeof(Object) );
			}
		}

		return this->malloc();
	}

// ---------------------------------------------------

	template <typename Object>
	ETRestrictHint void* ObjectPoolAllocator<Object>::Allocate( const SizeType sizeInBytes, const SizeType /*alignmentInBytes*/, const AllocationOptions options ) {
		return ObjectPoolAllocator<Object>::Allocate( sizeInBytes, options );
	}

// ---------------------------------------------------

	template <typename Object>
	ETRestrictHint void* ObjectPoolAllocator<Object>::Reallocate( void* const address, const SizeType sizeInBytes, const ReallocationOptions options ) {
		if( sizeInBytes > sizeof(Object) ) {
			return nullptr;
		}
		
		return address ? address : ObjectPoolAllocator::Allocate( sizeInBytes, options );
	}

// ---------------------------------------------------

	template <typename Object>
	ETRestrictHint void* ObjectPoolAllocator<Object>::Reallocate( void* const address, const SizeType sizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) {
		if( sizeInBytes > sizeof(Object) ) {
			return nullptr;
		}

		return address ? address : ObjectPoolAllocator::Allocate( sizeInBytes, alignmentInBytes, options );
	}

// ---------------------------------------------------

	template <typename Object>
	void ObjectPoolAllocator<Object>::Deallocate( void* const address ) {
		return this->free( address );
	}

// ---------------------------------------------------

	template <typename Object>
	void ObjectPoolAllocator<Object>::Deallocate( void* const address, const AlignedDeallocationSemantics ) {
		return ObjectPoolAllocator<Object>::Deallocate( address );
	}

// ---------------------------------------------------

	template <typename Object>
	typename ObjectPoolAllocator<Object>::SizeType ObjectPoolAllocator<Object>::EstimateActualAllocationSizeInBytes( const SizeType allocationSizeInBytes, const SizeType /*alignmentInBytes*/ ) const {
		return allocationSizeInBytes;
	}

}	// namespace Eldritch2Detail