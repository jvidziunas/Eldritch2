/*==================================================================*\
  Win32HeapAllocator.hpp
  ------------------------------------------------------------------
  Purpose:
  Implementation of the aligned allocator interface using the
  standard Win32 heap functions.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Allocator.hpp>
//------------------------------------------------------------------//

typedef void*	HANDLE;

namespace Eldritch2Detail {

	class Win32HeapAllocatorBase : public ::Eldritch2::Allocator {
	// - MEMORY ALLOCATION/DEALLOCATION ------------------

	public:
		using Allocator::Allocate;
		// Allocates a contiguous chunk of memory with the specified length using the passed-in allocation behavior options.
		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const AllocationOptions options ) override sealed;

		// Given a chunk of memory, attempts to expand or separately allocate a new chunk with size greater than or equal to the requested size.
		// The contents of the source memory are then copied over into the new region, if appropriate.
		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const ReallocationOptions options ) override sealed;
		// Given a chunk of memory, attempts to expand or separately allocate a new chunk with size greater than or equal to the requested size.
		// The contents of the source memory are then copied over into the new region, if appropriate.
		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) override sealed;

		using Allocator::Deallocate;
		// Releases a chunk of memory previously allocated by Allocate() or Reallocate() WITHOUT aligned semantics.
		void					Deallocate( void* const address ) override sealed;

	// - NATIVE HANDLE ACCESS ----------------------------

		// Retrieves the handle to the Win32 heap used for servicing allocations.
		::HANDLE	GetHeapHandle() const;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this Win32HeapAllocatorBase instance.
		Win32HeapAllocatorBase( const ::HANDLE heapHandle, const ::Eldritch2::UTF8Char* const name );

		// Destroys this Win32HeapAllocatorBase instance.
		~Win32HeapAllocatorBase();

	// - DATA MEMBERS ------------------------------------

	private:
		const ::HANDLE	_heap;
	};

}	// namespace Eldritch2Detail


namespace Eldritch2 {

	class Win32GlobalHeapAllocator : public ::Eldritch2Detail::Win32HeapAllocatorBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this Win32GlobalHeapAllocator instance.
		Win32GlobalHeapAllocator( const ::Eldritch2::UTF8Char* const name );

		// Destroys this Win32GlobalHeapAllocator instance.
		~Win32GlobalHeapAllocator();
	};

// ---------------------------------------------------

	class Win32PrivateHeapAllocator : public ::Eldritch2Detail::Win32HeapAllocatorBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this Win32PrivateHeapAllocator instance.
		Win32PrivateHeapAllocator( const SizeType allocationLimit, const ::Eldritch2::UTF8Char* const name );

		// Destroys this Win32PrivateHeapAllocator instance.
		~Win32PrivateHeapAllocator();
	};

}	// namespace Eldritch2
