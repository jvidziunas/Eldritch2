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
#include <Common/Memory/Allocator.hpp>
#include <Common/Atomic.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
class ErrorCode;
}

namespace Eldritch2 {

class ETPureAbstractHint AbstractArenaAllocator : public Allocator {
	// - TYPE PUBLISHING ---------------------------------

public:
	using Checkpoint = const void*;

	// ---

public:
	class ScopedCheckpoint {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ScopedCheckpoint instance.
		ScopedCheckpoint(AbstractArenaAllocator& arena);
		//!	Disable copy construction.
		ScopedCheckpoint(const ScopedCheckpoint&) = delete;

		~ScopedCheckpoint();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ScopedCheckpoint& operator=(const ScopedCheckpoint&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		AbstractArenaAllocator* _arena;
		Checkpoint              _checkpoint;
	};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

protected:
	//! Constructs this @ref ArenaAllocatorBase instance.
	AbstractArenaAllocator(const Utf8Char* const name);
	//! Disable copy construction.
	AbstractArenaAllocator(const AbstractArenaAllocator&) = delete;
	//! Constructs this @ref ArenaAllocatorBase instance.
	AbstractArenaAllocator(AbstractArenaAllocator&&);

	~AbstractArenaAllocator() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

public:
	ETRestrictHint void* Allocate(SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration = AllocationDuration::Normal) override sealed;
	ETRestrictHint void* Allocate(SizeType sizeInBytes, AllocationDuration duration = AllocationDuration::Normal) override sealed;

	void Deallocate(void* const address, SizeType sizeInBytes) override sealed;

	// ---------------------------------------------------

public:
	const char* GetCurrent(std::memory_order order = std::memory_order_consume) const;

	const char* GetEnd() const;

	// ---------------------------------------------------

public:
	Checkpoint CreateCheckpoint() const;

	void RestoreCheckpoint(Checkpoint checkpoint);

	// ---------------------------------------------------

protected:
	void Reset(char* arena, const char* end);

	// - DATA MEMBERS ------------------------------------

private:
	Atomic<char*> _arena;
	const char*   _arenaEnd;
};

// ---------------------------------------------------

class ExternalArenaAllocator : public AbstractArenaAllocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Disable copy construction.
	ExternalArenaAllocator(const ExternalArenaAllocator&) = delete;
	//!	Constructs this @ref ExternalArenaAllocator instance.
	ExternalArenaAllocator(const Utf8Char* const name);
	//!	Constructs this @ref ExternalArenaAllocator instance.
	ExternalArenaAllocator(ExternalArenaAllocator&&);

	~ExternalArenaAllocator() = default;

	// ---------------------------------------------------

public:
	using AbstractArenaAllocator::Reset;

	// ---------------------------------------------------

	//!	Disable assignment.
	ExternalArenaAllocator& operator=(const ExternalArenaAllocator&) = delete;
};

// ---------------------------------------------------

class ArenaChildAllocator : public AbstractArenaAllocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Disable copy construction.
	ArenaChildAllocator(const ArenaChildAllocator&) = delete;
	//!	Constructs this @ref ArenaChildAllocator instance.
	ArenaChildAllocator(const Utf8Char* const name);
	//!	Constructs this @ref ArenaChildAllocator instance.
	ArenaChildAllocator(ArenaChildAllocator&&);

	~ArenaChildAllocator();

	// ---------------------------------------------------

public:
	ErrorCode BindResources(Allocator& allocator, SizeType sizeInBytes, AllocationDuration duration = AllocationDuration::Normal);

	void FreeResources();

	// ---------------------------------------------------

	//!	Disable copy assignment.
	ArenaChildAllocator& operator=(const ArenaChildAllocator&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
	Allocator* _parent;
	char*      _allocation;
};

// ---------------------------------------------------

template <size_t sizeInBytes>
class StackAllocator : public AbstractArenaAllocator {
	// - TYPE PUBLISHING ---------------------------------

public:
	enum : SizeType {
		ReservedSizeInBytes = sizeInBytes
	};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Disable copy construction.
	StackAllocator(const StackAllocator&) = delete;
	//! Constructs this @ref FixedStackAllocator instance.
	StackAllocator(const Utf8Char* const name);

	~StackAllocator() = default;

	// ---------------------------------------------------

	//!	Disable copy assignment.
	StackAllocator& operator=(const StackAllocator&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
	//!	Aligned for generic type support.
	ET16ByteAligned char _stack[ReservedSizeInBytes];
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/ArenaAllocator.inl>
//------------------------------------------------------------------//
