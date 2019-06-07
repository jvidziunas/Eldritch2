/*==================================================================*\
  IntrusiveForwardList.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <class Value>
template <typename InputIterator>
ETInlineHint IntrusiveForwardList<Value>::IntrusiveForwardList(InputIterator begin, InputIterator end) :
	_container(begin, end) {
}

// ---------------------------------------------------

template <class Value>
template <typename UnaryPredicate>
ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::FindIf(UnaryPredicate predicate, Iterator where) {
	return FindIf(where, _container.end(), predicate);
}

// ---------------------------------------------------

template <class Value>
template <typename UnaryPredicate>
ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::FindIf(UnaryPredicate condition) {
	return FindIf(_container.begin(), _container.end(), condition);
}

// ---------------------------------------------------

template <class Value>
template <typename UnaryPredicate>
ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::FindIf(UnaryPredicate condition, ConstIterator where) const {
	return FindIf(where, _container.end(), condition);
}

// ---------------------------------------------------

template <class Value>
template <typename UnaryPredicate>
ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::FindIf(UnaryPredicate condition) const {
	return FindIf(_container.begin(), _container.end(), condition);
}

// ---------------------------------------------------

template <class Value>
template <typename UnaryPredicate>
ETInlineHint void IntrusiveForwardList<Value>::EraseIf(UnaryPredicate condition) {
	EraseAndDisposeIf(condition, [](Reference /*unused*/) {});
}

// ---------------------------------------------------

template <class Value>
template <typename UnaryPredicate, typename UnaryPredicate2>
ETInlineHint void IntrusiveForwardList<Value>::EraseAndDisposeIf(UnaryPredicate condition, UnaryPredicate2 disposer) {
	for (auto current(Begin()), end(End()); current != end;) {
		if (condition(*current)) {
			current = EraseAndDispose(current, disposer);
			continue;
		}

		++current;
	}
}

// ---------------------------------------------------

template <class Value>
template <typename UnaryPredicate>
ETInlineHint void IntrusiveForwardList<Value>::Sort(UnaryPredicate sort) {
	_container.sort(sort);
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::ConstBegin() const {
	return _container.cbegin();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::ConstEnd() const {
	return _container.cend();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::GetIteratorTo(Reference element) {
	return _container.locate(element);
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::GetIteratorTo(ConstReference element) const {
	return _container.locate(element);
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::ConstReference IntrusiveForwardList<Value>::Front() const {
	return _container.front();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::Reference IntrusiveForwardList<Value>::Front() {
	return _container.front();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint void IntrusiveForwardList<Value>::Prepend(Reference item) {
	_container.push_front(item);
}

// ---------------------------------------------------

template <class Value>
template <typename UnaryPredicate>
ETInlineHint void IntrusiveForwardList<Value>::PopFrontAndDispose(UnaryPredicate disposer) {
	auto& front(_container.front());
	_container.pop_front();

	disposer(front);
}

// ---------------------------------------------------

template <class Value>
ETInlineHint void IntrusiveForwardList<Value>::PopFront() {
	_container.pop_front();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Insert(Iterator where, Reference item) {
	return _container.insert(where, item);
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::InsertAfter(Iterator where, Reference item) {
	return _container.insert_after(where, item);
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::EraseAfter(Iterator beforeBegin, Iterator end) {
	return _container.erase_after(beforeBegin, end);
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::EraseAfter(Iterator where) {
	return _container.erase_after(where);
}

// ---------------------------------------------------

template <class Value>
template <typename UnaryPredicate>
ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::EraseAndDispose(Iterator where, UnaryPredicate disposer) {
	auto& element(*where);
	auto  result(_container.erase(where));

	disposer(element);

	return result;
}

// ---------------------------------------------------

template <class Value>
template <typename UnaryPredicate>
ETInlineHint void IntrusiveForwardList<Value>::EraseAndDispose(Iterator begin, Iterator end, UnaryPredicate disposer) {
	if (begin != end) {
		while (begin.next() != end.node()) {
			EraseAfterAndDispose(first, disposer);
		}

		EraseAfterAndDispose(_container.previous(first), disposer);
	}

	return begin;
}

// ---------------------------------------------------

template <class Value>
ETInlineHint void IntrusiveForwardList<Value>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <class Value>
template <typename UnaryPredicate>
ETInlineHint void IntrusiveForwardList<Value>::ClearAndDispose(UnaryPredicate disposer) {
	while (!_container.empty()) {
		PopFrontAndDispose(disposer);
	}
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveForwardList<Value>::SizeType IntrusiveForwardList<Value>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint bool IntrusiveForwardList<Value>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint IntrusiveForwardList<Value>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint void Swap(IntrusiveForwardList<Value>& lhs, IntrusiveForwardList<Value>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
