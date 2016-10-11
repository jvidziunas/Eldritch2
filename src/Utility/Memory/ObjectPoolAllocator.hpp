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
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
#include <Utility/Containers/IntrusiveMPSCQueue.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#include <boost/pool/simple_segregated_storage.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	class ObjectPoolAllocator : public Eldritch2::ChildAllocator, boost::simple_segregated_storage<Eldritch2::ChildAllocator::SizeType> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ObjectPoolAllocator instance.
		ObjectPoolAllocator( const Eldritch2::Utf8Char* const name, SizeType pageSizeInObjects, SizeType objectSizeInBytes, Eldritch2::Allocator& hostingAllocator );

		~ObjectPoolAllocator();

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

		ETRestrictHint void*	Allocate( SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration = AllocationDuration::Normal ) override;
		ETRestrictHint void*	Allocate( SizeType sizeInBytes, AllocationDuration duration = AllocationDuration::Normal ) override;

		void					Deallocate( void* const address, SizeType sizeInBytes ) override;

	// - TYPE PUBLISHING ---------------------------------

	private:
		struct TrackedAllocation : public Eldritch2::IntrusiveForwardListBaseHook {
			ET16ByteAligned char	arena[];
		};

	// - DATA MEMBERS ------------------------------------

	private:
		SizeType											_objectsPerPage;
		SizeType											_elementSizeInBytes;
		Eldritch2::IntrusiveMPSCQueue<TrackedAllocation>	_allocations;
	};

}	// namespace Eldritch2