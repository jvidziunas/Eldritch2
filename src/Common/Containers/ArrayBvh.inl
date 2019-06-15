/*==================================================================*\
  ArrayBvh.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint ArrayBvh<Value, SortPredicate, Allocator>::ArrayBvh(const AllocatorType& allocator, const SortType& sort) ETNoexceptHint : _leaves(allocator), _sort(sort), _sectors(SizeType(0u)) {}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void ArrayBvh<Value, SortPredicate, Allocator>::Insert(const ValueType& value) {
	_leaves.EmplaceBack(value, typename SortPredicate::Key(), SectorType{});
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayBvh<Value, SortPredicate, Allocator>::SizeType ArrayBvh<Value, SortPredicate, Allocator>::Erase(const ValueType& value) {
	_leaves.EraseUnordered(_leaves.Find(value)) != _leaves.End() ? 1u : 0u;
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename... ExtraArgs>
ETInlineHint ETForceInlineHint void ArrayBvh<Value, SortPredicate, Allocator>::Rebuild(ExtraArgs&... args) {
	_sort.Sort(args..., _leaves.Begin<Values>(), _leaves.End<Values>(), _leaves.Begin<Keys>());
	_sectors = _sort.Split(args..., _leaves.Begin<Values>(), _leaves.End<Values>(), _leaves.Begin<Sectors>());
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void ArrayBvh<Value, SortPredicate, Allocator>::Clear() ETNoexceptHint {
	_leaves.Clear();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void Swap(ArrayBvh<Value, Allocator>& lhs, ArrayBvh<Value, Allocator>& rhs) ETNoexceptHint {
	Swap(lhs._leaves, rhs._leaves);
	Swap(lhs._sort, rhs._sort);
}

} // namespace Eldritch2
