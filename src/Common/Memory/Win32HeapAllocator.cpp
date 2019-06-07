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
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Windows.h>
#include <malloc.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

namespace {

	enum : DWORD { HeapInitFlags = ETIsDebugBuild() ? HEAP_CREATE_ENABLE_TRACING : 0u };

} // anonymous namespace

AbstractWin32HeapAllocator::AbstractWin32HeapAllocator(HANDLE heap, const Utf8Char* const name) ETNoexceptHint : Allocator(name), _heap(heap) {
	ETAssert(_heap != nullptr, "Heap allocator requires a valid heap!");
}

// ---------------------------------------------------

ETRestrictHint void* AbstractWin32HeapAllocator::Allocate(SizeType byteSize, SizeType byteAlignment, SizeType byteOffset, AllocationDuration /*duration*/) ETNoexceptHint {
	if (ET_UNLIKELY((byteAlignment > 16u) | (byteOffset % byteAlignment != 0u))) {
		return nullptr;
	}

	return HeapAlloc(_heap, /*dwFlags =*/0, byteSize);
}

// ---------------------------------------------------

ETRestrictHint void* AbstractWin32HeapAllocator::Allocate(SizeType byteSize, AllocationDuration duration) ETNoexceptHint {
	return AbstractWin32HeapAllocator::Allocate(byteSize, /*byteAlignment =*/1u, /*byteOffset =*/0u, duration);
}

// ---------------------------------------------------

void AbstractWin32HeapAllocator::Deallocate(void* const address, SizeType /*sizeInBytes*/) ETNoexceptHint {
	if (ET_UNLIKELY(address == nullptr)) {
		return;
	}

	HeapFree(_heap, /*dwFlags =*/0, address);
}

// ---------------------------------------------------

Win32PrivateHeapAllocator::Win32PrivateHeapAllocator(SizeType byteLimit, const Utf8Char* const name) ETNoexceptHint : AbstractWin32HeapAllocator(HeapCreate(HeapInitFlags, SIZE_T(byteLimit), SIZE_T(byteLimit)), name) {}

// ---------------------------------------------------

Win32PrivateHeapAllocator::~Win32PrivateHeapAllocator() {
	HeapDestroy(AbstractWin32HeapAllocator::GetHeap());
}

// ---------------------------------------------------

Win32HeapAllocator::Win32HeapAllocator(const Utf8Char* const name) ETNoexceptHint : AbstractWin32HeapAllocator(HANDLE(_get_heap_handle()), name) {}

} // namespace Eldritch2
