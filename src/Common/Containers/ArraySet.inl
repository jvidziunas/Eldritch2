/*==================================================================*\
  ArraySet.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Algorithms.hpp>
//------------------------------------------------------------------//
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ArraySet<Value, SortPredicate, Allocator>::ArraySet(
	SizeType             capacityHint,
	const SorterType&    sortPredicate,
	const AllocatorType& allocator) :
	_container(capacityHint, sortPredicate, allocator) {
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename InputIterator>
ETInlineHint ArraySet<Value, SortPredicate, Allocator>::ArraySet(
	InputIterator        begin,
	InputIterator        end,
	const SorterType&    orderingPredicate,
	const AllocatorType& allocator) :
	_container(begin, end, orderingPredicate, allocator) {
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <class /*SFINAE*/>
ETInlineHint ArraySet<Value, SortPredicate, Allocator>::ArraySet(
	const ArraySet<Value, SortPredicate, Allocator>& set,
	const AllocatorType&                             allocator) :
	_container(set.GetOrderingPredicate(), allocator) {
	_container.insert(set.Begin(), set.End());
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ArraySet<Value, SortPredicate, Allocator>::ArraySet(const AllocatorType& allocator) :
	_container(allocator) {}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ArraySet<Value, SortPredicate, Allocator>::ArraySet(
	ArraySet<Value, SortPredicate, Allocator>&& set) :
	_container(eastl::move(set._container)) {
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::LowerBound(const ValueType& value) const {
	return _container.lower_bound(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::LowerBound(const ValueType& value) {
	return _container.lower_bound(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::UpperBound(const ValueType& value) const {
	return _container.upper_bound(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::UpperBound(const ValueType& value) {
	return _container.upper_bound(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename AlternateValue, typename BinaryPredicate>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::Find(const AlternateValue& value, BinaryPredicate sortPredicate) const {
	return _container.find_as(value, sortPredicate);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename AlternateValue, typename BinaryPredicate>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::Find(const AlternateValue& value, BinaryPredicate sortPredicate) {
	return _container.find_as(value, sortPredicate);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::Find(const ValueType& value) const {
	return _container.find(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::Find(const ValueType& value) {
	return _container.find(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename AlternateValue, typename BinaryPredicate>
ETInlineHint bool ArraySet<Value, SortPredicate, Allocator>::Contains(const AlternateValue& value, BinaryPredicate comparisonPredicate) const {
	return _container.find(value, comparisonPredicate) != _container.end();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint bool ArraySet<Value, SortPredicate, Allocator>::Contains(const ValueType& value) const {
	return _container.find(value) != _container.end();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename Predicate>
ETInlineHint void ArraySet<Value, SortPredicate, Allocator>::RemoveIf(Predicate predicate) {
	for (auto iterator(_container.begin()); iterator != _container.end();) {
		if (predicate(*iterator)) {
			iterator = _container.erase(iterator);
			continue;
		}

		++iterator;
	}
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::ConstBegin() const {
	return _container.cbegin();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::ConstEnd() const {
	return _container.cend();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstPointer ArraySet<Value, SortPredicate, Allocator>::GetData() const {
	return _container.data();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Pointer ArraySet<Value, SortPredicate, Allocator>::GetData() {
	return _container.data();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename... Arguments>
ETInlineHint Pair<typename ArraySet<Value, SortPredicate, Allocator>::Iterator, bool> ArraySet<Value, SortPredicate, Allocator>::Emplace(Arguments&&... arguments) {
	return _container.emplace(eastl::forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <class /*SFINAE*/>
ETInlineHint Pair<typename ArraySet<Value, SortPredicate, Allocator>::Iterator, bool> ArraySet<Value, SortPredicate, Allocator>::Insert(const ValueType& value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint Pair<typename ArraySet<Value, SortPredicate, Allocator>::Iterator, bool> ArraySet<Value, SortPredicate, Allocator>::Insert(ValueType&& value) {
	return _container.insert(eastl::forward<ValueType>(value));
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::SizeType ArraySet<Value, SortPredicate, Allocator>::Erase(const ValueType& value) {
	return _container.erase(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::Erase(Iterator beginPosition, Iterator endPosition) {
	return _container.erase(beginPosition, endPosition);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::Erase(Iterator position) {
	return _container.erase(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint void ArraySet<Value, SortPredicate, Allocator>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::SizeType ArraySet<Value, SortPredicate, Allocator>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint bool ArraySet<Value, SortPredicate, Allocator>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ArraySet<Value, SortPredicate, Allocator>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArraySet<Value, SortPredicate, Allocator>::SizeType ArraySet<Value, SortPredicate, Allocator>::GetCapacity() const {
	return _container.capacity();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint void ArraySet<Value, SortPredicate, Allocator>::SetCapacity(SizeType capacity) {
	_container.set_capacity(capacity);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint void ArraySet<Value, SortPredicate, Allocator>::Reserve(SizeType sizeHint) {
	_container.reserve(sizeHint);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename const ArraySet<Value, SortPredicate, Allocator>::SorterType& ArraySet<Value, SortPredicate, Allocator>::GetSorter() const {
	return _container.key_comp();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename const ArraySet<Value, SortPredicate, Allocator>::AllocatorType& ArraySet<Value, SortPredicate, Allocator>::GetAllocator() const {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint void Swap(ArraySet<Value, SortPredicate, Allocator>& set0, ArraySet<Value, SortPredicate, Allocator>& set1) {
	eastl::swap(set0._container, set1._container);
}

} // namespace Eldritch2
