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
#include <Common/Mpl/CharTypes.hpp>
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

enum class AllocationDuration : int {
	Temporary = 0,
	Normal    = 1,
	Permanent = 2
};

// ---

class ETPureAbstractHint Allocator {
	// - TYPE PUBLISHING ---------------------------------

public:
	using SizeType = size_t;

	ET_PUSH_MSVC_WARNING_STATE(disable : 4200)

	template <size_t Alignment>
	struct AllocationHeader {
		// - TYPE PUBLISHING ---------------------------------

	public:
		static ETConstexpr size_t DataAlignment = Alignment;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Disable copy construction.
		AllocationHeader(const AllocationHeader&) = delete;
		//! Constructs this @ref AllocationHeader instance.
		AllocationHeader(SizeType byteSize) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		static ETConstexpr const AllocationHeader* Get(const void* allocation) ETNoexceptHint;
		static ETConstexpr AllocationHeader* Get(void* allocation) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		static ETRestrictHint void* operator new(size_t, Allocator& allocator, SizeType userBytes) ETNoexceptHint;
		static void                 operator delete(void*, Allocator& allocator, SizeType userBytes)ETNoexceptHint;

		// ---------------------------------------------------

		//! Disable copy assignment.
		AllocationHeader& operator=(const AllocationHeader&) = delete;

		// - DATA MEMBERS ------------------------------------

	public:
		const SizeType userByteSize;
		alignas(Alignment) byte userBytes[];
	};

	ET_POP_MSVC_WARNING_STATE()

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

protected:
	//! Constructs this @ref Allocator instance.
	ETConstexpr Allocator(const Utf8Char* const name) ETNoexceptHint;
	//! Constructs this @ref Allocator instance.
	ETConstexpr Allocator(const Allocator&) ETNoexceptHint = default;

	~Allocator() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

public:
	//! Allocates a contiguous chunk of memory with the specified length and alignment using the passed-in allocation behavior duration.
	/*!	@param[in] byteSize Minimum size of the allocation, in bytes.
		@param[in] byteAlignment Minimum alignment of the allocation, in bytes.
		@param[in] byteOffset Offset into the allocation for which alignment should be considered.
		@param[in] duration Estimated allocation lifetime. Implementations may change their allocation strategy based on this value.
		@returns A pointer to the new block of memory, or a null pointer if the request could not be satisfied. */
	virtual ETRestrictHint void* Allocate(SizeType byteSize, SizeType byteAlignment, SizeType byteOffset, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint abstract;
	//! Allocates a contiguous chunk of memory with the specified length and alignment using the passed-in allocation behavior duration.
	/*!	@param[in] byteSize Minimum size of the allocation, in bytes.
		@param[in] duration Estimated allocation lifetime. Implementations may change their allocation strategy based on this value.
		@returns A pointer to the new block of memory, or a null pointer if the request could not be satisfied. */
	virtual ETRestrictHint void* Allocate(SizeType byteSize, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint abstract;

	//! Releases a chunk of memory previously allocated by @ref Allocate().
	/*!	@param[in] address Pointer to beginning of the user allocation to be released.
		@param[in] byteSize @parblock Client-facing size value for the allocation, in bytes. This *must* exactly match the size used in the corresponding call
			to @ref Allocate() that produced the allocation. @endparblock */
	virtual void Deallocate(void* const address, SizeType byteSize) ETNoexceptHint abstract;

	// - DEBUG INFORMATION -------------------------------

public:
	//! Gets a human-readable string identifying the allocator.
	ETConstexpr const Utf8Char* GetName() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	Allocator& operator=(const Allocator&) ETNoexceptHint = default;

	// - DATA MEMBERS ------------------------------------

private:
#if ET_DEBUG_BUILD
	const Utf8Char* _name;
#endif
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/Allocator.inl>
//------------------------------------------------------------------//
