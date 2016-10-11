/*==================================================================*\
  NullAllocator.hpp
  ------------------------------------------------------------------
  Purpose:
  
  
  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Allocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	class NullAllocator : public Eldritch2::Allocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Constructs this @ref NullAllocator instance.
		NullAllocator( const NullAllocator& ) = delete;
	//!	Constructs this @ref NullAllocator instance.
		NullAllocator();

		~NullAllocator() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

	public:
		ETRestrictHint void*	Allocate( SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration = AllocationDuration::Normal ) override sealed;
		ETRestrictHint void*	Allocate( SizeType sizeInBytes, AllocationDuration duration = AllocationDuration::Normal ) override sealed;

		void					Deallocate( void* const address, SizeType sizeInBytes ) override sealed;

	// ---------------------------------------------------

	public:
		static Eldritch2::NullAllocator&	GetInstance();
	};

}	// namespace Eldritch2