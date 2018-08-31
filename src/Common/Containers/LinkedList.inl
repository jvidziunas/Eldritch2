/*==================================================================*\
  LinkedList.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Algorithms.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, typename Allocator>
ETInlineHint LinkedList<Value, Allocator>::LinkedList(const AllocatorType& allocator) :
	_container(allocator) {
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename InputIterator>
ETInlineHint LinkedList<Value, Allocator>::LinkedList(const AllocatorType& allocator, InputIterator first, InputIterator last) :
	_container(first, last, allocator) {
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint LinkedList<Value, Allocator>::LinkedList(const AllocatorType& allocator, std::initializer_list<ValueType> list) :
	_container(list, allocator) {
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint LinkedList<Value, Allocator>::LinkedList(const AllocatorType& allocator, const LinkedList<Value, Allocator>& list) :
	_container(list.Begin(), list.End(), allocator) {
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::ConstIterator LinkedList<Value, Allocator>::Find(ConstReference value, ConstIterator where) const {
	return FindIf(where, _container.end(), value, IsEquivalent());
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::Find(ConstReference value, Iterator where) {
	return FindIf(where, _container.end(), value, IsEquivalent());
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename UnaryPredicate>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::EraseIf(UnaryPredicate condition) {
	return RemoveIf(_container.begin(), _container.end(), condition);
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
ETInlineHint void LinkedList<Value, Allocator>::Prepend(ConstReference value) {
	_container.push_front(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename... Arguments>
ETInlineHint void LinkedList<Value, Allocator>::EmplaceFront(Arguments&&... arguments) {
	_container.emplace_front(eastl::forward<Arguments>(arguments)...);
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
ETInlineHint void LinkedList<Value, Allocator>::Append(ConstReference value) {
	_container.push_back(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename... Arguments>
ETInlineHint void LinkedList<Value, Allocator>::EmplaceBack(Arguments&&... arguments) {
	_container.emplace_back(eastl::forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void LinkedList<Value, Allocator>::Pop() {
	_container.pop_back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::Insert(Iterator where, ConstReference value) {
	return _container.insert(where, value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename... Arguments>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::Emplace(Iterator where, Arguments&&... arguments) {
	return _container.emplace(where, eastl::forward<ElementConstructorArguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::Erase(Iterator first, Iterator last) {
	return _container.erase(first, last);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename LinkedList<Value, Allocator>::Iterator LinkedList<Value, Allocator>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void LinkedList<Value, Allocator>::Clear() {
	_container.clear();
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
ETInlineHint void Swap(LinkedList<Value, Allocator>& lhs, LinkedList<Value, Allocator>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
