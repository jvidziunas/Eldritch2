/*==================================================================*\
  IntrusiveList.inl
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

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::Find(ConstIterator searchHint, UnaryPredicate condition) const {
	return eastl::find_if(searchHint, _container.end(), condition);
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::Find(Iterator searchHint, UnaryPredicate condition) {
	return eastl::find_if(searchHint, _container.end(), condition);
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::Find(UnaryPredicate condition) const {
	return Find(_container.begin(), condition);
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::Find(UnaryPredicate condition) {
	return Find(_container.begin(), condition);
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint void IntrusiveLinkedList<Value>::EraseIf(UnaryPredicate condition) {
	EraseAndDisposeIf(condition, [](Reference /*unused*/) {});
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate, typename Disposer>
ETInlineHint void IntrusiveLinkedList<Value>::EraseAndDisposeIf(UnaryPredicate condition, Disposer disposer) {
	for (Iterator element(_container.begin()), end(_container.end()); element != end;) {
		if (condition(*element)) {
			ValueType& object(*element);

			element = _container.erase(element);
			disposer(object);

			continue;
		}

		++element;
	}
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::ConstBegin() const {
	return _container.cbegin();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::ConstEnd() const {
	return _container.cend();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::IteratorTo(ConstReference element) const {
	return _container.locate(element);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::IteratorTo(Reference element) {
	return _container.locate(element);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::ConstReference IntrusiveLinkedList<Value>::Front() const {
	return _container.front();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::Reference IntrusiveLinkedList<Value>::Front() {
	return _container.front();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::ConstReference IntrusiveLinkedList<Value>::Back() const {
	return _container.back();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::Reference IntrusiveLinkedList<Value>::Back() {
	return _container.back();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint void IntrusiveLinkedList<Value>::Prepend(Reference value) {
	_container.push_front(value);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint void IntrusiveLinkedList<Value>::PopFront() {
	_container.pop_front();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint void IntrusiveLinkedList<Value>::Append(Reference value) {
	_container.push_back(value);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint void IntrusiveLinkedList<Value>::Pop() {
	_container.pop_back();
}

// ---------------------------------------------------

template <typename Value>
template <typename Disposer>
ETInlineHint void IntrusiveLinkedList<Value>::PopFrontAndDispose(Disposer disposer) {
	ValueType& element(_container.front());

	_container.pop_front();

	disposer(element);
}

// ---------------------------------------------------

template <typename Value>
template <typename Disposer>
ETInlineHint void IntrusiveLinkedList<Value>::PopAndDispose(Disposer disposer) {
	ValueType& element(_container.back());

	_container.pop_back();

	disposer(element);
}

// ---------------------------------------------------

template <typename Value>
template <typename Disposer, typename ElementCloner>
ETInlineHint void IntrusiveLinkedList<Value>::CloneFrom(const IntrusiveLinkedList<Value>& list, Disposer disposer, ElementCloner cloner) {
	ClearAndDispose(disposer);

	for (const ValueType& element : list._container) {
		_container.push_back(cloner(element));
	}
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::Insert(Iterator where, Reference value) {
	return _container.insert(where, value);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Value>
template <typename Disposer>
ETInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::EraseAndDispose(Iterator where, Disposer disposer) {
	ValueType& element(*where);
	where = _container.erase(where);
	disposer(element);

	return where;
}

// ---------------------------------------------------

template <typename Value>
template <typename Disposer>
ETInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::EraseAndDispose(Iterator begin, Iterator end, Disposer disposer) {
	while (begin != end) {
		begin = EraseAndDispose(begin, disposer);
	}

	return begin;
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint void IntrusiveLinkedList<Value>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Value>
template <typename Disposer>
ETInlineHint void IntrusiveLinkedList<Value>::ClearAndDispose(Disposer disposer) {
	while (!_container.empty()) {
		PopFrontAndDispose(disposer);
	}
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint typename IntrusiveLinkedList<Value>::SizeType IntrusiveLinkedList<Value>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint bool IntrusiveLinkedList<Value>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint IntrusiveLinkedList<Value>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint void Swap(IntrusiveLinkedList<Value>& lhs, IntrusiveLinkedList<Value>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
