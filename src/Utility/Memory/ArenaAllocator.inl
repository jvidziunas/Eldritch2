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
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <size_t arenaSizeInBytes>
	FixedStackAllocator<arenaSizeInBytes>::FixedStackAllocator( const ::Eldritch2::UTF8Char* const name ) : ::Eldritch2Detail::ArenaAllocatorBase( _arena, sizeof(_arena), name ) {}

// ---------------------------------------------------

	template <size_t arenaSizeInBytes>
	FixedStackAllocator<arenaSizeInBytes>::~FixedStackAllocator() {}

}	// namespace Eldritch2