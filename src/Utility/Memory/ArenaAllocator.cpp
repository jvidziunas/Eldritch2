/*==================================================================*\
  ArenaAllocator.cpp
  ------------------------------------------------------------------
  Purpose:
  Provides a wrapper around a surrogate 'stack' object for (super)
  fast allocatons and amortized releases. Note that this speed comes
  at a design cost, as the overall size of the allocations must be
  tracked and controlled with a considerably higher degree of
  fidelity, often requiring external allocation size profiles, etc.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <EASTL/memory.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ArenaAllocatorBase::ArenaAllocatorBase( void* pool, const void* const poolEnd, const Utf8Char* const name ) : Allocator( name ), _allocationPointer( static_cast<char*>(pool) ), _arenaEnd( static_cast<const char*>(poolEnd) ) {}

// ---------------------------------------------------

	ArenaAllocatorBase::ArenaAllocatorBase( void* const pool, const SizeType poolCapacityInBytes, const Eldritch2::Utf8Char* const name ) : ArenaAllocatorBase( pool, static_cast<char*>(pool) + poolCapacityInBytes, name ) {}

// ---------------------------------------------------

	ArenaAllocatorBase::ArenaAllocatorBase( ArenaAllocatorBase&& allocator ) : Allocator( eastl::forward<Allocator>( allocator ) ), _allocationPointer( allocator._allocationPointer.load( std::memory_order_relaxed ) ), _arenaEnd( allocator._arenaEnd ) {}

// ---------------------------------------------------

	ETRestrictHint void* ArenaAllocatorBase::Allocate( SizeType sizeInBytes, SizeType alignmentInBytes, SizeType /*offsetInBytes*/, AllocationDuration duration ) {
		size_t	allocationSizeInBytes( sizeInBytes + ( alignmentInBytes - 1u ) );
		void*	allocation( ArenaAllocatorBase::Allocate( allocationSizeInBytes, duration ) );

		if( !allocation ) {
			return nullptr;
		}

		return eastl::align( alignmentInBytes, sizeInBytes, allocation, allocationSizeInBytes );
	}

// ---------------------------------------------------

	ETRestrictHint void* ArenaAllocatorBase::Allocate( SizeType sizeInBytes, AllocationDuration /*duration*/ ) {
		char* const	newMemory( _allocationPointer.fetch_add( sizeInBytes, std::memory_order_consume ) );

		return ( newMemory + sizeInBytes <= _arenaEnd ) ? newMemory : nullptr;
	}

// ---------------------------------------------------

	void ArenaAllocatorBase::Deallocate( void* const /*address*/, SizeType /*sizeInBytes*/ ) {}

// ---------------------------------------------------

	ArenaAllocatorBase::Checkpoint ArenaAllocatorBase::CreateCheckpoint() const {
		return _allocationPointer;
	}

// ---------------------------------------------------

	void ArenaAllocatorBase::RestoreCheckpoint( const Checkpoint checkpoint ) {
	//	Verify the checkpoint is legal.
		ETRuntimeAssert( static_cast<const char*>(checkpoint) <= _allocationPointer.load( std::memory_order_relaxed ) );

		_allocationPointer.store( static_cast<char*>(const_cast<void*>(checkpoint)), std::memory_order_release );
	}

// ---------------------------------------------------

	ExternalArenaAllocator::ExternalArenaAllocator( void* const pool, const void* const poolEnd, const Utf8Char* const name ) : ArenaAllocatorBase( pool, poolEnd, name ) {}

// ---------------------------------------------------

	ExternalArenaAllocator::ExternalArenaAllocator( void* const pool, SizeType poolCapacityInBytes, const Utf8Char* const name ) : ExternalArenaAllocator( pool, static_cast<char*>(pool)+poolCapacityInBytes, name ) {}

// ---------------------------------------------------

	ExternalArenaAllocator::ExternalArenaAllocator( ExternalArenaAllocator&& allocator ) : ArenaAllocatorBase( eastl::forward<ArenaAllocatorBase>( allocator ) ) {}

// ---------------------------------------------------

	ArenaChildAllocator::ArenaChildAllocator( Allocator& parent, SizeType allocationLimitInBytes, AllocationDuration duration, const Utf8Char* const name ) : ArenaAllocatorBase( parent.Allocate( allocationLimitInBytes, 16u, 0u, duration ), allocationLimitInBytes, name ), _parent( parent ), _allocation( _allocationPointer ) {}

// ---------------------------------------------------
	
	ArenaChildAllocator::ArenaChildAllocator( ArenaChildAllocator&& allocator ) : ArenaAllocatorBase( eastl::forward<ArenaAllocatorBase>( allocator ) ), _allocation( allocator._allocation ), _parent( allocator._parent ) {
		allocator._allocation = nullptr;
	}

// ---------------------------------------------------

	ArenaChildAllocator::~ArenaChildAllocator() {
		if( _allocation ) {
			_parent.Deallocate( _allocation, (_arenaEnd - _allocation) );
		}
	}

}	// namespace Eldritch2