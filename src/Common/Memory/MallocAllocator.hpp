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
#include <Common/Memory/Allocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

class MallocAllocator : public Allocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref MallocAllocator instance.
	MallocAllocator(const Utf8Char* name = "<unnamed allocator>");
	//!	Constructs this @ref MallocAllocator instance.
	MallocAllocator(const MallocAllocator&);

	~MallocAllocator() = default;

	// ---------------------------------------------------

public:
	//! Allocates a contiguous chunk of memory with the specified length and alignment using the passed-in allocation behavior duration.
	ETRestrictHint void* Allocate(SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration = AllocationDuration::Normal) override;
	//! Allocates a contiguous chunk of memory with the specified length using the passed-in allocation behavior duration.
	ETRestrictHint void* Allocate(SizeType sizeInBytes, AllocationDuration duration = AllocationDuration::Normal) override;

	//! Releases a chunk of memory previously allocated by @ref Allocate().
	void Deallocate(void* const address, SizeType sizeInBytes) override;

	// ---------------------------------------------------

public:
	MallocAllocator& operator=(const MallocAllocator&);

	// ---------------------------------------------------

	friend void Swap(MallocAllocator&, MallocAllocator&);
};

} // namespace Eldritch2
