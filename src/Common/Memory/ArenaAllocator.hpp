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
#include <Common/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
enum class Result : int;
} // namespace Eldritch2

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
		ScopedCheckpoint(AbstractArenaAllocator& arena) ETNoexceptHint;
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
	//! Constructs this @ref AbstractArenaAllocator instance.
	ETConstexpr AbstractArenaAllocator(const Utf8Char* name, byte* head, byte* tail) ETNoexceptHint;
	//! Disable copy construction.
	AbstractArenaAllocator(const AbstractArenaAllocator&) = delete;

	~AbstractArenaAllocator();

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

public:
	ETRestrictHint void* Allocate(SizeType byteSize, SizeType byteAlignment, SizeType byteOffset, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint override sealed;
	ETRestrictHint void* Allocate(SizeType byteSize, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint override sealed;

	void Deallocate(void* const address, SizeType sizeInBytes) ETNoexceptHint override sealed;

	// ---------------------------------------------------

public:
	Checkpoint GetHead() const ETNoexceptHint;

	Checkpoint GetTail(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

	void Restore(Checkpoint checkpoint) ETNoexceptHint;

	// ---------------------------------------------------

protected:
	void Reset(byte* head, byte* tail) ETNoexceptHint;

	void Swap(AbstractArenaAllocator&) ETNoexceptHint;

	// ---------------------------------------------------

protected:
	bool IsValid(Checkpoint) const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
#if ET_ASSERTIONS_ENABLED
	Atomic<SizeType> _freedBytes; //!< Count the number of bytes returned to the allocator for leak detection.
#endif                            // ET_ASSERTIONS_ENABLED
	const byte*   _head;          //!< Head pointer from which new allocations will be made.
	Atomic<byte*> _tail;          //!< Tail pointer denoting the end of the owned region of memory.
};

// ---------------------------------------------------

class ArenaChildAllocator : public AbstractArenaAllocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ArenaChildAllocator instance.
	ETConstexpr ArenaChildAllocator(const Utf8Char* name) ETNoexceptHint;
	//!	Disable copy construction.
	ArenaChildAllocator(const ArenaChildAllocator&) = delete;
	//!	Constructs this @ref ArenaChildAllocator instance.
	ArenaChildAllocator(ArenaChildAllocator&&) ETNoexceptHint;

	~ArenaChildAllocator();

	// ---------------------------------------------------

public:
	Result BindResources(Allocator& allocator, SizeType byteSize, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint;

	void FreeResources() ETNoexceptHint;

	// ---------------------------------------------------

	//!	Disable copy assignment.
	ArenaChildAllocator& operator=(const ArenaChildAllocator&) = delete;
	ArenaChildAllocator& operator                              =(ArenaChildAllocator&&) ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	Allocator* _parent;

	// ---------------------------------------------------

	friend ETConstexpr bool operator==(const ArenaChildAllocator&, const ArenaChildAllocator&) ETNoexceptHint;

	friend ETConstexpr bool operator!=(const ArenaChildAllocator&, const ArenaChildAllocator&) ETNoexceptHint;

	friend void Swap(ArenaChildAllocator&, ArenaChildAllocator&) ETNoexceptHint;
};

// ---------------------------------------------------

template <size_t ByteSize>
class FixedArenaAllocator : public AbstractArenaAllocator {
	// - TYPE PUBLISHING ---------------------------------

public:
	enum : SizeType { ReservedByteSize = ByteSize };

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref FixedArenaAllocator instance.
	ETConstexpr FixedArenaAllocator(const Utf8Char* const name) ETNoexceptHint;
	//!	Disable copy construction.
	FixedArenaAllocator(const FixedArenaAllocator&) = delete;

	~FixedArenaAllocator() = default;

	// ---------------------------------------------------

	//!	Disable copy assignment.
	FixedArenaAllocator& operator=(const FixedArenaAllocator&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
	//!	Aligned for generic type support.
	ET16ByteAligned char _stack[ReservedByteSize];
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/ArenaAllocator.inl>
//------------------------------------------------------------------//
