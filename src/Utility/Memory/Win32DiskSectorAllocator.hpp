/*==================================================================*\
  Win32DiskSectorAllocator.hpp
  ------------------------------------------------------------------
  Purpose:
  

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

	class Win32DiskSectorAllocator : public ::Eldritch2::Allocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this Win32DiskDectorAllocator instance.
		Win32DiskSectorAllocator( const SizeType allocationLimitInBytes = ~static_cast<SizeType>( 0 ), const ::Eldritch2::UTF8Char* const name = UTF8L( "<Unnamed Win32 Disk Sector Allocator>" ) );

		// Destroys this Win32DiskSectorAllocator instance.
		~Win32DiskSectorAllocator();

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

		// Allocates a contiguous chunk of memory with the specified length using the passed-in allocation behavior options.
		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const AllocationOptions options = AllocationOption::DEFAULT ) override sealed;
		// Allocates a contiguous chunk of memory with the specified length and alignment using the passed-in allocation behavior options.
		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const SizeType alignmentInBytes, const AllocationOptions options = AllocationOption::DEFAULT ) override sealed;
		
		// Given a chunk of memory, attempts to expand or separately allocate a new chunk with size greater than or equal to the requested size.
		// The contents of the source memory are then copied over into the new region, if appropriate.
		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const ReallocationOptions options = ReallocationOption::DEFAULT ) override sealed;
		// Given a chunk of memory, attempts to expand or separately allocate a new chunk with size greater than or equal to the requested size.
		// The contents of the source memory are then copied over into the new region, if appropriate.
		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options = ReallocationOption::DEFAULT ) override sealed;
		
		// Releases a chunk of memory previously allocated by Allocate() or Reallocate() WITHOUT aligned semantics.
		void					Deallocate( void* const address ) override sealed;

		void					Deallocate( void* const address, const AlignedDeallocationSemantics ) override sealed;

	// - ALLOCATION DESCRIPTION --------------------------

		// Provides an (accurate) estimate for the amount of space that will actually be requested for accomodating an aligned
		// allocation request.
		SizeType	EstimateActualAllocationSizeInBytes( const SizeType allocationSizeInBytes, const SizeType alignmentInBytes ) const override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		const SizeType	_allocationGranularityInBytes;
		const SizeType	_largePageMinimumSizeInBytes;
	};

}	// namespace Eldritch2
