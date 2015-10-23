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
//------------------------------------------------------------------//
#include <algorithm>
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::std;

// There are a few combo assignment/conditional expressions in
// the heap manipulation functions here. All are intentional.
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
#		pragma warning( disable : 4706 )
#endif

#ifdef CopyMemory
#	undef CopyMemory
#endif

namespace {

	enum : ::DWORD {
		HEAP_INIT_FLAGS	= ETIsDebugModeEnabled() ? HEAP_CREATE_ENABLE_TRACING : 0u
	};

}	// anonymous namespace

namespace Eldritch2 {
namespace Detail {

	Win32HeapAllocatorBase::Win32HeapAllocatorBase( const ::HANDLE heapHandle, const UTF8Char* const name ) : Allocator( name ), _heap( heapHandle ) {
		ETRuntimeAssert( nullptr != _heap );
	}

// ---------------------------------------------------

	ETRestrictHint void* Win32HeapAllocatorBase::Allocate( const SizeType sizeInBytes, const AllocationOptions ) {
		return ::HeapAlloc( _heap, 0, sizeInBytes );
	}

// ---------------------------------------------------

	ETRestrictHint void* Win32HeapAllocatorBase::Reallocate( void* const address, const SizeType newSizeInBytes, const AllocationOptions options ) {
		if( !address ) {
			return this->Allocate( newSizeInBytes, options );
		}

		return ::HeapReAlloc( _heap, (options & ReallocationOption::FAIL_IF_MOVE_REQUIRED) ? HEAP_REALLOC_IN_PLACE_ONLY : 0u, address, newSizeInBytes );
	}

// ---------------------------------------------------

	ETRestrictHint void* Win32HeapAllocatorBase::Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) {
		if( !address ) {
			return this->Allocate( newSizeInBytes, alignmentInBytes, options );
		}

		if( void* const returnPointer = ::HeapReAlloc( _heap, HEAP_REALLOC_IN_PLACE_ONLY, address, this->EstimateActualAllocationSizeInBytes( newSizeInBytes, alignmentInBytes ) ) ) {
			return returnPointer;
		}

		if( options & ReallocationOption::FAIL_IF_MOVE_REQUIRED ) {
			return nullptr;
		}

		if( void* const newAllocation = this->Allocate( this->EstimateActualAllocationSizeInBytes( newSizeInBytes, alignmentInBytes ), alignmentInBytes, options ) ) {
			const size_t	overheadInBytes( static_cast<size_t>(static_cast<char*>(address) - static_cast<char*>(GetAllocationPointerFromAlignedUserPointer( address )) ) );

			::Eldritch2::CopyMemory( newAllocation, address, ::HeapSize( _heap, 0, GetAllocationPointerFromAlignedUserPointer( address ) ) - overheadInBytes );
			this->Deallocate( address, ::Eldritch2::AlignedDeallocationSemantics );

			return newAllocation;
		}

		return nullptr;
	}

// ---------------------------------------------------

	void Win32HeapAllocatorBase::Deallocate( void* const address ) {
		// HeapSize barfs horribly if we feed it a null by accident.
		if( ETBranchLikelyHint( nullptr != address ) ) {
			::HeapFree( _heap, 0, address );
		}
	}

// ---------------------------------------------------

	::HANDLE Win32HeapAllocatorBase::GetHeapHandle() const {
		return _heap;
	}

}	// namespace Detail

	Win32GlobalHeapAllocator::Win32GlobalHeapAllocator( const ::Eldritch2::UTF8Char* const name ) : Detail::Win32HeapAllocatorBase( reinterpret_cast<::HANDLE>(::_get_heap_handle()), name ) {}

// ---------------------------------------------------

	Win32PrivateHeapAllocator::Win32PrivateHeapAllocator( const SizeType allocationLimit, const ::Eldritch2::UTF8Char* const name ) : Detail::Win32HeapAllocatorBase( ::HeapCreate( HEAP_INIT_FLAGS, static_cast<SIZE_T>(0u), static_cast<SIZE_T>(allocationLimit)), name ) {}
	
// ---------------------------------------------------

	Win32PrivateHeapAllocator::~Win32PrivateHeapAllocator() {
		::HeapDestroy( this->GetHeapHandle() );
	}

}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
