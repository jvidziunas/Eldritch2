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

	template <typename Instance, typename Key>
	ETInlineHint ETForceInlineHint RenderSortPredicate<Instance, Key>::RenderSortPredicate(Vector cellSize) ETNoexceptHint : _inverseCellSize(~cellSize) {}

	// ---------------------------------------------------

	template <typename Instance, typename Key>
	ETInlineHint ETForceInlineHint void RenderSortPredicate<Instance, Key>::SortValues(Scheduling::JobExecutor& executor, Iterator begin, Iterator end, KeyType outKeys[]) ETNoexceptHint {
		executor.ForEachSplit<16u>(begin, end, outKeys, [](Scheduling::JobExecutor& /*executor*/, Iterator begin, Iterator end, KeyType outKeys[]) ETNoexceptHint {

		});
	}

	// ---------------------------------------------------

	template <typename Instance, typename Key>
	template <typename Split>
	ETInlineHint ETForceInlineHint uint32 RenderSortPredicate<Instance, Key>::FindSplits(Scheduling::JobExecutor& executor, ConstIterator begin, ConstIterator end, Split splits[]) ETNoexceptHint {
		struct {
			Atomic<uint32> position;
		} sharedData;

		sharedData.position.store(0u, std::memory_order_relaxed);
		executor.ForEachSplit<64u>(begin, end, [&](Scheduling::JobExecutor& /*executor*/, ConstIterator begin, ConstIterator end) ETNoexceptHint {
			splits[sharedData.position.fetch_add(1u)];
		});

		return sharedData.position.load(std::memory_order_consume);
	}

	// ---------------------------------------------------

	template <typename Instance, class Allocator>
	ETInlineHint ETForceInlineHint RenderConcept<Instance, Allocator>::RenderConcept(const AllocatorType& allocator, const SortType& sort) ETNoexceptHint : BvhType(allocator, sort), _shouldRebuildHierarchy(false) {}

	// ---------------------------------------------------

	template <typename Instance, class Allocator>
	ETInlineHint ETForceInlineHint bool RenderConcept<Instance, Allocator>::ShouldRebuildHierarchy(MemoryOrder order) ETNoexceptHint {
		return _shouldRebuildHierarchy.load(order);
	}

	// ---------------------------------------------------

	template <typename ViewType, typename Instance, class Allocator>
	ETInlineHint ETNoInlineHint void BuildShadowViewList(ArrayList<ViewType>& views, const RenderConcept<Instance, Allocator>& source) {

	}

	// ---------------------------------------------------

	template <typename ViewType, typename Instance, class Allocator>
	ETInlineHint ETNoInlineHint void BuildViewList(ArrayList<ViewType>& views, const RenderConcept<Instance, Allocator>& source) {

	}

}} // namespace Eldritch2::Graphics
