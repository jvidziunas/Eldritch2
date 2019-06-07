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
	ETConstexpr MallocAllocator(const Utf8Char* name = "<unnamed allocator>") ETNoexceptHint;
	//!	Constructs this @ref MallocAllocator instance.
	ETConstexpr MallocAllocator(const MallocAllocator&) ETNoexceptHint = default;

	~MallocAllocator() = default;

	// ---------------------------------------------------

public:
	//! Allocates a contiguous chunk of memory with the specified length and alignment using the passed-in allocation behavior duration.
	ETRestrictHint void* Allocate(SizeType byteSize, SizeType byteAlignment, SizeType byteOffset, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint override;
	//! Allocates a contiguous chunk of memory with the specified length using the passed-in allocation behavior duration.
	ETRestrictHint void* Allocate(SizeType byteSize, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint override;

	//! Releases a chunk of memory previously allocated by @ref Allocate().
	void Deallocate(void* const address, SizeType byteSize) ETNoexceptHint override;

	// ---------------------------------------------------

public:
	ETConstexpr MallocAllocator& operator=(const MallocAllocator&) ETNoexceptHint;

	// ---------------------------------------------------

	friend ETConstexpr bool operator==(const MallocAllocator&, const MallocAllocator&) ETNoexceptHint;

	friend ETConstexpr void Swap(MallocAllocator&, MallocAllocator&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/MallocAllocator.inl>
//------------------------------------------------------------------//
