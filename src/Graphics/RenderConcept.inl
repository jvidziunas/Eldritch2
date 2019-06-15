/*==================================================================*\
  RenderConcept.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobExecutor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	template <typename Record, typename Key>
	ETInlineHint ETForceInlineHint RenderSortPredicate<Record, Key>::RenderSortPredicate(Vector cellSize) ETNoexceptHint : _inverseCellSize(~cellSize) {}

	// ---------------------------------------------------

	template <typename Record, typename Key>
	ETInlineHint ETForceInlineHint void RenderSortPredicate<Record, Key>::Sort(Scheduling::JobExecutor& executor, Iterator begin, Iterator end, KeyType outKeys[]) ETNoexceptHint {
		executor.ForEachSplit<16u>(begin, end, outKeys, [](Scheduling::JobExecutor& /*executor*/, Iterator begin, Iterator end, KeyType outKeys[]) ETNoexceptHint {

		});
	}

	// ---------------------------------------------------

	template <typename Record, typename Key>
	template <typename Sector>
	ETInlineHint ETForceInlineHint uint32 RenderSortPredicate<Record, Key>::Split(Scheduling::JobExecutor& executor, ConstIterator begin, ConstIterator end, Sector sectors[]) ETNoexceptHint {
		struct {
			Atomic<uint32> position;
		} sharedData;

		sharedData.position.store(0u, std::memory_order_relaxed);
		executor.ForEachSplit<64u>(begin, end, [&](Scheduling::JobExecutor& /*executor*/, ConstIterator begin, ConstIterator end) ETNoexceptHint {
			sectors[sharedData.position.fetch_add(1u)];
		});

		return sharedData.position.load(std::memory_order_consume);
	}

	// ---------------------------------------------------

	template <typename Record, class Allocator>
	ETInlineHint ETForceInlineHint RenderConcept<Record, Allocator>::RenderConcept(const AllocatorType& allocator, const SortType& sort) ETNoexceptHint : BvhType(allocator, sort), _shouldRebuildHierarchy(false) {}

	// ---------------------------------------------------

	template <typename Record, class Allocator>
	ETInlineHint ETForceInlineHint bool RenderConcept<Record, Allocator>::ShouldRebuildHierarchy(MemoryOrder order) ETNoexceptHint {
		return _shouldRebuildHierarchy.load(order);
	}

}} // namespace Eldritch2::Graphics
