/*==================================================================*\
  MallocAllocator.hpp
  ------------------------------------------------------------------
  Purpose:
  Definition of an allocator object using the standard C library
  malloc/free functions with special variants for aligned operations
  (preferring the platform's standard implementation, if available)

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
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
		// Constructs this MallocAllocator instance.
		MallocAllocator( const ::Eldritch2::UTF8Char* name );

		// Destroys this MallocAllocator instance.
		~MallocAllocator();

	// ---------------------------------------------------

		ETRestrictHint void*	Allocate( SizeType size, ::Eldritch2::AllocationOptions options = ::Eldritch2::AllocationOption::DEFAULT ) override sealed;

		ETRestrictHint void*	Reallocate( void* address, SizeType sizeInBytes, ::Eldritch2::ReallocationOptions options = ::Eldritch2::ReallocationOption::DEFAULT ) override sealed;

		ETRestrictHint void*	Reallocate( void* address, SizeType newSizeInBytes, SizeType alignmentInBytes, ::Eldritch2::ReallocationOptions options = ::Eldritch2::ReallocationOption::DEFAULT ) override sealed;

	// ---------------------------------------------------

		void	Deallocate( void* address ) override sealed;
	};

}	// namespace Eldritch2
