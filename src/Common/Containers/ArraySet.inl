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
ETInlineHint ETForceInlineHint ArraySet<Value, SortPredicate, Allocator>::ArraySet(const AllocatorType& allocator, const SorterType& sort, SizeType capacityHint) :
	_container(sort, allocator) {
	_container.reserve(capacityHint);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename InputIterator>
ETInlineHint ETForceInlineHint ArraySet<Value, SortPredicate, Allocator>::ArraySet(const AllocatorType& allocator, const SorterType& sort, InputIterator begin, InputIterator end) :
	_container(sort, allocator) {
	_container.insert(begin, end);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint ArraySet<Value, SortPredicate, Allocator>::ArraySet(const AllocatorType& allocator, const SorterType& sort, InitializerList<ValueType> values) :
	_container(values, sort, allocator) {}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint ArraySet<Value, SortPredicate, Allocator>::ArraySet(const AllocatorType& allocator, const ArraySet<Value, SortPredicate, Allocator>& set) :
	_container(set.GetOrderingPredicate(), allocator) {
	_container.insert(set.Begin(), set.End());
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::LowerBound(const ValueType& value) const {
	return _container.lower_bound(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::LowerBound(const ValueType& value) {
	return _container.lower_bound(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::UpperBound(const ValueType& value) const {
	return _container.upper_bound(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::UpperBound(const ValueType& value) {
	return _container.upper_bound(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename Value2, typename SortPredicate2>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::Find(const Value2& value, SortPredicate2 sort) const {
	return _container.find_as<Value2, SortPredicate2>(value, Move(sort));
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename Value2, typename SortPredicate2>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::Find(const Value2& value, SortPredicate2 sort) {
	return _container.find_as<Value2, SortPredicate2>(value, Move(sort));
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::Find(const ValueType& value) const {
	return _container.find(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::Find(const ValueType& value) {
	return _container.find(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename Value2, typename SortPredicate2>
ETInlineHint ETForceInlineHint bool ArraySet<Value, SortPredicate, Allocator>::Contains(const Value2& value, SortPredicate2 sort) const {
	return _container.find_as<Value2, SortPredicate2>(value, Move(sort)) != _container.end();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint bool ArraySet<Value, SortPredicate, Allocator>::Contains(const ValueType& value) const {
	return _container.find(value) != _container.end();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void ArraySet<Value, SortPredicate, Allocator>::RemoveIf(UnaryPredicate filter) {
	for (auto iterator(_container.begin()); iterator != _container.end();) {
		if (filter(*iterator)) {
			iterator = _container.erase(iterator);
			continue;
		}

		++iterator;
	}
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::ConstBegin() const {
	return _container.cbegin();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::ConstEnd() const {
	return _container.cend();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstIterator ArraySet<Value, SortPredicate, Allocator>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::ConstPointer ArraySet<Value, SortPredicate, Allocator>::GetData() const {
	return _container.data();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Pointer ArraySet<Value, SortPredicate, Allocator>::GetData() {
	return _container.data();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint Pair<typename ArraySet<Value, SortPredicate, Allocator>::Iterator, bool> ArraySet<Value, SortPredicate, Allocator>::Emplace(Arguments&&... arguments) {
	return _container.emplace(Forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint Pair<typename ArraySet<Value, SortPredicate, Allocator>::Iterator, bool> ArraySet<Value, SortPredicate, Allocator>::Insert(const ValueType& value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint Pair<typename ArraySet<Value, SortPredicate, Allocator>::Iterator, bool> ArraySet<Value, SortPredicate, Allocator>::Insert(ValueType&& value) {
	return _container.insert(Move(value));
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::SizeType ArraySet<Value, SortPredicate, Allocator>::Erase(const ValueType& value) {
	return _container.erase(value);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::Iterator ArraySet<Value, SortPredicate, Allocator>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void ArraySet<Value, SortPredicate, Allocator>::ClearAndDispose(UnaryPredicate disposer) {
	for (const ValueType& value : _container) {
		/*	Use of const_cast is gross, but this is one of the suggested solutions in http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103 */
		disposer(const_cast<ValueType&>(value));
	}

	_container.clear();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void ArraySet<Value, SortPredicate, Allocator>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::SizeType ArraySet<Value, SortPredicate, Allocator>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint bool ArraySet<Value, SortPredicate, Allocator>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint ArraySet<Value, SortPredicate, Allocator>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArraySet<Value, SortPredicate, Allocator>::SizeType ArraySet<Value, SortPredicate, Allocator>::GetCapacity() const {
	return _container.capacity();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void ArraySet<Value, SortPredicate, Allocator>::SetCapacity(SizeType capacity) {
	_container.set_capacity(capacity);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void ArraySet<Value, SortPredicate, Allocator>::Reserve(SizeType sizeHint) {
	_container.reserve(sizeHint);
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename const ArraySet<Value, SortPredicate, Allocator>::SorterType& ArraySet<Value, SortPredicate, Allocator>::GetSorter() const {
	return _container.key_comp();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename const ArraySet<Value, SortPredicate, Allocator>::AllocatorType& ArraySet<Value, SortPredicate, Allocator>::GetAllocator() const {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void Swap(ArraySet<Value, SortPredicate, Allocator>& lhs, ArraySet<Value, SortPredicate, Allocator>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
