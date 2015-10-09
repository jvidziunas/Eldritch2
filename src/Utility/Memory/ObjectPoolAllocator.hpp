/*==================================================================*\
  ObjectPoolAllocator.hpp
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
#include <boost/pool/simple_segregated_storage.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Object>
	class ObjectPoolAllocator : public ::Eldritch2::Allocator, private ::boost::simple_segregated_storage<::Eldritch2::Allocator::SizeType> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this ObjectPoolAllocator instance.
		ObjectPoolAllocator( const ::Eldritch2::UTF8Char* const name, const SizeType pageSizeInObjects, ::Eldritch2::Allocator& hostingAllocator );

		// Destroys this ObjectPoolAllocator instance.
		~ObjectPoolAllocator();

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

		// Allocates a contiguous chunk of memory with the specified length using the passed-in allocation behavior options.
		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const AllocationOptions options ) override;
		// Allocates a contiguous chunk of memory with the specified length and alignment using the passed-in allocation behavior options.
		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const SizeType alignmentInBytes, const AllocationOptions options ) override;

		// Given a chunk of memory, attempts to expand or separately allocate a new chunk with size greater than or equal to the requested size.
		// The contents of the source memory are then copied over into the new region, if appropriate.
		ETRestrictHint void*	Reallocate( void* const address, const SizeType sizeInBytes, const ReallocationOptions options ) override;
		// Given a chunk of memory, attempts to expand or separately allocate a new chunk with size greater than or equal to the requested size.
		// The contents of the source memory are then copied over into the new region, if appropriate.
		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) override;

		// Releases a chunk of memory previously allocated by Allocate() or Reallocate().
		void					Deallocate( void* const address ) override;
		// Releases a chunk of memory previously allocated by Allocate() or Reallocate() WITH aligned semantics.
		void					Deallocate( void* const address, const AlignedDeallocationSemantics ) override;

		SizeType	EstimateActualAllocationSizeInBytes( const SizeType allocationSizeInBytes, const SizeType alignmentInBytes ) const override;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::Allocator&	_hostingAllocator;
		const SizeType			_objectsPerPage;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/ObjectPoolAllocator.inl>
//------------------------------------------------------------------//