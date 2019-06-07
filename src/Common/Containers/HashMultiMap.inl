/*==================================================================*\
  HashMultiMap.inl
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

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashMultiMap(
	const AllocatorType&         allocator,
	SizeType                     bucketCount,
	const HashPredicateType&     hash,
	const EqualityPredicateType& equal) :
	_container(bucketCount, hash, equal, allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename InputIterator>
ETInlineHint ETForceInlineHint HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashMultiMap(
	const AllocatorType&         allocator,
	SizeType                     bucketCount,
	const HashPredicateType&     hash,
	const EqualityPredicateType& equal,
	InputIterator                begin,
	InputIterator                end) :
	_container(begin, end, bucketCount, hash, equal, allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashMultiMap(
	const AllocatorType&         allocator,
	SizeType                     bucketCount,
	const HashPredicateType&     hash,
	const EqualityPredicateType& equal,
	InitializerList<ValueType>   values) :
	_container(values, bucketCount, hash, equal, allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashMultiMap(const AllocatorType& allocator, const HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>& map) :
	_container(map.Begin(), map.End(), 0u, map.GetHash(), map.GetEqualityPredicate(), allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint Span<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Slice(
	const Key2&               key,
	const HashPredicate2&     hash,
	const EqualityPredicate2& equal) const {
	const auto resultPair(_container.equal_range(key, hash, equal));
	return { resultPair.first, resultPair.second };
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint Span<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Slice(
	const Key2&               key,
	const HashPredicate2&     hash,
	const EqualityPredicate2& equal) {
	const auto resultPair(_container.equal_range(key, hash, equal));
	return { resultPair.first, resultPair.second };
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint Span<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Slice(const KeyType& key) const {
	const auto resultPair(_container.equal_range(key));
	return { resultPair.first, resultPair.second };
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint Span<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Slice(const KeyType& key) {
	const auto resultPair(_container.equal_range(key));
	return { resultPair.first, resultPair.second };
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::EraseIf(UnaryPredicate condition) {
	for (Iterator element(_container.begin()), end(_container.end()); element != end;) {
		if (condition(*element)) {
			element = _container.erase(element);
		} else {
			++element;
		}
	}
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstBegin(SizeType bucketIndex) const {
	return _container.begin(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstBegin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstEnd(SizeType bucketIndex) const {
	return _container.end(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstEnd() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Begin(SizeType bucketIndex) const {
	return _container.begin(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::LocalIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Begin(SizeType bucketIndex) {
	return _container.begin(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::End(SizeType bucketIndex) const {
	return _container.end(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::LocalIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::End(SizeType bucketIndex) {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint Pair<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator, bool> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Insert(const ValueType& value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint Pair<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator, bool> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Insert(ValueType&& value) {
	return _container.insert(Move(value));
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename... Arguments>
ETInlineHint ETForceInlineHint Pair<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator, bool> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Emplace(Arguments&&... arguments) {
	return _container.emplace(Forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::SizeType HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Erase(const KeyType& key) {
	return _container.erase(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint void HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ClearAndDispose(UnaryPredicate disposer) {
	for (ValueType& value : _container) {
		disposer(value.second);
	}

	_container.clear();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::SizeType HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::GetSize() const ETNoexceptHint {
	return _container.size();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint bool HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::IsEmpty() const ETNoexceptHint {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::operator bool() const ETNoexceptHint {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::EqualityPredicateType HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::GetEqualityPredicate() const ETNoexceptHint {
	return _container.key_eq();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashPredicateType HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::GetHash() const ETNoexceptHint {
	return _container.hash_predicate();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename const HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::AllocatorType& HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::GetAllocator() const ETNoexceptHint {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint void Swap(HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>& lhs, HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
