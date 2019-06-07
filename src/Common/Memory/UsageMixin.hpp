/*==================================================================*\
  UsageMixin.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/Allocator.hpp>
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#include <EASTL/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <class BackingAllocator>
class UsageMixin : public BackingAllocator {
	// - TYPE PUBLISHING ---------------------------------

public:
	using SizeType = typename BackingAllocator::SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	//!	Constructs this @ref TrackingAllocatorMixin instance.
	template <typename... Arguments>
	UsageMixin(Arguments&&... args) ETNoexceptHint;
	//!	Constructs this @ref TrackingAllocatorMixin instance.
	template <class = eastl::enable_if<eastl::is_copy_constructible_v<BackingAllocator>>>
	UsageMixin(const UsageMixin&) ETNoexceptHint;

	~UsageMixin() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

public:
	ETRestrictHint void* Allocate(SizeType byteSize, SizeType byteAlignment, SizeType byteOffset, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint override;
	ETRestrictHint void* Allocate(SizeType byteSize, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint override;

	void Deallocate(void* const address, SizeType byteSize) ETNoexceptHint override;

	// ---------------------------------------------------

public:
	SizeType GetPeakUsageInBytes() const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	//!	Amount of memory currently being used from this allocator, specified in bytes.
	/*!	@remarks Updated during each call to @ref Allocate(). */
	Atomic<SizeType> _usage;
	//!	Peak amount of memory used from this allocator, specified in bytes.
	/*!	@remarks Updated during each call to @ref Allocate(). */
	Atomic<SizeType> _peakUsage;

	// ---------------------------------------------------

	template <typename BackingAllocator2>
	friend void Swap(UsageMixin<BackingAllocator2>&, UsageMixin<BackingAllocator2>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/UsageMixin.inl>
//------------------------------------------------------------------//