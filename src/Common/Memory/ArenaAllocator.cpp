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
#include <Common/Memory/ArenaAllocator.hpp>
#include <Common/ErrorCode.hpp>
#include <Common/Assert.hpp>
//------------------------------------------------------------------//
#include <eastl/memory.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	AbstractArenaAllocator::AbstractArenaAllocator( const Utf8Char* const name ) : Allocator( name ), _arena( nullptr ), _arenaEnd( nullptr ) {}

// ---------------------------------------------------

	AbstractArenaAllocator::AbstractArenaAllocator( AbstractArenaAllocator&& allocator ) : Allocator( eastl::forward<Allocator>( allocator ) ), _arena( allocator._arena.load( std::memory_order_relaxed ) ), _arenaEnd( allocator._arenaEnd ) {}

// ---------------------------------------------------

	ETRestrictHint void* AbstractArenaAllocator::Allocate( SizeType sizeInBytes, SizeType alignmentInBytes, SizeType /*offsetInBytes*/, AllocationDuration duration ) {
		size_t	allocationSizeInBytes( sizeInBytes + ( alignmentInBytes - 1u ) );
		void*	allocation( AbstractArenaAllocator::Allocate( allocationSizeInBytes, duration ) );

		return allocation ? eastl::align( alignmentInBytes, sizeInBytes, allocation, allocationSizeInBytes ) : nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* AbstractArenaAllocator::Allocate( SizeType sizeInBytes, AllocationDuration /*duration*/ ) {
		char*	allocation( _arena.load( std::memory_order_consume ) );

		do {
			if (static_cast<size_t>(allocation - _arenaEnd) < sizeInBytes) {
				return nullptr;
			}
		} while (!_arena.compare_exchange_weak( allocation, allocation + sizeInBytes, std::memory_order_release ) );

		return allocation;
	}

// ---------------------------------------------------

	void AbstractArenaAllocator::Deallocate( void* const /*address*/, SizeType /*sizeInBytes*/ ) {
	//	Arena allocators only release memory when expressly destroyed.
	}

// ---------------------------------------------------

	AbstractArenaAllocator::Checkpoint AbstractArenaAllocator::CreateCheckpoint() const {
		return _arena;
	}

// ---------------------------------------------------

	void AbstractArenaAllocator::RestoreCheckpoint( Checkpoint checkpoint ) {
		ET_ASSERT( static_cast<const char*>(checkpoint) <= _arena.load( std::memory_order_relaxed ), "Attempting to restore arena checkpoint for different/invalid allocator!" );

		_arena.store( static_cast<char*>(const_cast<void*>(checkpoint)), std::memory_order_release );
	}

// ---------------------------------------------------

	void AbstractArenaAllocator::Reset( char* arena, const char* end ) {
		_arena		= arena;
		_arenaEnd	= end;
	}

// ---------------------------------------------------

	ExternalArenaAllocator::ExternalArenaAllocator( const Utf8Char* const name ) : AbstractArenaAllocator( name ) {}

// ---------------------------------------------------

	ExternalArenaAllocator::ExternalArenaAllocator( ExternalArenaAllocator&& allocator ) : AbstractArenaAllocator( eastl::forward<AbstractArenaAllocator>( allocator ) ) {}

// ---------------------------------------------------

	ArenaChildAllocator::ArenaChildAllocator( const Utf8Char* const name ) : AbstractArenaAllocator( name ), _parent( nullptr ), _allocation( nullptr ) {}

// ---------------------------------------------------
	
	ArenaChildAllocator::ArenaChildAllocator(
		ArenaChildAllocator&& allocator
	) : AbstractArenaAllocator( eastl::forward<AbstractArenaAllocator>( allocator ) ),
		_allocation( eastl::exchange( allocator._allocation, nullptr ) ),
		_parent( eastl::exchange( allocator._parent, nullptr ) ) {}

// ---------------------------------------------------

	ArenaChildAllocator::~ArenaChildAllocator() {
		FreeResources();
	}

// ---------------------------------------------------

	ErrorCode ArenaChildAllocator::BindResources( Allocator& allocator, SizeType sizeInBytes, AllocationDuration duration ) {
		void*	const allocation( allocator.Allocate( sizeInBytes, 16u, 0u, duration ) );

		if (allocation == nullptr) {
			return Error::OutOfMemory;
		}

		FreeResources();

		AbstractArenaAllocator::Reset( static_cast<char*>(allocation), static_cast<char*>(allocation) + sizeInBytes );

		return Error::None;
	}

// ---------------------------------------------------

	void ArenaChildAllocator::FreeResources() {
		if (_parent) {
			_parent->Deallocate( _allocation, GetEnd() - _allocation );
		}
	}

}	// namespace Eldritch2