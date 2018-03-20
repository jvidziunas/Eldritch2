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

typedef void*	HANDLE;

namespace Eldritch2 {
namespace Detail {

	class Win32HeapAllocatorBase : public Allocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//! Constructs this @ref Win32HeapAllocatorBase instance.
		Win32HeapAllocatorBase( HANDLE heapHandle, const Utf8Char* const name );
	//!	Disable copy construction.
		Win32HeapAllocatorBase( const Win32HeapAllocatorBase& ) = delete;

		~Win32HeapAllocatorBase() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

	public:
		ETRestrictHint void*	Allocate( SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration options = AllocationDuration::Normal ) override sealed;
		ETRestrictHint void*	Allocate( SizeType sizeInBytes, AllocationDuration duration = AllocationDuration::Normal ) override sealed;

		void					Deallocate( void* const address, SizeType sizeInBytes ) override sealed;

	// - NATIVE HANDLE ACCESS ----------------------------

	public:
	//! Retrieves the handle to the Win32 heap used for servicing allocations.
		HANDLE	GetHeapHandle() const;

	// ---------------------------------------------------

	//!	Disable assignment.
		Win32HeapAllocatorBase&	operator=( const Win32HeapAllocatorBase& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const HANDLE	_heap;
	};

}	// namespace Detail

	class Win32GlobalHeapAllocator : public Detail::Win32HeapAllocatorBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Win32GlobalHeapAllocator instance.
		Win32GlobalHeapAllocator( const Win32GlobalHeapAllocator& );
	//! Constructs this @ref Win32GlobalHeapAllocator instance.
		Win32GlobalHeapAllocator( const Utf8Char* const name );

		~Win32GlobalHeapAllocator() = default;

	// ---------------------------------------------------

	//!	Disable assignment.
		Win32GlobalHeapAllocator&	operator=( const Win32GlobalHeapAllocator& ) = delete;
	};

// ---------------------------------------------------

	class Win32PrivateHeapAllocator : public Detail::Win32HeapAllocatorBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref Win32PrivateHeapAllocator instance.
		Win32PrivateHeapAllocator( SizeType allocationLimitInBytes, const Utf8Char* const name );
	//!	Disable copy construction.
		Win32PrivateHeapAllocator( const Win32PrivateHeapAllocator& ) = delete;

		~Win32PrivateHeapAllocator();

	// ---------------------------------------------------

	//!	Disable assignment.
		Win32PrivateHeapAllocator&	operator=( const Win32PrivateHeapAllocator& ) = delete;
	};

}	// namespace Eldritch2
