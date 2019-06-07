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
#include <Common/Memory/Allocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

class NullAllocator : public Allocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

protected:
	//!	Constructs this @ref NullAllocator instance.
	NullAllocator(const NullAllocator&) ETNoexceptHint = default;
	//!	Constructs this @ref NullAllocator instance.
	NullAllocator() ETNoexceptHint;

	~NullAllocator() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

public:
	ETRestrictHint void* Allocate(SizeType byteSize, SizeType byteAlignment, SizeType byteOffset, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint override sealed;
	ETRestrictHint void* Allocate(SizeType byteSize, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint override sealed;

	void Deallocate(void* const address, SizeType byteSize) ETNoexceptHint override sealed;

	// ---------------------------------------------------

public:
	static NullAllocator& GetInstance() ETNoexceptHint;
};

} // namespace Eldritch2
