/*==================================================================*\
  TreeMap.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Span.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint TreeMap<Key, Value, SortPredicate, Allocator>::TreeMap(const AllocatorType& allocator, const SortPredicate& sort) :
	_container(sort, allocator) {}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename InputIterator>
ETInlineHint ETForceInlineHint TreeMap<Key, Value, SortPredicate, Allocator>::TreeMap(const AllocatorType& allocator, const SortPredicate& sort, InputIterator begin, InputIterator end) :
	_container(begin, end, sort, allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint TreeMap<Key, Value, SortPredicate, Allocator>::TreeMap(const AllocatorType& allocator, const SortPredicate& sort, InitializerList<ValueType> values) :
	_container(values, sort, allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::Iterator TreeMap<Key, Value, SortPredicate, Allocator>::Find(const KeyType& key) {
	return _container.find(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::ConstIterator TreeMap<Key, Value, SortPredicate, Allocator>::Find(const KeyType& key) const {
	return _container.find(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void TreeMap<Key, Value, SortPredicate, Allocator>::EraseIf(UnaryPredicate condition) {
	for (auto element(_container.begin()), end(_container.end()); element != end;) {
		if (condition(*element)) {
			element = _container.erase(element);
		} else {
			++element;
		}
	}
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::Iterator TreeMap<Key, Value, SortPredicate, Allocator>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::ConstIterator TreeMap<Key, Value, SortPredicate, Allocator>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::ConstIterator TreeMap<Key, Value, SortPredicate, Allocator>::ConstBegin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::Iterator TreeMap<Key, Value, SortPredicate, Allocator>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::ConstIterator TreeMap<Key, Value, SortPredicate, Allocator>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::ConstIterator TreeMap<Key, Value, SortPredicate, Allocator>::ConstEnd() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::MappedType& TreeMap<Key, Value, SortPredicate, Allocator>::operator[](const KeyType& key) {
	return _container[key];
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint Pair<typename TreeMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> TreeMap<Key, Value, SortPredicate, Allocator>::Insert(const ValueType& value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::SizeType TreeMap<Key, Value, SortPredicate, Allocator>::Erase(const KeyType& key) {
	return _container.erase(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::Iterator TreeMap<Key, Value, SortPredicate, Allocator>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::Iterator TreeMap<Key, Value, SortPredicate, Allocator>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void TreeMap<Key, Value, SortPredicate, Allocator>::ClearAndDispose(UnaryPredicate disposer) {
	for (ValueType& value : _container) {
		disposer(value.second);
	}

	_container.clear();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void TreeMap<Key, Value, SortPredicate, Allocator>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeMap<Key, Value, SortPredicate, Allocator>::SizeType TreeMap<Key, Value, SortPredicate, Allocator>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint bool TreeMap<Key, Value, SortPredicate, Allocator>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint TreeMap<Key, Value, SortPredicate, Allocator>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename const TreeMap<Key, Value, SortPredicate, Allocator>::AllocatorType& TreeMap<Key, Value, SortPredicate, Allocator>::GetAllocator() const {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void Swap(TreeMap<Key, Value, SortPredicate, Allocator>& lhs, TreeMap<Key, Value, SortPredicate, Allocator>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
