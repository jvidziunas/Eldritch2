/*==================================================================*\
  MallocAllocator.hpp
  ------------------------------------------------------------------
  Purpose:
  Definition of an allocator object using the standard C library
  malloc/free functions with special variants for aligned operations
  (preferring the platform's standard implementation, if available)

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Allocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	class MallocAllocator : public ::Eldritch2::Allocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref MallocAllocator instance.
		MallocAllocator( const ::Eldritch2::UTF8Char* name );

		~MallocAllocator() = default;

	// ---------------------------------------------------

		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const AllocationOptions options ) override sealed;

		//! Attempts to expand or separately allocate a new chunk with size greater than or equal to the requested size. The contents of the source memory are then copied over into the new region, if appropriate.
		ETRestrictHint void*	Reallocate( void* const address, const SizeType sizeInBytes, const ReallocationOptions options ) override sealed;
		//! Attempts to expand or separately allocate a new chunk with size greater than or equal to the requested size. The contents of the source memory are then copied over into the new region, if appropriate.
		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) override sealed;

		//! Releases a chunk of memory previously allocated by @ref Allocate() or @ref Reallocate().
		void					Deallocate( void* const address ) override sealed;
	};

}	// namespace Eldritch2
