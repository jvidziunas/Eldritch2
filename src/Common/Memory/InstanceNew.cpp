/*==================================================================*\
  InstanceNew.cpp
  ------------------------------------------------------------------
  Purpose:
  Provides externally-visible overrides/overloads for the C++ global
  operator new, used in place of malloc() and friends. The flow of
  execution is then routed through to an allocator object adhering
  to the Lakos allocator model (see the description in
  <Common/Memory/Allocator.hpp>).

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

//------------------------------------------------------------------//

ETRestrictHint ETNeverThrowsHint void* operator new(size_t sizeInBytes, Eldritch2::Allocator& allocator, Eldritch2::AllocationDuration duration, const char* /*file*/, int /*line*/) throw() {
	const auto result(static_cast<size_t*>(allocator.Allocate(sizeInBytes + sizeof(size_t), duration)));

	if (!result) {
		return nullptr;
	}

	result[0] = sizeInBytes;
	return result + 1;
}

// ---------------------------------------------------

ETRestrictHint ETNeverThrowsHint void* operator new(size_t sizeInBytes, Eldritch2::Allocator& allocator, Eldritch2::AllocationDuration duration) throw() {
	const auto result(static_cast<size_t*>(allocator.Allocate(sizeInBytes + sizeof(size_t), duration)));

	if (!result) {
		return nullptr;
	}

	result[0] = sizeInBytes;
	return result + 1;
}

// ---------------------------------------------------

ETRestrictHint ETNeverThrowsHint void* operator new(size_t sizeInBytes, Eldritch2::Allocator& allocator, size_t alignmentInBytes, Eldritch2::AllocationDuration duration, const char* /*file*/, int /*line*/) throw() {
	const auto result(static_cast<size_t*>(allocator.Allocate(sizeInBytes + sizeof(size_t), alignmentInBytes, sizeof(size_t), duration)));

	if (!result) {
		return nullptr;
	}

	result[0] = sizeInBytes;
	return result + 1;
}

// ---------------------------------------------------

ETRestrictHint ETNeverThrowsHint void* operator new(size_t sizeInBytes, Eldritch2::Allocator& allocator, size_t alignmentInBytes, Eldritch2::AllocationDuration duration) throw() {
	const auto result(static_cast<size_t*>(allocator.Allocate(sizeInBytes + sizeof(size_t), alignmentInBytes, sizeof(size_t), duration)));

	if (!result) {
		return nullptr;
	}

	result[0] = sizeInBytes;
	return result + 1;
}

// ---------------------------------------------------

ETNeverThrowsHint void operator delete(void* allocatedMemory, Eldritch2::Allocator& allocator, Eldritch2::AllocationDuration /*duration*/, const char* /*file*/, int /*line*/) throw() {
	allocator.Deallocate(allocatedMemory, static_cast<size_t*>(allocatedMemory)[-1]);
}

// ---------------------------------------------------

ETNeverThrowsHint void operator delete(void* allocatedMemory, Eldritch2::Allocator& allocator, Eldritch2::AllocationDuration /*duration*/) throw() {
	allocator.Deallocate(allocatedMemory, static_cast<size_t*>(allocatedMemory)[-1]);
}

// ---------------------------------------------------

ETNeverThrowsHint void operator delete(void* allocatedMemory, Eldritch2::Allocator& allocator, size_t /*alignmentInBytes*/, Eldritch2::AllocationDuration /*duration*/, const char* /*file*/, int /*line*/) throw() {
	allocator.Deallocate(allocatedMemory, static_cast<size_t*>(allocatedMemory)[-1]);
}

// ---------------------------------------------------

ETNeverThrowsHint void operator delete(void* allocatedMemory, Eldritch2::Allocator& allocator, size_t /*alignmentInBytes*/, Eldritch2::AllocationDuration /*duration*/) throw() {
	allocator.Deallocate(allocatedMemory, static_cast<size_t*>(allocatedMemory)[-1]);
}
