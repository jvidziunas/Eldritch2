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

	ETInlineHint ArenaAllocatorBase::ScopedCheckpoint::ScopedCheckpoint( ArenaAllocatorBase& allocator ) : _allocator( allocator ), _checkpoint( allocator.CreateCheckpoint() ) {}

// ---------------------------------------------------

	ETInlineHint ArenaAllocatorBase::ScopedCheckpoint::~ScopedCheckpoint() {
		_allocator.RestoreCheckpoint( _checkpoint );
	}

// ---------------------------------------------------

	template <size_t arenaSizeInBytes>
	FixedStackAllocator<arenaSizeInBytes>::FixedStackAllocator( const Eldritch2::Utf8Char* const name ) : Eldritch2::ArenaAllocatorBase( _arena, sizeof(_arena), name ) {}

}	// namespace Eldritch2