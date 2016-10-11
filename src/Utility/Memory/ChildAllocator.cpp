/*==================================================================*\
  ChildAllocator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <eastl/utility.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ChildAllocator::ChildAllocator( Allocator& allocator, const Utf8Char* const name ) : Allocator( name ), _parent( &allocator ), _allocatedAmount( 0u ), _peakAllocatedAmount( 0u ) {}

// ---------------------------------------------------

	ChildAllocator::ChildAllocator( const ChildAllocator& allocator ) : Allocator( allocator ), _parent( allocator._parent ), _allocatedAmount( 0u ), _peakAllocatedAmount( 0u ) {}

// ---------------------------------------------------

	ChildAllocator::ChildAllocator() : Allocator( "Anonymous Child Allocator" ) {
	//	Default construction of child allocators is dangerous and is provided because many containers (unfortunately) require a default constructor.
		ETRuntimeAssert( false );
	}

// ---------------------------------------------------
		
	ETRestrictHint void* ChildAllocator::Allocate( SizeType sizeInBytes, AllocationDuration duration ) {
		return ChildAllocator::Allocate( sizeInBytes, 1u, 0u, duration );
	}

// ---------------------------------------------------

	ETRestrictHint void* ChildAllocator::Allocate( SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration ) {
		const auto	result( _parent->Allocate( sizeInBytes, alignmentInBytes, offsetInBytes, duration ) );

		if( result ) {
			const SizeType	effectiveSize( _allocatedAmount.fetch_add( sizeInBytes, std::memory_order_relaxed ) );
			SizeType		peakSize;
			do {
				peakSize = _peakAllocatedAmount.load( std::memory_order_relaxed );
			} while( peakSize < effectiveSize && _allocatedAmount.compare_exchange_weak( peakSize, effectiveSize, std::memory_order_relaxed ) );
		}

		return result;
	}

// ---------------------------------------------------

	void ChildAllocator::Deallocate( void* const address, SizeType sizeInBytes ) {
		_parent->Deallocate( address, sizeInBytes );
	}

// ---------------------------------------------------

	ChildAllocator&	ChildAllocator::operator=( const ChildAllocator& allocator ) {
		ETRuntimeAssert( 0u == _allocatedAmount.load( std::memory_order_relaxed ) );

		_parent = allocator._parent;

		return *this;
	}

// ---------------------------------------------------

	void ChildAllocator::Swap( ChildAllocator& allocator ) {
		_allocatedAmount.store( _allocatedAmount.exchange( _allocatedAmount.load( std::memory_order_relaxed ), std::memory_order_consume ), std::memory_order_release );
		_peakAllocatedAmount.store( _peakAllocatedAmount.exchange( _peakAllocatedAmount.load( std::memory_order_relaxed ), std::memory_order_consume ), std::memory_order_release );

		eastl::swap( _parent, allocator._parent );
	}

}	// namespace Eldritch2