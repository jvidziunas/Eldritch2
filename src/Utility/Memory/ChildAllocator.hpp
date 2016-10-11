/*==================================================================*\
  ChildAllocator.hpp
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
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {

	class ChildAllocator : public Eldritch2::Allocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref ChildAllocator instance.
	/*! @param[in] allocator @ref Allocator the @ref ChildAllocator should forward requests to.
		@param[in] name @ref Null-terminated C string containing a human-readable C string describing the purpose for which the @ref ChildAllocator is being created. */
		ChildAllocator( Eldritch2::Allocator& allocator, const Eldritch2::Utf8Char* const name );
	//! Constructs this @ref ChildAllocator instance.
		ChildAllocator( const ChildAllocator& );
	//! Constructs this @ref ChildAllocator instance.
		ChildAllocator();

		~ChildAllocator() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------
		
	public:
		ETRestrictHint void*	Allocate( SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration = AllocationDuration::Normal ) override;
		ETRestrictHint void*	Allocate( SizeType sizeInBytes, AllocationDuration duration = AllocationDuration::Normal ) override;

		void					Deallocate( void* const address, SizeType sizeInBytes ) override;

	// ---------------------------------------------------

	public:
		size_t	GetPeakAllocationAmountInBytes() const;

	// ---------------------------------------------------

	public:
		ChildAllocator&	operator=( const ChildAllocator& );

	// ---------------------------------------------------

	public:
		void	Swap( ChildAllocator& allocator );

	// ---------------------------------------------------

	//!	Gets a reference to the @ref Allocator that owns this @ref ChildAllocator.
	/*!	@returns A reference to the @ref Allocator instance this @ref ChildAllocator forwards allocation requests to. */
		Eldritch2::Allocator&	GetParent() const;

	// - DATA MEMBERS ------------------------------------

	protected:
		Eldritch2::Allocator*	_parent;
		std::atomic<size_t>		_allocatedAmount;
		std::atomic<size_t>		_peakAllocatedAmount;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/ChildAllocator.inl>
//------------------------------------------------------------------//