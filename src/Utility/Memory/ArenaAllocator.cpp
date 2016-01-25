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
#include <memory>
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {

	ArenaAllocatorBase::ArenaAllocatorBase( void* pool, const void* const poolEnd, const UTF8Char* const name ) : Allocator( name ), _allocationPointer( static_cast<char*>(pool) ), _arenaEnd( static_cast<const char*>(poolEnd) ) {}

// ---------------------------------------------------

	ArenaAllocatorBase::ArenaAllocatorBase( void* const pool, const SizeType poolCapacityInBytes, const ::Eldritch2::UTF8Char* const name ) : ArenaAllocatorBase( pool, static_cast<char*>(pool) + poolCapacityInBytes, name ) {}

// ---------------------------------------------------

	ArenaAllocatorBase::ArenaAllocatorBase( ArenaAllocatorBase&& allocator ) : Allocator( allocator.GetName() ), _allocationPointer( allocator._allocationPointer.load( memory_order_relaxed ) ), _arenaEnd( allocator._arenaEnd ) {}

// ---------------------------------------------------

	ETRestrictHint void* ArenaAllocatorBase::Allocate( const SizeType sizeInBytes, const AllocationOptions /*options*/ ) {
		char* const	newMemory( _allocationPointer.fetch_add( sizeInBytes ) );

		if( (newMemory + sizeInBytes) <= _arenaEnd ) {
			return newMemory;
		}

		return nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* ArenaAllocatorBase::Allocate( const SizeType sizeInBytes, const SizeType alignmentInBytes, const AllocationOptions options ) {
		SizeType	allocationSize( ArenaAllocatorBase::EstimateActualAllocationSizeInBytes( sizeInBytes, alignmentInBytes ) );

		if( void* allocation = Allocate( allocationSize, options ) ) {
			return ::std::align( alignmentInBytes, sizeInBytes, allocation, allocationSize );
		}

		return nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* ArenaAllocatorBase::Reallocate( void* const /*address*/, const SizeType /*newSizeInBytes*/, const ReallocationOptions /*options*/ ) {
		// Reallocation is not supported for arena allocators. This will always fail.
		return nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* ArenaAllocatorBase::Reallocate( void* const /*address*/, const SizeType /*newSizeInBytes*/, const SizeType /*newAlignmentInBytes*/, const ReallocationOptions /*options*/ ) {
		// Reallocation is not supported for arena allocators. This will always fail.
		return nullptr;
	}

// ---------------------------------------------------

	void ArenaAllocatorBase::Deallocate( void* const /*address*/ ) {}

// ---------------------------------------------------

	void ArenaAllocatorBase::Deallocate( void* const /*address*/, const AlignedDeallocationSemanticsTag ) {}

// ---------------------------------------------------

	ArenaAllocatorBase::Checkpoint ArenaAllocatorBase::CreateCheckpoint() const {
		return _allocationPointer;
	}

// ---------------------------------------------------

	void ArenaAllocatorBase::RestoreCheckpoint( const Checkpoint checkpoint ) {
		// Verify the checkpoint is legal.
		ETRuntimeAssert( static_cast<const char*>(checkpoint) <= _allocationPointer.load( memory_order_relaxed ) );

		_allocationPointer = static_cast<char*>(const_cast<void*>(checkpoint));
	}

// ---------------------------------------------------

	ArenaAllocatorBase::SizeType ArenaAllocatorBase::EstimateActualAllocationSizeInBytes( const SizeType allocationSizeInBytes, const SizeType alignmentInBytes ) const {
		return allocationSizeInBytes + (alignmentInBytes - 1u);
	}

// ---------------------------------------------------

	ExternalArenaAllocator::ExternalArenaAllocator( void* const pool, const void* const poolEnd, const UTF8Char* const name ) : ArenaAllocatorBase( pool, poolEnd, name ) {}

// ---------------------------------------------------

	ExternalArenaAllocator::ExternalArenaAllocator( void* const pool, const SizeType poolCapacityInBytes, const UTF8Char* const name ) : ExternalArenaAllocator( pool, static_cast<char*>(pool)+poolCapacityInBytes, name ) {}

// ---------------------------------------------------

	ExternalArenaAllocator::ExternalArenaAllocator( ExternalArenaAllocator&& allocator ) : ArenaAllocatorBase( move( allocator ) ) {}

// ---------------------------------------------------

	ArenaChildAllocator::ArenaChildAllocator( Allocator& parent, const SizeType allocationLimitInBytes, const AllocationOptions allocationOptions, const UTF8Char* const name ) : ArenaAllocatorBase( parent.Allocate( allocationLimitInBytes, 16u, allocationOptions ), allocationLimitInBytes, name ), _parent( parent ), _allocation( _allocationPointer ) {}

// ---------------------------------------------------
	
	ArenaChildAllocator::ArenaChildAllocator( ArenaChildAllocator&& allocator ) : ArenaAllocatorBase( move( allocator ) ), _allocation( allocator._allocation ), _parent( allocator._parent ) {
		allocator._allocation = nullptr;
	}

// ---------------------------------------------------

	ArenaChildAllocator::~ArenaChildAllocator() {
		if( _allocation ) {
			_parent.Deallocate( _allocation, ::Eldritch2::AlignedDeallocationSemantics );
		}
	}

}	// namespace Eldritch2