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
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Concurrency/AtomicOperations.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <memory>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2Detail {

	ArenaAllocatorBase::ArenaAllocatorBase( void* pool, const SizeType allocationLimitInBytes, const UTF8Char* const name ) : Allocator( name ), _arenaPtr( static_cast<char*>(pool) ), _arenaEnd( static_cast<char*>(pool) + allocationLimitInBytes ) {}

// ---------------------------------------------------

	ETRestrictHint void* ArenaAllocatorBase::Allocate( const SizeType sizeInBytes, const AllocationOptions /*options*/ ) {
		char* const	newMemory( reinterpret_cast<char*>(AtomicAdd( reinterpret_cast<uintptr&>(_arenaPtr), sizeInBytes )) );

		if( (newMemory + sizeInBytes) <= _arenaEnd ) {
			return newMemory;
		}

		return nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* ArenaAllocatorBase::Allocate( const SizeType sizeInBytes, const SizeType alignmentInBytes, const AllocationOptions options ) {
		SizeType	allocationSize( ArenaAllocatorBase::EstimateActualAllocationSizeInBytes( sizeInBytes, alignmentInBytes ) );
		void*		allocation( this->Allocate( allocationSize, options ) );

		return allocation ? ::std::align( alignmentInBytes, allocationSize, allocation, allocationSize ) : nullptr;
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

	void ArenaAllocatorBase::Deallocate( void* const /*address*/, const AlignedDeallocationSemantics ) {}

// ---------------------------------------------------

	const void* ArenaAllocatorBase::GetCheckpoint() const {
		return _arenaPtr;
	}

// ---------------------------------------------------

	void ArenaAllocatorBase::RestoreCheckpoint( const void* const checkpoint ) {
		// Verify the checkpoint is legal.
		ETRuntimeAssert( static_cast<const char*>(checkpoint) <= _arenaPtr );

		_arenaPtr = static_cast<char*>(const_cast<void*>(checkpoint));
	}

// ---------------------------------------------------

	ArenaAllocatorBase::SizeType ArenaAllocatorBase::EstimateActualAllocationSizeInBytes( const SizeType allocationSizeInBytes, const SizeType alignmentInBytes ) const {
		return allocationSizeInBytes + (alignmentInBytes - 1u);
	}

}	// namespace Eldritch2Detail

namespace Eldritch2 {

	ExternalArenaAllocator::ExternalArenaAllocator( void* pool, const SizeType allocationLimitInBytes, const UTF8Char* const name ) : ArenaAllocatorBase( pool, allocationLimitInBytes, name ) {}

// ---------------------------------------------------

	ArenaChildAllocator::ArenaChildAllocator( Allocator& parent, const SizeType allocationLimitInBytes, const AllocationOptions allocationOptions, const UTF8Char* const name ) : ArenaAllocatorBase( parent.Allocate( allocationLimitInBytes, 16u, allocationOptions ), allocationLimitInBytes, name ), _parent( parent ), _allocation( _arenaPtr ) {}

// ---------------------------------------------------

	ArenaChildAllocator::~ArenaChildAllocator() {
		_parent.Deallocate( _allocation, ::Eldritch2::AlignedDeallocationSemantics );
	}

}	// namespace Eldritch2