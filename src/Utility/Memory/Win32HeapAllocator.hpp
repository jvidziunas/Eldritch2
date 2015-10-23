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
#include <Utility/Memory/Allocator.hpp>
//------------------------------------------------------------------//

typedef void*	HANDLE;

namespace Eldritch2 {
namespace Detail {

	class Win32HeapAllocatorBase : public ::Eldritch2::Allocator {
	// - MEMORY ALLOCATION/DEALLOCATION ------------------

	public:
		using Allocator::Allocate;
		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const AllocationOptions options ) override sealed;

		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const ReallocationOptions options ) override sealed;
		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) override sealed;

		using Allocator::Deallocate;
		void					Deallocate( void* const address ) override sealed;

	// - NATIVE HANDLE ACCESS ----------------------------

		//! Retrieves the handle to the Win32 heap used for servicing allocations.
		::HANDLE	GetHeapHandle() const;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref Win32HeapAllocatorBase instance.
		Win32HeapAllocatorBase( const ::HANDLE heapHandle, const ::Eldritch2::UTF8Char* const name );

		//! Destroys this @ref Win32HeapAllocatorBase instance.
		~Win32HeapAllocatorBase() = default;

	// - DATA MEMBERS ------------------------------------

	private:
		const ::HANDLE	_heap;
	};

}	// namespace Detail

	class Win32GlobalHeapAllocator : public Detail::Win32HeapAllocatorBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Win32GlobalHeapAllocator instance.
		Win32GlobalHeapAllocator( const ::Eldritch2::UTF8Char* const name );

		//! Destroys this @ref Win32GlobalHeapAllocator instance.
		~Win32GlobalHeapAllocator() = default;
	};

// ---------------------------------------------------

	class Win32PrivateHeapAllocator : public Detail::Win32HeapAllocatorBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Win32PrivateHeapAllocator instance.
		Win32PrivateHeapAllocator( const SizeType allocationLimit, const ::Eldritch2::UTF8Char* const name );

		//! Destroys this @ref Win32PrivateHeapAllocator instance.
		~Win32PrivateHeapAllocator();
	};

}	// namespace Eldritch2
