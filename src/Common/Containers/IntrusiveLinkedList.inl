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
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::Find(ConstIterator searchHint, UnaryPredicate condition) const ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, ConstReference>()) {
	return Eldritch2::FindIf<ConstIterator, UnaryPredicate>(searchHint, _container.end(), Move(condition));
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::Find(Iterator searchHint, UnaryPredicate condition) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, Reference>()) {
	return Eldritch2::FindIf<Iterator, UnaryPredicate>(searchHint, _container.end(), Move(condition));
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::Find(UnaryPredicate condition) const ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, ConstReference>()) {
	return Eldritch2::Find<ConstIterator, UnaryPredicate>(_container.cbegin(), _container.cend(), Move(condition));
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::Find(UnaryPredicate condition) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, Reference>()) {
	return Eldritch2::Find<Iterator, UnaryPredicate>(_container.begin(), _container.end(), Move(condition));
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void IntrusiveLinkedList<Value>::EraseIf(UnaryPredicate condition) {
	this->EraseAndDisposeIf(condition, [](Reference /*unused*/) ETNoexceptHint {});
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate, typename UnaryPredicate2>
ETInlineHint ETForceInlineHint void IntrusiveLinkedList<Value>::EraseAndDisposeIf(UnaryPredicate condition, UnaryPredicate2 disposer) {
	for (Iterator element(_container.begin()), end(_container.end()); element != end;) {
		if (condition(*element)) {
			disposer(*element);
			element = _container.erase(element);

			continue;
		}

		++element;
	}
}

// ---------------------------------------------------

template <class Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::ConstBegin() const ETNoexceptHint {
	return _container.cbegin();
}

// ---------------------------------------------------

template <class Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::ConstEnd() const ETNoexceptHint {
	return _container.cend();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::Begin() const ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::Begin() ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::End() const ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::End() ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::ConstIterator IntrusiveLinkedList<Value>::IteratorTo(ConstReference element) const {
	return _container.locate(element);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::IteratorTo(Reference element) {
	return _container.locate(element);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::ConstReference IntrusiveLinkedList<Value>::Front() const {
	return _container.front();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::Reference IntrusiveLinkedList<Value>::Front() {
	return _container.front();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::ConstReference IntrusiveLinkedList<Value>::Back() const {
	return _container.back();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::Reference IntrusiveLinkedList<Value>::Back() {
	return _container.back();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint void IntrusiveLinkedList<Value>::Prepend(Reference value) ETNoexceptHint {
	_container.push_front(value);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint void IntrusiveLinkedList<Value>::PopFront() ETNoexceptHint {
	_container.pop_front();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint void IntrusiveLinkedList<Value>::Append(Reference value) ETNoexceptHint {
	_container.push_back(value);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint void IntrusiveLinkedList<Value>::Pop() ETNoexceptHint {
	_container.pop_back();
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void IntrusiveLinkedList<Value>::PopFrontAndDispose(UnaryPredicate disposer) {
	disposer(_container.front());
	_container.pop_front();
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void IntrusiveLinkedList<Value>::PopAndDispose(UnaryPredicate disposer) {
	disposer(_container.back());
	_container.pop_back();
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate, typename Cloner>
ETInlineHint ETForceInlineHint void IntrusiveLinkedList<Value>::CloneFrom(const IntrusiveLinkedList<Value>& list, UnaryPredicate disposer, Cloner cloner) {
	this->ClearAndDispose(disposer);
	for (ConstReference element : list._container) {
		_container.push_back(cloner(element));
	}
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::Insert(Iterator where, Reference value) {
	return _container.insert(where, value);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint void IntrusiveLinkedList<Value>::Erase(Reference where) {
	_container.remove(where);
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::EraseAndDispose(Iterator where, UnaryPredicate disposer) {
	Reference element(*where);
	where = _container.erase(where);
	disposer(element);

	return where;
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::Iterator IntrusiveLinkedList<Value>::EraseAndDispose(Iterator begin, Iterator end, UnaryPredicate disposer) {
	while (begin != end) {
		begin = this->EraseAndDispose(begin, disposer);
	}

	return begin;
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint void IntrusiveLinkedList<Value>::Clear() ETNoexceptHint {
	_container.clear();
}

// ---------------------------------------------------

template <typename Value>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void IntrusiveLinkedList<Value>::ClearAndDispose(UnaryPredicate disposer) {
	while (!_container.empty()) {
		this->PopFrontAndDispose(disposer);
	}
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint typename IntrusiveLinkedList<Value>::SizeType IntrusiveLinkedList<Value>::GetSize() const ETNoexceptHint {
	return _container.size();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint bool IntrusiveLinkedList<Value>::IsEmpty() const ETNoexceptHint {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint IntrusiveLinkedList<Value>::operator bool() const ETNoexceptHint {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Value>
ETInlineHint ETForceInlineHint void Swap(IntrusiveLinkedList<Value>& lhs, IntrusiveLinkedList<Value>& rhs) ETNoexceptHint {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
