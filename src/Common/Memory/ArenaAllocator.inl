/*==================================================================*\
  ArenaAllocator.hpp
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
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	ETInlineHint AbstractArenaAllocator::ScopedCheckpoint::ScopedCheckpoint( AbstractArenaAllocator& arena ) : _arena( &arena ), _checkpoint( arena.CreateCheckpoint() ) {}

// ---------------------------------------------------

	ETInlineHint AbstractArenaAllocator::ScopedCheckpoint::~ScopedCheckpoint() {
		_arena->RestoreCheckpoint( _checkpoint );
	}

// ---------------------------------------------------

	ETInlineHint const char* AbstractArenaAllocator::GetCurrent( std::memory_order order ) const {
		return _arena.load( order );
	}

// ---------------------------------------------------

	ETInlineHint const char* AbstractArenaAllocator::GetEnd() const {
		return _arenaEnd;
	}

// ---------------------------------------------------

	template <size_t sizeInBytes>
	ETInlineHint StackAllocator<sizeInBytes>::StackAllocator( const Utf8Char* const name ) : AbstractArenaAllocator( name ) {
		Reset( eastl::begin( _stack ), eastl::end( _stack ) );
	}

}	// namespace Eldritch2