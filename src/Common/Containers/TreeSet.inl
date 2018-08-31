/*==================================================================*\
  TreeSet.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Range.hpp>
//------------------------------------------------------------------//
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, class SortPredicate, class Allocator>
template <typename InputIterator>
ETInlineHint TreeSet<Value, SortPredicate, Allocator>::TreeSet(const AllocatorType& allocator, const SortPredicate& sort, InputIterator begin, InputIterator end) :
	_container(begin, end, sort, allocator) {
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint TreeSet<Value, SortPredicate, Allocator>::TreeSet(const AllocatorType& allocator, const SortPredicate& sort, std::initializer_list<ValueType> set) :
	_container(set, sort, allocator) {
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint TreeSet<Value, SortPredicate, Allocator>::TreeSet(const AllocatorType& allocator, const SortPredicate& sort) :
	_container(sort, allocator) {
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::Find(const ValueType& value) {
	return _container.find(value);
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::Find(const ValueType& value) const {
	return _container.find(value);
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
template <typename UnaryPredicate>
ETInlineHint void TreeSet<Value, SortPredicate, Allocator>::RemoveIf(UnaryPredicate condition) {
	for (Iterator element(_container.begin()), end(_container.end()); element != end;) {
		if (condition(*element)) {
			element = _container.erase(element);
		} else {
			++element;
		}
	}
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::ConstBegin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::ConstEnd() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint Pair<typename TreeSet<Value, SortPredicate, Allocator>::Iterator, bool> TreeSet<Value, SortPredicate, Allocator>::Insert(const ValueType& value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint void TreeSet<Value, SortPredicate, Allocator>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::SizeType TreeSet<Value, SortPredicate, Allocator>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint bool TreeSet<Value, SortPredicate, Allocator>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint TreeSet<Value, SortPredicate, Allocator>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint typename const TreeSet<Value, SortPredicate, Allocator>::AllocatorType& TreeSet<Value, SortPredicate, Allocator>::GetAllocator() const {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint void Swap(TreeSet<Value, SortPredicate, Allocator>& lhs, TreeSet<Value, SortPredicate, Allocator>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
