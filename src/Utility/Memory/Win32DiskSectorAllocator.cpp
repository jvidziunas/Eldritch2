/*==================================================================*\
  Win32DiskSectorAllocator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Win32DiskSectorAllocator.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
//------------------------------------------------------------------//
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace {

	static ETNoAliasHint Win32DiskSectorAllocator::SizeType GetSystemAllocationGranularityInBytes() {
		::SYSTEM_INFO	systemInfo;

		::GetNativeSystemInfo( &systemInfo );

		return systemInfo.dwAllocationGranularity;
	}

// ---------------------------------------------------

	static ETNoAliasHint size_t GetAllocationSizeInBytes( const void* address ) {
		::MEMORY_BASIC_INFORMATION	informationBuffer;

		SetMemory( &informationBuffer, 0, sizeof(informationBuffer) );
		::VirtualQuery( address, &informationBuffer, sizeof(informationBuffer) );

		return informationBuffer.RegionSize;
	}

}	// anonymous namespace

namespace Eldritch2 {

	Win32DiskSectorAllocator::Win32DiskSectorAllocator( const SizeType allocationLimitInBytes, const UTF8Char* const name ) : Allocator( allocationLimitInBytes, name ), _allocationGranularityInBytes( GetSystemAllocationGranularityInBytes() ), _largePageMinimumSizeInBytes( GetLargePageMinimum() ) {}

// ---------------------------------------------------

	Win32DiskSectorAllocator::~Win32DiskSectorAllocator() {}

// ---------------------------------------------------

	ETRestrictHint void* Win32DiskSectorAllocator::Allocate( const SizeType sizeInBytes, const AllocationOptions allocationOptions ) {
		::DWORD	allocFlags( MEM_COMMIT | MEM_RESERVE );

		if( allocationOptions & AllocationOption::PERMANENT_ALLOCATION ) {
			allocFlags |= MEM_TOP_DOWN;
		}

		return ::VirtualAlloc( nullptr, sizeInBytes, allocFlags, PAGE_READWRITE );
	}

// ---------------------------------------------------

	ETRestrictHint void* Win32DiskSectorAllocator::Allocate( const SizeType sizeInBytes, const SizeType alignmentInBytes, const AllocationOptions allocationOptions ) {
		return (alignmentInBytes <= _allocationGranularityInBytes) ? this->Allocate( sizeInBytes, allocationOptions ) : nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* Win32DiskSectorAllocator::Reallocate( void* const address, const SizeType newSizeInBytes, const ReallocationOptions options ) {
		if( options & ReallocationOption::FAIL_IF_MOVE_REQUIRED ) {
			return nullptr;
		}

		if( void* const	newBuffer = this->Allocate( newSizeInBytes, options ) ) {
			CopyMemory( newBuffer, address, Min( GetAllocationSizeInBytes( address ), newSizeInBytes ) );
			this->Deallocate( address );

			return newBuffer;
		}

		return nullptr;
	}

// ---------------------------------------------------

	ETRestrictHint void* Win32DiskSectorAllocator::Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) {
		if( options & ReallocationOption::FAIL_IF_MOVE_REQUIRED ) {
			return nullptr;
		}

		if( void* const	newBuffer = this->Allocate( newSizeInBytes, alignmentInBytes, options ) ) {
			CopyMemory( newBuffer, address, Min( GetAllocationSizeInBytes( address ), newSizeInBytes ) );
			this->Deallocate( address, ::Eldritch2::AlignedDeallocationSemantics );

			return newBuffer;
		}

		return nullptr;
	}

// ---------------------------------------------------

	void Win32DiskSectorAllocator::Deallocate( void* const address ) {
		if( ETBranchLikelyHint( nullptr != address ) ) {
			::VirtualFree( address, 0u, MEM_RELEASE );
		}
	}

// ---------------------------------------------------

	void Win32DiskSectorAllocator::Deallocate( void* const address, const AlignedDeallocationSemantics ) {
		this->Deallocate( address );
	}

// ---------------------------------------------------

	Win32DiskSectorAllocator::SizeType Win32DiskSectorAllocator::EstimateActualAllocationSizeInBytes( const SizeType allocationSizeInBytes, const SizeType /* alignmentInBytes */ ) const {
		return( ( allocationSizeInBytes + ( _allocationGranularityInBytes - 1u ) ) / _allocationGranularityInBytes );
	}

}	// namespace Eldritch2