/*==================================================================*\
  ArenaAllocator.hpp
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

namespace Eldritch2Detail {

	class ETPureAbstractHint ArenaAllocatorBase : public ::Eldritch2::Allocator {
	// - MEMORY ALLOCATION/DEALLOCATION ------------------

	public:
		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const AllocationOptions options ) override sealed;
		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const SizeType alignmentInBytes, const AllocationOptions options ) override sealed;

		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const ReallocationOptions options ) override sealed;
		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) override sealed;

		void					Deallocate( void* const address ) override sealed;
		void					Deallocate( void* const address, const AlignedDeallocationSemantics ) override sealed;

	// ---------------------------------------------------

		const void*	GetCheckpoint() const;

		void		RestoreCheckpoint( const void* const checkpoint );

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref ArenaAllocatorBase instance.
		ArenaAllocatorBase( void* pool, const SizeType allocationLimitInBytes, const ::Eldritch2::UTF8Char* const name );

		//! Destroys this @ref ArenaAllocatorBase instance.
		~ArenaAllocatorBase() = default;

	// ---------------------------------------------------

		SizeType	EstimateActualAllocationSizeInBytes( const SizeType allocationSizeInBytes, const SizeType alignmentInBytes ) const override sealed;

	// - DATA MEMBERS ------------------------------------

		::std::atomic<char*>	_allocationPointer;
		char* const				_arenaEnd;
	};

}	// namespace Eldritch2Detail

namespace Eldritch2 {

	class ExternalArenaAllocator : public ::Eldritch2Detail::ArenaAllocatorBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ExternalArenaAllocator instance.
		ExternalArenaAllocator( void* pool, const Allocator::SizeType allocationLimitInBytes, const ::Eldritch2::UTF8Char* const name );

		//!	Destroys this @ref ExternalArenaAllocator instance.
		~ExternalArenaAllocator() = default;
	};

// ---------------------------------------------------

	class ArenaChildAllocator : public ::Eldritch2Detail::ArenaAllocatorBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ArenaChildAllocator instance.
		ArenaChildAllocator( ::Eldritch2::Allocator& parent, const Allocator::SizeType allocationLimitInBytes, const Allocator::AllocationOptions allocationOptions, const ::Eldritch2::UTF8Char* const name );

		//!	Destroys this @ref ArenaChildAllocator instance.
		~ArenaChildAllocator();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::Allocator&	_parent;
		void* const				_allocation;
	};

// ---------------------------------------------------

	template <size_t arenaSizeInBytes>
	class FixedStackAllocator : public ::Eldritch2Detail::ArenaAllocatorBase {
	public:
		enum : size_t {
			RESERVED_SIZE_IN_BYTES = arenaSizeInBytes
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref FixedStackAllocator instance.
		FixedStackAllocator( const ::Eldritch2::UTF8Char* const name );

		//! Destroys this @ref FixedStackAllocator instance.
		~FixedStackAllocator();

	// - DATA MEMBERS ------------------------------------

	private:
		ET16ByteAligned char	_arena[arenaSizeInBytes];
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/ArenaAllocator.inl>
//------------------------------------------------------------------//