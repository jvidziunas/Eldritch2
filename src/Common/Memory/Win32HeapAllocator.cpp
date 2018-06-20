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
#include <Common/Memory/Win32HeapAllocator.hpp>
#include <Common/Assert.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
#include <malloc.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	enum : DWORD { HeapInitFlags = ETIsDebugBuild() ? HEAP_CREATE_ENABLE_TRACING : 0u };

} // anonymous namespace

namespace Detail {

	Win32HeapAllocatorBase::Win32HeapAllocatorBase(HANDLE heap, const Utf8Char* const name) :
		Allocator(name),
		_heap(heap) {
		ET_ASSERT(_heap != nullptr, "Heap allocator requires a valid heap!");
	}

	// ---------------------------------------------------

	ETRestrictHint void* Win32HeapAllocatorBase::Allocate(SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration /*duration*/) {
		if (ET_UNLIKELY((alignmentInBytes > 16u) | (offsetInBytes % alignmentInBytes != 0u))) {
			return nullptr;
		}

		return HeapAlloc(_heap, 0, sizeInBytes);
	}

	// ---------------------------------------------------

	ETRestrictHint void* Win32HeapAllocatorBase::Allocate(SizeType sizeInBytes, AllocationDuration duration) {
		return Win32HeapAllocatorBase::Allocate(sizeInBytes, 1u, 0u, duration);
	}

	// ---------------------------------------------------

	void Win32HeapAllocatorBase::Deallocate(void* const address, SizeType /*sizeInBytes*/) {
		if (ET_UNLIKELY(address == nullptr)) {
			return;
		}

		HeapFree(_heap, 0, address);
	}

	// ---------------------------------------------------

	HANDLE Win32HeapAllocatorBase::GetHeap() const {
		return _heap;
	}

} // namespace Detail

Win32GlobalHeapAllocator::Win32GlobalHeapAllocator(const Utf8Char* const name) :
	Detail::Win32HeapAllocatorBase(reinterpret_cast<HANDLE>(_get_heap_handle()), name) {}

// ---------------------------------------------------

Win32GlobalHeapAllocator::Win32GlobalHeapAllocator(const Win32GlobalHeapAllocator& allocator) :
	Detail::Win32HeapAllocatorBase(reinterpret_cast<HANDLE>(_get_heap_handle()), allocator.GetName()) {}

// ---------------------------------------------------

Win32PrivateHeapAllocator::Win32PrivateHeapAllocator(SizeType allocationLimit, const Utf8Char* const name) :
	Detail::Win32HeapAllocatorBase(HeapCreate(HeapInitFlags, static_cast<SIZE_T>(0u), static_cast<SIZE_T>(allocationLimit)), name) {}

// ---------------------------------------------------

Win32PrivateHeapAllocator::~Win32PrivateHeapAllocator() {
	HeapDestroy(Win32HeapAllocatorBase::GetHeap());
}

} // namespace Eldritch2
