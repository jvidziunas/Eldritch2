/*==================================================================*\
  NullAllocator.hpp
  ------------------------------------------------------------------
  Purpose:
  
  
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

	class NullAllocator : public ::Eldritch2::Allocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref NullAllocator instance.
		NullAllocator();

		//!	Destroys this @ref NullAllocator instance.
		~NullAllocator() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const AllocationOptions options ) override sealed;
		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const SizeType alignmentInBytes, const AllocationOptions options ) override sealed;

		ETRestrictHint void*	Reallocate( void* const address, const SizeType sizeInBytes, const ReallocationOptions options ) override sealed;
		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) override sealed;

		void					Deallocate( void* const address ) override sealed;
		void					Deallocate( void* const address, const AlignedDeallocationSemantics ) override sealed;

	// - ALLOCATION STATISTICS ---------------------------

		SizeType	EstimateActualAllocationSizeInBytes( const SizeType allocationSizeInBytes, const SizeType alignmentInBytes ) const override sealed;

	// ---------------------------------------------------

		static ::Eldritch2::NullAllocator&	GetInstance();
	};

}	// namespace Eldritch2