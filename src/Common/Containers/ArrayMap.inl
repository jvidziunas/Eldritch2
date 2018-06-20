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
ETInlineHint ArrayMap<Key, Value, SortPredicate, Allocator>::ArrayMap(
	InputIterator            begin,
	InputIterator            end,
	const SortPredicateType& sortPredicate,
	const AllocatorType&     allocator) :
	_container(begin, end, sortPredicate, allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ArrayMap<Key, Value, SortPredicate, Allocator>::ArrayMap(
	const ArrayMap<Key, Value, SortPredicate, Allocator>& map) :
	_container(map._container) {
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ArrayMap<Key, Value, SortPredicate, Allocator>::ArrayMap(
	ArrayMap<Key, Value, SortPredicate, Allocator>&& map) :
	_container(eastl::move(map._container)) {
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ArrayMap<Key, Value, SortPredicate, Allocator>::ArrayMap(
	const AllocatorType& allocator) :
	_container(allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::LowerBound(const KeyType& key) const {
	return _container.lower_bound(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::LowerBound(const KeyType& key) {
	return _container.lower_bound(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::UpperBound(const KeyType& key) const {
	return _container.upper_bound(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::UpperBound(const KeyType& key) {
	return _container.upper_bound(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename AlternateKeyType, typename AlternateSortPredicate>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::Find(const AlternateKeyType& key, AlternateSortPredicate sortPredicate) const {
	return _container.find_as(key, sortPredicate);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::Find(const KeyType& key) const {
	return _container.find(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename AlternateKeyType, typename AlternateSortPredicate>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::Find(const AlternateKeyType& key, AlternateSortPredicate sortPredicate) {
	return _container.find_as(key, sortPredicate);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::Find(const KeyType& key) {
	return _container.find(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename AlternateKey, typename AlternateSortPredicate>
ETInlineHint bool ArrayMap<Key, Value, SortPredicate, Allocator>::ContainsKey(const AlternateKey& key, const AlternateSortPredicate& sortPredicate) const {
	return _container.find_as(key, sortPredicate) != _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint bool ArrayMap<Key, Value, SortPredicate, Allocator>::ContainsKey(const KeyType& key) const {
	return _container.find(key) != _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstReverseIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ConstReverseBegin() const {
	return _container.crbegin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstReverseIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ConstReverseEnd() const {
	return _container.crend();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstReverseIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ReverseBegin() const {
	return _container.rbegin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ReverseIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ReverseBegin() {
	return _container.rbegin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstReverseIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ReverseEnd() const {
	return _container.rend();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ReverseIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ReverseEnd() {
	return _container.rend();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ConstBegin() const {
	return _container.cbegin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::ConstEnd() const {
	return _container.cend();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, SortPredicate, Allocator>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint Pair<typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, SortPredicate, Allocator>::Insert(const ValueType& value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint Pair<typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, SortPredicate, Allocator>::Insert(ValueType&& value) {
	return _container.insert(eastl::move(value));
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename... Args>
ETInlineHint Pair<typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, SortPredicate, Allocator>::TryEmplace(const KeyType& key, Args&&... args) {
	Iterator position(_container.find(key));

	if (position != _container.end()) {
		return { position, false };
	}

	return _container.emplace(key, MappedType(eastl::forward<Args>(args)...));
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename... Args>
ETInlineHint Pair<typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, SortPredicate, Allocator>::TryEmplace(KeyType&& key, Args&&... args) {
	Iterator position(_container.find(key));

	if (position != _container.end()) {
		return { position, false };
	}

	return _container.emplace(eastl::move(key), ValueType(eastl::forward<Args>(args)...));
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename... Args>
ETInlineHint Pair<typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, SortPredicate, Allocator>::Emplace(ConstIterator position, Args&&... args) {
	return _container.emplace(position, eastl::forward<Args>(args)...);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
template <typename... Args>
ETInlineHint Pair<typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, SortPredicate, Allocator>::Emplace(Args&&... args) {
	return _container.emplace(eastl::forward<Args>(args)...);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::SizeType ArrayMap<Key, Value, SortPredicate, Allocator>::Erase(const KeyType& key) {
	return _container.erase(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::Erase(Iterator position) {
	return _container.erase(position);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::Iterator ArrayMap<Key, Value, SortPredicate, Allocator>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint void ArrayMap<Key, Value, SortPredicate, Allocator>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename const ArrayMap<Key, Value, SortPredicate, Allocator>::SortPredicateType& ArrayMap<Key, Value, SortPredicate, Allocator>::GetSortPredicate() const {
	return _container.key_comp();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::SortPredicateType& ArrayMap<Key, Value, SortPredicate, Allocator>::GetSortPredicate() {
	return _container.key_comp();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ArrayMap<Key, Value, SortPredicate, Allocator>& ArrayMap<Key, Value, SortPredicate, Allocator>::operator=(const ArrayMap<Key, Value, SortPredicate, Allocator>& other) {
	_container = other._container;
	return *this;
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ArrayMap<Key, Value, SortPredicate, Allocator>& ArrayMap<Key, Value, SortPredicate, Allocator>::operator=(ArrayMap<Key, Value, SortPredicate, Allocator>&& other) {
	_container = eastl::move(other._container);
	return *this;
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint void ArrayMap<Key, Value, SortPredicate, Allocator>::SetCapacity(SizeType sizeInElements) {
	_container.set_capacity(sizeInElements);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint void ArrayMap<Key, Value, SortPredicate, Allocator>::Reserve(SizeType sizeInElements) {
	_container.reserve(sizeInElements);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename ArrayMap<Key, Value, SortPredicate, Allocator>::SizeType ArrayMap<Key, Value, SortPredicate, Allocator>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint bool ArrayMap<Key, Value, SortPredicate, Allocator>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint ArrayMap<Key, Value, SortPredicate, Allocator>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint typename const ArrayMap<Key, Value, SortPredicate, Allocator>::AllocatorType& ArrayMap<Key, Value, SortPredicate, Allocator>::GetAllocator() {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename SortPredicate, class Allocator>
ETInlineHint void Swap(ArrayMap<Key, Value, SortPredicate, Allocator>& map0, ArrayMap<Key, Value, SortPredicate, Allocator>& map1) {
	eastl::swap(map0._container, map1._container);
}

} // namespace Eldritch2
