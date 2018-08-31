/*==================================================================*\
  ArrayMap.inl
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

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename InputIterator>
ETInlineHint ETForceInlineHint ArrayMap<Key, Value, SortPredicate, Allocator>::ArrayMap(const AllocatorType& allocator, const SortPredicateType& sort, InputIterator begin, InputIterator end) :
	_container(begin, end, sort, allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint ArrayMap<Key, Value, SortPredicate, Allocator>::ArrayMap(const AllocatorType& allocator, const SortPredicateType& sort, std::initializer_list<ValueType> map) :
	_container(map, sort, allocator) {}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint ArrayMap<Key, Value, SortPredicate, Allocator>::ArrayMap(const AllocatorType& allocator, const SortPredicateType& sort, SizeType capacity) :
	_container(sort, allocator) {
	_container.reserve(capacity);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::LowerBound(const KeyType& key) const {
	return _container.lower_bound(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::LowerBound(const KeyType& key) {
	return _container.lower_bound(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::UpperBound(const KeyType& key) const {
	return _container.upper_bound(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::UpperBound(const KeyType& key) {
	return _container.upper_bound(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename Key2, typename SortPredicate2>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::Find(const Key2& key, SortPredicate2 sort) const {
	return _container.find_as<Key2, SortPredicate2>(key, eastl::move(sort));
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::Find(const KeyType& key) const {
	return _container.find(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename Key2, typename SortPredicate2>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::Find(const Key2& key, SortPredicate2 sort) {
	return _container.find_as<Key2, SortPredicate2>(key, eastl::move(sort));
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::Find(const KeyType& key) {
	return _container.find(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename Key2, typename SortPredicate2>
ETInlineHint ETForceInlineHint bool ArrayMap<Key, Value, SortPredicate, Allocator>::Contains(const Key2& key, SortPredicate2 sort) const {
	return _container.find_as<Key2, SortPredicate2>(key, eastl::move(sort)) != _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint bool ArrayMap<Key, Value, SortPredicate, Allocator>::Contains(const KeyType& key) const {
	return _container.find(key) != _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstReverseIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ConstReverseBegin() const {
	return _container.crbegin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstReverseIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ConstReverseEnd() const {
	return _container.crend();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstReverseIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ReverseBegin() const {
	return _container.rbegin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ReverseIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ReverseBegin() {
	return _container.rbegin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstReverseIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ReverseEnd() const {
	return _container.rend();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ReverseIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ReverseEnd() {
	return _container.rend();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ConstBegin() const {
	return _container.cbegin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ConstEnd() const {
	return _container.cend();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::MappedType& ArrayMap<Key, Value, SortPredicate, Allocator>::operator[](const KeyType& key) {
	return _container[key];
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::MappedType& ArrayMap<Key, Value, SortPredicate, Allocator>::operator[](KeyType&& key) {
	return _container[eastl::move(key)];
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint Pair<typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, SortPredicate, Allocator>::Insert(const ValueType& value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint Pair<typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, SortPredicate, Allocator>::Insert(ValueType&& value) {
	return _container.insert(eastl::move(value));
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint Pair<typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, SortPredicate, Allocator>::TryEmplace(const KeyType& key, Arguments&&... arguments) {
	Iterator where(_container.find(key));
	if (where != _container.end()) {
		return { where, false };
	}

	return _container.emplace(key, MappedType(eastl::forward<Arguments>(arguments)...));
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint Pair<typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, SortPredicate, Allocator>::TryEmplace(KeyType&& key, Arguments&&... arguments) {
	Iterator where(_container.find(key));
	if (where != _container.end()) {
		return { where, false };
	}

	return _container.emplace(eastl::move(key), MappedType(eastl::forward<Arguments>(arguments)...));
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint Pair<typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, SortPredicate, Allocator>::Emplace(ConstIterator where, Arguments&&... arguments) {
	return _container.emplace(where, eastl::forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename... Args>
ETInlineHint ETForceInlineHint Pair<typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, SortPredicate, Allocator>::Emplace(Args&&... args) {
	return _container.emplace(eastl::forward<Args>(args)...);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::SizeType ArrayMap<Key, Value, SortPredicate, Allocator>::Erase(const KeyType& key) {
	return _container.erase(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void ArrayMap<Key, Value, SortPredicate, Allocator>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename const ArrayMap<Key, Value, SortPredicate, Allocator>::SortPredicateType& ArrayMap<Key, Value, SortPredicate, Allocator>::GetSortPredicate() const {
	return _container.key_comp();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::SortPredicateType& ArrayMap<Key, Value, SortPredicate, Allocator>::GetSortPredicate() {
	return _container.key_comp();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void ArrayMap<Key, Value, SortPredicate, Allocator>::SetCapacity(SizeType sizeInElements) {
	_container.set_capacity(sizeInElements);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void ArrayMap<Key, Value, SortPredicate, Allocator>::Reserve(SizeType sizeInElements) {
	_container.reserve(sizeInElements);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::SizeType ArrayMap<Key, Value, SortPredicate, Allocator>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint bool ArrayMap<Key, Value, SortPredicate, Allocator>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint ArrayMap<Key, Value, SortPredicate, Allocator>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint typename const ArrayMap<Key, Value, SortPredicate, Allocator>::AllocatorType& ArrayMap<Key, Value, SortPredicate, Allocator>::GetAllocator() {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ETForceInlineHint void Swap(ArrayMap<Key, Value, SortPredicate, Allocator>& lhs, ArrayMap<Key, Value, SortPredicate, Allocator>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
