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
#include <Common/Containers/Span.hpp>
//------------------------------------------------------------------//
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, class SortPredicate, class Allocator>
template <typename InputIterator>
ETInlineHint ETForceInlineHint TreeSet<Value, SortPredicate, Allocator>::TreeSet(const AllocatorType& allocator, const SortPredicate& sort, InputIterator begin, InputIterator end) :
	_container(begin, end, sort, allocator) {
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint TreeSet<Value, SortPredicate, Allocator>::TreeSet(const AllocatorType& allocator, const SortPredicate& sort, InitializerList<ValueType> values) :
	_container(values, sort, allocator) {
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint TreeSet<Value, SortPredicate, Allocator>::TreeSet(const AllocatorType& allocator, const SortPredicate& sort) :
	_container(sort, allocator) {
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::Find(const ValueType& value) {
	return _container.find(value);
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::Find(const ValueType& value) const {
	return _container.find(value);
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void TreeSet<Value, SortPredicate, Allocator>::RemoveIf(UnaryPredicate condition) {
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
ETInlineHint ETForceInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::ConstBegin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::ConstEnd() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint Pair<typename TreeSet<Value, SortPredicate, Allocator>::Iterator, bool> TreeSet<Value, SortPredicate, Allocator>::Insert(const ValueType& value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void TreeSet<Value, SortPredicate, Allocator>::ClearAndDispose(UnaryPredicate disposer) {
	for (const ValueType& value : _container) {
		/*	Use of const_cast is gross, but this is one of the suggested solutions in http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103 */
		disposer(const_cast<ValueType&>(value));
	}

	_container.clear();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void TreeSet<Value, SortPredicate, Allocator>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename TreeSet<Value, SortPredicate, Allocator>::SizeType TreeSet<Value, SortPredicate, Allocator>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint bool TreeSet<Value, SortPredicate, Allocator>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint TreeSet<Value, SortPredicate, Allocator>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename const TreeSet<Value, SortPredicate, Allocator>::AllocatorType& TreeSet<Value, SortPredicate, Allocator>::GetAllocator() const {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Value, class SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void Swap(TreeSet<Value, SortPredicate, Allocator>& lhs, TreeSet<Value, SortPredicate, Allocator>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
