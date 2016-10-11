/*==================================================================*\
  Win32HeapAllocator.cpp
  ------------------------------------------------------------------
  Purpose:
  Implementation of the aligned allocator interface using the
  standard Win32 heap functions.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Win32HeapAllocator.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <malloc.h>
//------------------------------------------------------------------//

// There are a few combo assignment/conditional expressions in
// the heap manipulation functions here. All are intentional.
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
#		pragma warning( disable : 4706 )
#endif

#ifdef CopyMemory
#	undef CopyMemory
#endif

namespace Eldritch2 {
namespace {

	enum : DWORD {
		HeapInitFlags	= ETIsDebugModeEnabled() ? HEAP_CREATE_ENABLE_TRACING : 0u
	};

}	// anonymous namespace

namespace Detail {

	Win32HeapAllocatorBase::Win32HeapAllocatorBase( HANDLE heapHandle, const Utf8Char* const name ) : Allocator( name ), _heap( heapHandle ) {
		ETRuntimeAssert( nullptr != _heap );
	}

// ---------------------------------------------------

	ETRestrictHint void* Win32HeapAllocatorBase::Allocate( SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration /*duration*/ ) {
		if( ETBranchUnlikelyHint( ( alignmentInBytes > 16u ) | (offsetInBytes % alignmentInBytes != 0u) ) ) {
			return nullptr;
		}

		return HeapAlloc( _heap, 0, sizeInBytes );
	}

// ---------------------------------------------------

	ETRestrictHint void* Win32HeapAllocatorBase::Allocate( SizeType sizeInBytes, AllocationDuration duration ) {
		return Win32HeapAllocatorBase::Allocate( sizeInBytes, 1u, 0u, duration ); 
	}

// ---------------------------------------------------

	void Win32HeapAllocatorBase::Deallocate( void* const address, SizeType /*sizeInBytes*/ ) {
		if( ETBranchUnlikelyHint( nullptr == address ) ) {
			return;
		}

		HeapFree( _heap, 0, address );
	}

// ---------------------------------------------------

	HANDLE Win32HeapAllocatorBase::GetHeapHandle() const {
		return _heap;
	}

}	// namespace Detail

	Win32GlobalHeapAllocator::Win32GlobalHeapAllocator( const Utf8Char* const name ) : Detail::Win32HeapAllocatorBase( reinterpret_cast<HANDLE>(_get_heap_handle()), name ) {}

// ---------------------------------------------------

	Win32GlobalHeapAllocator::Win32GlobalHeapAllocator( const Win32GlobalHeapAllocator& allocator ) : Detail::Win32HeapAllocatorBase( reinterpret_cast<HANDLE>( _get_heap_handle() ), allocator.GetName() ) {}

// ---------------------------------------------------

	Win32PrivateHeapAllocator::Win32PrivateHeapAllocator( SizeType allocationLimit, const Utf8Char* const name ) : Detail::Win32HeapAllocatorBase( HeapCreate( HeapInitFlags, static_cast<SIZE_T>( 0u ), static_cast<SIZE_T>( allocationLimit ) ), name ) {}
	
// ---------------------------------------------------

	Win32PrivateHeapAllocator::~Win32PrivateHeapAllocator() {
		HeapDestroy( Win32HeapAllocatorBase::GetHeapHandle() );
	}

}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
