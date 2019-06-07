/*==================================================================*\
  ArenaAllocator.cpp
  ------------------------------------------------------------------
  Purpose:
  Provides a wrapper around a surrogate 'stack' object for (super)
  fast allocatons and amortized releases. Note that this speed comes
  at a design cost, as the overall size of the allocations must be
  tracked and controlled with a considerably higher degree of
  fidelity, often requiring external allocation size profiles, etc.

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

namespace Eldritch2 {

AbstractArenaAllocator::~AbstractArenaAllocator() {
	ETAssert(_head == _tail.load(std::memory_order_consume), "arena {} is leaking memory!", fmt::ptr(this));
}

// ---------------------------------------------------

ETRestrictHint void* AbstractArenaAllocator::Allocate(SizeType byteSize, SizeType byteAlignment, SizeType /*byteOffset*/, AllocationDuration duration) ETNoexceptHint {
	SizeType allocationByteSize(byteSize + (byteAlignment - 1u));
	void*    allocation(AbstractArenaAllocator::Allocate(allocationByteSize, duration));

	return allocation ? eastl::align(byteAlignment, byteSize, allocation, allocationByteSize) : nullptr;
}

// ---------------------------------------------------

ETRestrictHint void* AbstractArenaAllocator::Allocate(SizeType sizeInBytes, AllocationDuration /*duration*/) ETNoexceptHint {
	const byte* const head(_head);
	byte*             allocation(_tail.load(std::memory_order_consume));

	do {
		if (SizeType(allocation - head) < sizeInBytes) {
			return nullptr;
		}
		allocation = allocation - sizeInBytes;
	} while (!_tail.compare_exchange_weak(allocation, allocation - sizeInBytes, std::memory_order_release));

	return allocation;
}

// ---------------------------------------------------

void AbstractArenaAllocator::Deallocate(void* const address, SizeType byteSize) ETNoexceptHint {
#if ET_ASSERTIONS_ENABLED
	_freedBytes.fetch_add(address ? byteSize : 0u, std::memory_order_relaxed);
#else  // !ET_ASSERTIONS_ENABLED
	ETUnreferencedParameter(address);
	ETUnreferencedParameter(byteSize);
#endif // ET_ASSERTIONS_ENABLED
}

// ---------------------------------------------------

void AbstractArenaAllocator::Restore(Checkpoint checkpoint) ETNoexceptHint {
	ETAssert(IsValid(checkpoint), "Restoring invalid checkpoint {} on allocator {}!", fmt::ptr(checkpoint), fmt::ptr(this));
	_tail.store(static_cast<byte*>(const_cast<void*>(checkpoint)), std::memory_order_release);
}

// ---------------------------------------------------

void AbstractArenaAllocator::Reset(byte* head, byte* tail) ETNoexceptHint {
#if ET_ASSERTIONS_ENABLED
	_freedBytes.store(0u, std::memory_order_release);
#endif // ET_ASSERTIONS_ENABLED
	_head = head;
	_tail.store(tail, std::memory_order_release);
}

// ---------------------------------------------------

void AbstractArenaAllocator::Swap(AbstractArenaAllocator& rhs) ETNoexceptHint {
	using ::Eldritch2::Swap;

	Swap(_head, rhs._head);
	_tail.store(rhs._tail.exchange(_tail.load(std::memory_order_relaxed), std::memory_order_release), std::memory_order_release);
}

// ---------------------------------------------------

ArenaChildAllocator::ArenaChildAllocator(ArenaChildAllocator&& allocator) ETNoexceptHint : ArenaChildAllocator(allocator.GetName()) {
	using ::Eldritch2::Swap;
	Swap(*this, allocator);
}

// ---------------------------------------------------

ArenaChildAllocator::~ArenaChildAllocator() {
	FreeResources();
}

// ---------------------------------------------------

Result ArenaChildAllocator::BindResources(Allocator& allocator, SizeType byteSize, AllocationDuration /*duration*/) ETNoexceptHint {
	using Header = AllocationHeader<16u>;

	const auto previous(Header::Get(AbstractArenaAllocator::GetHead()));
	const auto header(new (allocator, byteSize) Header(byteSize));
	ET_ABORT_UNLESS(header ? Result::Success : Result::OutOfMemory);

	if (ETIsDebugBuild()) {
		Fill(header->userBytes, header->userBytes + byteSize, byte(0u));
	}

	AbstractArenaAllocator::Reset(header->userBytes, header->userBytes + byteSize);
	if (Allocator* parent = Exchange(_parent, ETAddressOf(allocator))) {
		parent->Deallocate(const_cast<Header*>(previous), sizeof(Header) + previous->userByteSize);
	}

	return Result::Success;
}

// ---------------------------------------------------

void ArenaChildAllocator::FreeResources() ETNoexceptHint {
	using Header = AllocationHeader<16u>;

	const auto previous(Header::Get(AbstractArenaAllocator::GetHead()));

	AbstractArenaAllocator::Reset(/*head =*/nullptr, /*tail =*/nullptr);
	if (Allocator* parent = Exchange(_parent, nullptr)) {
		parent->Deallocate(const_cast<Header*>(previous), sizeof(Header) + previous->userByteSize);
	}
}

// ---------------------------------------------------

void Swap(ArenaChildAllocator& lhs, ArenaChildAllocator& rhs) ETNoexceptHint {
	lhs.Swap(rhs);
	Swap(lhs._parent, rhs._parent);
}

} // namespace Eldritch2
