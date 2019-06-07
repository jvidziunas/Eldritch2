/*==================================================================*\
  ArenaAllocator.hpp
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
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Algorithms.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETInlineHint ETForceInlineHint AbstractArenaAllocator::ScopedCheckpoint::ScopedCheckpoint(AbstractArenaAllocator& arena) ETNoexceptHint : _arena(ETAddressOf(arena)), _checkpoint(arena.GetHead()) {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint AbstractArenaAllocator::ScopedCheckpoint::~ScopedCheckpoint() {
	_arena->Restore(_checkpoint);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint AbstractArenaAllocator::AbstractArenaAllocator(const Utf8Char* name, byte* begin, byte* end) ETNoexceptHint : Allocator(name),
#if ET_ASSERTIONS_ENABLED
																																			_freedBytes(0u),
#endif // ET_ASSERTIONS_ENABLED
																																			_head(begin),
																																			_tail(end) {
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint AbstractArenaAllocator::Checkpoint AbstractArenaAllocator::GetHead() const ETNoexceptHint {
	return _head;
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint AbstractArenaAllocator::Checkpoint AbstractArenaAllocator::GetTail(MemoryOrder order) const ETNoexceptHint {
	return _tail.load(order);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint bool AbstractArenaAllocator::IsValid(Checkpoint checkpoint) const ETNoexceptHint {
	return checkpoint >= GetTail(std::memory_order_consume);
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ArenaChildAllocator::ArenaChildAllocator(const Utf8Char* name) ETNoexceptHint : AbstractArenaAllocator(name, /*head =*/nullptr, /*tail =*/nullptr), _parent(nullptr) {}

// ---------------------------------------------------

template <size_t ByteSize>
ETConstexpr ETForceInlineHint FixedArenaAllocator<ByteSize>::FixedArenaAllocator(const Utf8Char* const name) ETNoexceptHint : AbstractArenaAllocator(name) {
	Reset(Begin(_stack), End(_stack));
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint bool operator==(const ArenaChildAllocator& lhs, const ArenaChildAllocator& rhs) ETNoexceptHint {
	return lhs._parent == rhs._parent;
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint bool operator!=(const ArenaChildAllocator& lhs, const ArenaChildAllocator& rhs) ETNoexceptHint {
	return !(lhs == rhs);
}

} // namespace Eldritch2
