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

template <typename Value, class Allocator>
ETInlineHint ETForceInlineHint ArrayBvh<Value, Allocator>::ArrayBvh(const AllocatorType& allocator) :
	_allocator(allocator),
	_leaves(ChildAllocator(_allocator, "Array BVH Leaf Allocator")),
	_splits(ChildAllocator(_allocator, "Array BVH Split Allocator")) {
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayBvh<Value, Allocator>::SplitConstIterator ArrayBvh<Value, Allocator>::ConstBegin() const ETNoexceptHint {
	return _splits.ConstBegin();
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayBvh<Value, Allocator>::SplitConstIterator ArrayBvh<Value, Allocator>::ConstEnd() const ETNoexceptHint {
	return _splits.ConstEnd();
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayBvh<Value, Allocator>::SplitConstIterator ArrayBvh<Value, Allocator>::Begin() const ETNoexceptHint {
	return _splits.Begin();
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayBvh<Value, Allocator>::SplitConstIterator ArrayBvh<Value, Allocator>::End() const ETNoexceptHint {
	return _splits.End();
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayBvh<Value, Allocator>::LeafConstIterator ArrayBvh<Value, Allocator>::Find(const ValueType& value) const ETNoexceptHint {
	return Find(_leaves.Begin(), _leaves.End(), [&value](typename LeafContainer::ConstReference leaf) { return eastl::get<Value&>(leaf) == value; });
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayBvh<Value, Allocator>::LeafIterator ArrayBvh<Value, Allocator>::Find(const ValueType& value) ETNoexceptHint {
	return Find(_leaves.Begin(), _leaves.End(), [&value](typename LeafContainer::Reference leaf) { return eastl::get<Value&>(leaf) == value; });
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint ETForceInlineHint void ArrayBvh<Value, Allocator>::Insert(const ValueType& value) {
	_values.Append(value);
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayBvh<Value, Allocator>::LeafIterator ArrayBvh<Value, Allocator>::Erase(const ValueType& value) {
	_leaves.EraseUnordered(Find(value));
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint ETForceInlineHint void ArrayBvh<Value, Allocator>::Clear() {
	_leaves.Clear();
	_splits.Clear();
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint ETForceInlineHint void Swap(ArrayBvh<Value, Allocator>& lhs, ArrayBvh<Value, Allocator>& rhs) {
	Swap(lhs._allocator, rhs._allocator);
	Swap(lhs._extractor, rhs._extractor);
	Swap(lhs._leaves, rhs._leaves);
	Swap(lhs._splits, rhs._splits);
}

} // namespace Eldritch2
