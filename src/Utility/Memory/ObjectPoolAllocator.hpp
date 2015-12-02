/*==================================================================*\
  ObjectPoolAllocator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveVyukovMPSCQueue.hpp>
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#include <boost/pool/simple_segregated_storage.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	class ObjectPoolAllocator : public ::Eldritch2::ChildAllocator, ::boost::simple_segregated_storage<ChildAllocator::SizeType> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ObjectPoolAllocator instance.
		ObjectPoolAllocator( const ::Eldritch2::UTF8Char* const name, const SizeType pageSizeInObjects, const SizeType objectSizeInBytes, ::Eldritch2::Allocator& hostingAllocator );

		//!	Destroys this @ref ObjectPoolAllocator instance.
		~ObjectPoolAllocator();

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const AllocationOptions options ) override;
		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const SizeType alignmentInBytes, const AllocationOptions options ) override;

		ETRestrictHint void*	Reallocate( void* const address, const SizeType sizeInBytes, const ReallocationOptions options ) override;
		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) override;

		void					Deallocate( void* const address ) override;
		void					Deallocate( void* const address, const AlignedDeallocationSemantics ) override;

		SizeType				EstimateActualAllocationSizeInBytes( const SizeType allocationSizeInBytes, const SizeType alignmentInBytes ) const override;

	// - TYPE PUBLISHING ---------------------------------

	private:
		struct ET16ByteAligned TrackedAllocation : public ::Eldritch2::IntrusiveForwardListBaseHook {
			ET16ByteAligned char	arena[];
		};

	// - DATA MEMBERS ------------------------------------

		SizeType													_objectsPerPage;
		SizeType													_elementSizeInBytes;
		::Eldritch2::IntrusiveVyukovMPSCQueue<TrackedAllocation>	_allocations;
	};

}	// namespace Eldritch2