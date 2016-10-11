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

namespace Eldritch2 {

	class ETPureAbstractHint ArenaAllocatorBase : public Eldritch2::Allocator {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using Checkpoint	= const void*;

	// ---

	public:
		class ScopedCheckpoint {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref ScopedCheckpoint instance.
			ScopedCheckpoint( ArenaAllocatorBase& allocator );
		//!	Disable copying.
			ScopedCheckpoint( const ScopedCheckpoint& ) = delete;

			~ScopedCheckpoint();

		// ---------------------------------------------------

		//!	Disable assignment.
			ScopedCheckpoint&	operator=( const ScopedCheckpoint& ) = delete;

		// - DATA MEMBERS ------------------------------------

		private:
			ArenaAllocatorBase&	_allocator;
			const Checkpoint	_checkpoint;
		};

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

	public:
		ETRestrictHint void*	Allocate( SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration = AllocationDuration::Normal ) override sealed;
		ETRestrictHint void*	Allocate( SizeType sizeInBytes, AllocationDuration duration = AllocationDuration::Normal ) override sealed;

		void					Deallocate( void* const address, SizeType sizeInBytes ) override sealed;

	// ---------------------------------------------------

	public:
		Checkpoint	CreateCheckpoint() const;

		void		RestoreCheckpoint( const Checkpoint checkpoint );

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//! Constructs this @ref ArenaAllocatorBase instance.
		ArenaAllocatorBase( void* const pool, SizeType poolCapacityInBytes, const Eldritch2::Utf8Char* const name );
	//! Constructs this @ref ArenaAllocatorBase instance.
		ArenaAllocatorBase( void* const pool, const void* const poolEnd, const Eldritch2::Utf8Char* const name );
	//! Disable copying.
		ArenaAllocatorBase( const ArenaAllocatorBase& ) = delete;
	//! Constructs this @ref ArenaAllocatorBase instance.
		ArenaAllocatorBase( ArenaAllocatorBase&& );

		~ArenaAllocatorBase() = default;

	// - DATA MEMBERS ------------------------------------

	protected:
		std::atomic<char*>	_allocationPointer;
		const char* const	_arenaEnd;
	};

// ---------------------------------------------------

	class ExternalArenaAllocator : public Eldritch2::ArenaAllocatorBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ExternalArenaAllocator instance.
		ExternalArenaAllocator( void* const pool, SizeType poolCapacityInBytes, const Eldritch2::Utf8Char* const name );
	//!	Constructs this @ref ExternalArenaAllocator instance.
		ExternalArenaAllocator( void* const pool, const void* const poolEnd, const Eldritch2::Utf8Char* const name );
	//!	Disable copying.
		ExternalArenaAllocator( const ExternalArenaAllocator& ) = delete;
	//!	Constructs this @ref ExternalArenaAllocator instance.
		ExternalArenaAllocator( ExternalArenaAllocator&& );

		~ExternalArenaAllocator() = default;

	// ---------------------------------------------------

	//!	Disable assignment.
		ExternalArenaAllocator&	operator=( const ExternalArenaAllocator& ) = delete;
	};

// ---------------------------------------------------

	class ArenaChildAllocator : public Eldritch2::ArenaAllocatorBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ArenaChildAllocator instance.
		ArenaChildAllocator( Eldritch2::Allocator& parent, SizeType allocationLimitInBytes, AllocationDuration duration, const Eldritch2::Utf8Char* const name );
	//!	Disable copying.
		ArenaChildAllocator( const ArenaChildAllocator& ) = delete;
	//!	Constructs this @ref ArenaChildAllocator instance.
		ArenaChildAllocator( ArenaChildAllocator&& );

		~ArenaChildAllocator();

	// ---------------------------------------------------

	//!	Disable assignment.
		ArenaChildAllocator&	operator=( const ArenaChildAllocator& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::Allocator&	_parent;
		char*					_allocation;
	};

// ---------------------------------------------------

	template <size_t arenaSizeInBytes>
	class FixedStackAllocator : public Eldritch2::ArenaAllocatorBase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : SizeType {
			ReservedSizeInBytes = arenaSizeInBytes
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref FixedStackAllocator instance.
		FixedStackAllocator( const Eldritch2::Utf8Char* const name );
	//!	Disable copying.
		FixedStackAllocator( const FixedStackAllocator& ) = delete;

		~FixedStackAllocator() = default;

	// ---------------------------------------------------

	//!	Disable assignment.
		FixedStackAllocator&	operator=( const FixedStackAllocator& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//	Aligned for generic type support.
		ET16ByteAligned char	_arena[arenaSizeInBytes];
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/ArenaAllocator.inl>
//------------------------------------------------------------------//