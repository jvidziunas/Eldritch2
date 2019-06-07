/*==================================================================*\
  Win32HeapAllocator.hpp
  ------------------------------------------------------------------
  Purpose:
  Implementation of the aligned allocator interface using the
  standard Win32 heap functions.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/Allocator.hpp>
//------------------------------------------------------------------//

using HANDLE = void*;

namespace Eldritch2 {

class AbstractWin32HeapAllocator : public Allocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

protected:
	//! Constructs this @ref Win32HeapAllocatorBase instance.
	AbstractWin32HeapAllocator(HANDLE heap, const Utf8Char* const name) ETNoexceptHint;
	//!	Disable copy construction.
	AbstractWin32HeapAllocator(const AbstractWin32HeapAllocator&) = delete;

	~AbstractWin32HeapAllocator() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

public:
	ETRestrictHint void* Allocate(SizeType byteSize, SizeType byteAlignment, SizeType byteOffset, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint override sealed;
	ETRestrictHint void* Allocate(SizeType byteSize, AllocationDuration = AllocationDuration::Normal) ETNoexceptHint override sealed;

	void Deallocate(void* const address, SizeType byteSize) ETNoexceptHint override sealed;

	// - NATIVE HANDLE ACCESS ----------------------------

public:
	//! Retrieves the handle to the Win32 heap used for servicing allocations.
	ETConstexpr HANDLE GetHeap() const ETNoexceptHint;

	// ---------------------------------------------------

	//!	Disable assignment.
	AbstractWin32HeapAllocator& operator=(const AbstractWin32HeapAllocator&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
	HANDLE _heap;

	// ---------------------------------------------------

	friend ETConstexpr bool operator==(AbstractWin32HeapAllocator&, AbstractWin32HeapAllocator&) ETNoexceptHint;
};

// ---

class Win32PrivateHeapAllocator : public AbstractWin32HeapAllocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref Win32PrivateHeapAllocator instance.
	Win32PrivateHeapAllocator(SizeType allocationByteLimit, const Utf8Char* const name) ETNoexceptHint;
	//!	Disable copy construction.
	Win32PrivateHeapAllocator(const Win32PrivateHeapAllocator&) = delete;

	~Win32PrivateHeapAllocator();

	// ---------------------------------------------------

	//!	Disable assignment.
	Win32PrivateHeapAllocator& operator=(const Win32PrivateHeapAllocator&) = delete;
};

// ---

class Win32HeapAllocator : public AbstractWin32HeapAllocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref Win32GlobalHeapAllocator instance.
	Win32HeapAllocator(const Utf8Char* const name) ETNoexceptHint;
	//!	Constructs this @ref Win32GlobalHeapAllocator instance.
	Win32HeapAllocator(const Win32HeapAllocator&) ETNoexceptHint;

	~Win32HeapAllocator() = default;

	// ---------------------------------------------------

	//!	Disable assignment.
	Win32HeapAllocator& operator=(const Win32HeapAllocator&) = delete;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/Win32HeapAllocator.inl>
//------------------------------------------------------------------//
