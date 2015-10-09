/*==================================================================*\
  Allocator.hpp
  ------------------------------------------------------------------
  Purpose:
  Definition of a basic allocator object that can be used to manage
  memory. Note that this class uses the slightly different (and
  vastly superior) per-instance Lakos allocator model (see "Towards
  a Better Allocator Model" for details) as opposed to the standard
  per-type C++ model. If compatibility with the old style allocator
  is of concern, see the accompanying wrapper class defined in
  ETStdAllocator.hpp.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	class ETPureAbstractHint Allocator {
	// - TYPE PUBLISHING ---------------------------------

	public:
		typedef size_t					SizeType;
		typedef ::Eldritch2::ptrdiff	DifferenceType;
		typedef ::Eldritch2::uint32		AllocationOptions;
		typedef ::Eldritch2::uint32		ReallocationOptions;

	// ---

		enum AllocationOption : AllocationOptions {
			TEMPORARY_ALLOCATION = 0,
			PERMANENT_ALLOCATION = 1
		};

	// ---

		enum ReallocationOption : ReallocationOptions {
			TEMPORARY_REALLOCATION = AllocationOption::TEMPORARY_ALLOCATION,
			PERMANENT_REALLOCATION = AllocationOption::PERMANENT_ALLOCATION,

			FAIL_IF_MOVE_REQUIRED = 2u
		};

	// ---

		struct AlignedDeallocationSemantics {};

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

		//! Allocates a contiguous chunk of memory with the specified length using the passed-in allocation behavior options.
		virtual ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const AllocationOptions options ) abstract;
		//! Allocates a contiguous chunk of memory with the specified length and alignment using the passed-in allocation behavior options.
		virtual ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const SizeType alignmentInBytes, const AllocationOptions options );

		//! Attempts to expand or separately allocate a new chunk with size greater than or equal to the requested size. The contents of the source memory are then copied over into the new region, if appropriate.
		virtual ETRestrictHint void*	Reallocate( void* const address, const SizeType sizeInBytes, const ReallocationOptions options ) abstract;
		//! Attempts to expand or separately allocate a new chunk with size greater than or equal to the requested size. The contents of the source memory are then copied over into the new region, if appropriate.
		virtual ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) abstract;

		//! Releases a chunk of memory previously allocated by @ref Allocate() or @ref Reallocate().
		virtual void					Deallocate( void* const address ) abstract;
		//! Releases a chunk of memory previously allocated by @ref Allocate() or @ref Reallocate() WITH aligned semantics.
		virtual void					Deallocate( void* const address, const AlignedDeallocationSemantics );

	// ---------------------------------------------------

		//! Given an object known to have been allocated from this @ref Allocator, calls the destructor for the given object and then releases its memory back to the @ref Allocator for later use.
		template <typename Object>
		ETForceInlineHint void	Delete( Object& object );
		//!	Given an object known to have been allocated from this @ref Allocator, calls the destructor for the given object and then releases its memory back to the @ref Allocator for later use.
		template <typename Object>
		ETForceInlineHint void	Delete( Object* const object );
		//!	Given an object known to have been allocated from this @ref Allocator, calls the destructor for the given object and then releases its memory back to the @ref Allocator for later use.
		template <typename Object>
		ETForceInlineHint void	Delete( Object& object, const AlignedDeallocationSemantics );
		//!	Given an object known to have been allocated from this @ref Allocator, calls the destructor for the given object and then releases its memory back to the @ref Allocator for later use.
		template <typename Object>
		ETForceInlineHint void	Delete( Object* const object, const AlignedDeallocationSemantics );

	// - DEBUG INFORMATION -------------------------------

		//! Gets a human-readable string identifying the allocator.
		const ::Eldritch2::UTF8Char*	GetName() const;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref Allocator instance.
		Allocator( const ::Eldritch2::UTF8Char*	const name );
		//! Constructs this @ref Allocator instance.
		Allocator( ::Eldritch2::Allocator&& allocator );
		
		//! Destroys this @ref Allocator instance.
		~Allocator() = default;

	// - ALLOCATION STATISTICS ---------------------------

		static ETNoAliasHint void*	GetAllocationPointerFromAlignedUserPointer( void* userPointer );

		//! Provides a conservative estimate for the amount of space that will actually be requested for accommodating an aligned allocation request.
		virtual SizeType			EstimateActualAllocationSizeInBytes( const SizeType allocationSizeInBytes, const SizeType alignmentInBytes ) const;

	// - DATA MEMBERS ------------------------------------

		const ::Eldritch2::UTF8Char*	_name;

	// ---------------------------------------------------

	private:
		//! Disable assignment.
		::Eldritch2::Allocator&	operator=( const ::Eldritch2::Allocator& ) = delete;
	};

// ---------------------------------------------------

	template <typename ObjectType>
	ETForceInlineHint ETNoAliasHint ObjectType*	AllocateTemporaryArray( ::Eldritch2::Allocator& allocator, size_t arraySizeInObjects );
	template <typename ObjectType>
	ETForceInlineHint ETNoAliasHint ObjectType*	AllocateTemporaryArray( ::Eldritch2::Allocator& allocator, size_t arraySizeInObjects, size_t arrayAlignmentInBytes );

	template <typename ObjectType>
	ETForceInlineHint void						DeleteTemporaryArray( ::Eldritch2::Allocator& allocator, ObjectType* const allocatedArray );
	template <typename ObjectType>
	ETForceInlineHint void						DeleteTemporaryArray( ::Eldritch2::Allocator& allocator, ObjectType* const allocatedArray, ::Eldritch2::Allocator::AlignedDeallocationSemantics );

// ---------------------------------------------------

	extern const Allocator::AlignedDeallocationSemantics	AlignedDeallocationSemantics;

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/Allocator.inl>
//------------------------------------------------------------------//