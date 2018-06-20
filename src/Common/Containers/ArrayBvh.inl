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
ETInlineHint ArrayBvh<Value, Allocator>::ArrayBvh(
	const AllocatorType& allocator) :
	_allocator(allocator),
	_leaves(ChildAllocator{ _allocator, "Array BVH Leaf Allocator" }),
	_splits(ChildAllocator{ _allocator, "Array BVH Split Allocator" }) {
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint typename ArrayBvh<Value, Allocator>::ConstLeafIterator ArrayBvh<Value, Allocator>::ConstBegin() const {
	return _leaves.Begin();
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint typename ArrayBvh<Value, Allocator>::ConstLeafIterator ArrayBvh<Value, Allocator>::ConstEnd() const {
	return _leaves.End();
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint typename ArrayBvh<Value, Allocator>::ConstLeafIterator ArrayBvh<Value, Allocator>::Begin() const {
	return _leaves.Begin();
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint typename ArrayBvh<Value, Allocator>::LeafIterator ArrayBvh<Value, Allocator>::Begin() {
	return _leaves.Begin();
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint typename ArrayBvh<Value, Allocator>::ConstLeafIterator ArrayBvh<Value, Allocator>::End() const {
	return _leaves.End();
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint typename ArrayBvh<Value, Allocator>::LeafIterator ArrayBvh<Value, Allocator>::End() {
	return _leaves.End();
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint void ArrayBvh<Value, Allocator>::Insert(const ValueType& value) {
	_values.Append(value);
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint typename ArrayBvh<Value, Allocator>::LeafIterator ArrayBvh<Value, Allocator>::Find(const ValueType& value) {
	return eastl::find_if(_leaves.Begin(), _leaves.End(), [&value](const LeafType& leaf) {
		return leaf.second == &value;
	});
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint typename ArrayBvh<Value, Allocator>::LeafIterator ArrayBvh<Value, Allocator>::Erase(LeafIterator position) {
	_leaves.Erase(position, Eldritch2::UnorderedSemantics);
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint void ArrayBvh<Value, Allocator>::Clear() {
	_leaves.Clear();
	_splits.Clear();
}

// ---------------------------------------------------

template <typename Value, class Allocator>
ETInlineHint void Swap(ArrayBvh<Value, Allocator>& lhs, ArrayBvh<Value, Allocator>& rhs) {
	Swap(lhs._allocator, rhs._allocator);
	Swap(lhs._extractor, rhs._extractor);
	Swap(lhs._leaves, rhs._leaves);
	Swap(lhs._splits, rhs._splits);
}

} // namespace Eldritch2
