/*==================================================================*\
  ChildAllocator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/Allocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

class ChildAllocator : public Allocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref ChildAllocator instance.
	/*! @param[in] parent @ref Allocator the @ref ChildAllocator should forward requests to.
		@param[in] name @ref Null-terminated C string containing a human-readable C string describing the purpose for which the @ref ChildAllocator is being created. */
	ChildAllocator(Allocator& parent, const Utf8Char* const name) ETNoexceptHint;
	//! Constructs this @ref ChildAllocator instance.
	ChildAllocator(const ChildAllocator&) ETNoexceptHint = default;

	~ChildAllocator() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

public:
	ETRestrictHint void* Allocate(SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration = AllocationDuration::Normal) override;
	ETRestrictHint void* Allocate(SizeType sizeInBytes, AllocationDuration duration = AllocationDuration::Normal) override;

	void Deallocate(void* const address, SizeType sizeInBytes) override;

	// ---------------------------------------------------

public:
	//!	Gets a reference to the @ref Allocator that owns this @ref ChildAllocator.
	/*!	@returns A reference to the @ref Allocator instance this @ref ChildAllocator forwards allocation requests to. */
	Allocator& GetParent() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	ChildAllocator& operator=(const ChildAllocator&) ETNoexceptHint = default;

	// - DATA MEMBERS ------------------------------------

private:
	Allocator* _parent;

	// ---------------------------------------------------

	friend void Swap(ChildAllocator&, ChildAllocator&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/ChildAllocator.inl>
//------------------------------------------------------------------//
