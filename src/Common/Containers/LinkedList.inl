/*==================================================================*\
  LinkedList.inl
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

namespace Eldritch2 {

template <typename Value, typename Allocator>
ETInlineHint LinkedList<Value, Allocator>::LinkedList(
	const AllocatorType& allocator) :
	_container(allocator) {
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename InputIterator>
ETInlineHint LinkedList<Value, Allocator>::LinkedList(
	InputIterator        first,
	InputIterator        last,
	const AllocatorType& allocator) :
	_container(first, last, allocator) {
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <class /*SFINAE*/>
ETInlineHint LinkedList<Value, Allocator>::LinkedList(
	const LinkedList<Value, Allocator>& list,
	const AllocatorType&                allocator) :
	_container(list._container, allocator) {
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::ConstIterator LinkedList<Value, Allocator>::Find(ConstReference itemTemplate, ConstIterator searchHint) const {
	return FindIf(searchHint, _container.end(), itemTemplate, IsEquivalent());
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::Find(ConstReference itemTemplate, Iterator searchHint) {
	return FindIf(searchHint, _container.end(), itemTemplate, IsEquivalent());
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename ItemPredicate>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::EraseIf(ItemPredicate itemPredicate) {
	return RemoveIf(_container.begin(), _container.end(), itemPredicate);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::ConstIterator LinkedList<Value, Allocator>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::ConstIterator LinkedList<Value, Allocator>::ConstBegin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::ConstIterator LinkedList<Value, Allocator>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::ConstIterator LinkedList<Value, Allocator>::ConstEnd() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::Reference LinkedList<Value, Allocator>::Front() {
	return _container.front();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::ConstReference LinkedList<Value, Allocator>::Front() const {
	return _container.front();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <class /*SFINAE*/>
ETInlineHint void LinkedList<Value, Allocator>::Prepend(ConstReference value) {
	_container.push_front(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename... ElementConstructorArguments>
ETInlineHint void LinkedList<Value, Allocator>::EmplaceFront(ElementConstructorArguments&&... elementConstructorArguments) {
	_container.emplace_front(eastl::forward<ElementConstructorArguments>(elemenetConstructorArguments)...);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void LinkedList<Value, Allocator>::PopFront() {
	_container.pop_front();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::Reference LinkedList<Value, Allocator>::Back() {
	return _container.back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::ConstReference LinkedList<Value, Allocator>::Back() const {
	return _container.back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <class /*SFINAE*/>
ETInlineHint void LinkedList<Value, Allocator>::Append(ConstReference value) {
	_container.push_back(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename... ElementConstructorArguments>
ETInlineHint void LinkedList<Value, Allocator>::EmplaceBack(ElementConstructorArguments&&... elementConstructorArguments) {
	_container.emplace_back(eastl::forward<ElementConstructorArguments>(elementConstructorArguments)...);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void LinkedList<Value, Allocator>::Pop() {
	_container.pop_back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <class /*SFINAE*/>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::Insert(Iterator location, ConstReference value) {
	return _container.insert(location, value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename... ElementConstructorArguments>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::Emplace(Iterator location, ElementConstructorArguments&&... elementConstructorArguments) {
	return _container.emplace(location, eastl::forward<ElementConstructorArguments>(elementConstructorArguments)...);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::Erase(Iterator first, Iterator last) {
	return _container.erase(first, last);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::Erase(Iterator location) {
	return _container.erase(location);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void LinkedList<Value, Allocator>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <class /*SFINAE*/>
ETInlineHint LinkedList<Value, Allocator>& LinkedList<Value, Allocator>::operator=(const LinkedList<Value, Allocator>& list) {
	_container = list._container;

	return *this;
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint LinkedList<Value, Allocator>& LinkedList<Value, Allocator>::operator=(LinkedList<Value, Allocator>&& list) {
	_container = eastl::move(list._container);

	return *this;
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint bool LinkedList<Value, Allocator>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint LinkedList<Value, Allocator>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::SizeType LinkedList<Value, Allocator>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename const LinkedList<Value, Allocator>::AllocatorType& LinkedList<Value, Allocator>::GetAllocator() const {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void Swap(LinkedList<Value, Allocator>& list0, LinkedList<Value, Allocator>& list1) {
	eastl::swap(list0._container, list1._container);
}

} // namespace Eldritch2
