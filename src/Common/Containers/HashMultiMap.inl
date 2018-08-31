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

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashMultiMap(
	const AllocatorType&         allocator,
	SizeType                     bucketCount,
	const HashPredicateType&     hash,
	const EqualityPredicateType& equal) :
	_container(bucketCount, hash, equal, allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename InputIterator>
ETInlineHint ETForceInlineHint HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashMultiMap(
	const AllocatorType&         allocator,
	SizeType                     bucketCount,
	const HashPredicateType&     hash,
	const EqualityPredicateType& equal,
	InputIterator                begin,
	InputIterator                end) :
	_container(begin, end, bucketCount, hash, equal, allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashMultiMap(
	const AllocatorType&             allocator,
	SizeType                         bucketCount,
	const HashPredicateType&         hash,
	const EqualityPredicateType&     equal,
	std::initializer_list<ValueType> map) :
	_container(map, bucketCount, hash, equal, allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashMultiMap(const AllocatorType& allocator, const HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& map) :
	_container(map.Begin(), map.End(), 0u, map.GetHash(), map.GetEqualityPredicate(), allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint Range<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EqualRange(
	const Key2&               key,
	const HashPredicate2&     hash,
	const EqualityPredicate2& equal) const {
	const auto resultPair(_container.equal_range(key, hash, equal));
	return { resultPair.first, resultPair.second };
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint Range<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EqualRange(
	const Key2&               key,
	const HashPredicate2&     hash,
	const EqualityPredicate2& equal) {
	const auto resultPair(_container.equal_range(key, hash, equal));
	return { resultPair.first, resultPair.second };
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint Range<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EqualRange(const KeyType& key) const {
	const auto resultPair(_container.equal_range(key));
	return { resultPair.first, resultPair.second };
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint Range<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EqualRange(const KeyType& key) {
	const auto resultPair(_container.equal_range(key));
	return { resultPair.first, resultPair.second };
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EraseIf(UnaryPredicate condition) {
	for (Iterator element(_container.begin()), end(_container.end()); element != end;) {
		if (condition(*element)) {
			element = _container.erase(element);
		} else {
			++element;
		}
	}
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstBegin(SizeType bucketIndex) const {
	return _container.begin(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstBegin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstEnd(SizeType bucketIndex) const {
	return _container.end(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstEnd() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin(SizeType bucketIndex) const {
	return _container.begin(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::LocalIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin(SizeType bucketIndex) {
	return _container.begin(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End(SizeType bucketIndex) const {
	return _container.end(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::LocalIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End(SizeType bucketIndex) {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint Pair<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Insert(const ValueType& value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint Pair<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Insert(ValueType&& value) {
	return _container.insert(eastl::move(value));
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename... Arguments>
ETInlineHint ETForceInlineHint Pair<typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Emplace(Arguments&&... arguments) {
	return _container.emplace(eastl::forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::SizeType HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase(const KeyType& key) {
	return _container.erase(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint void HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename Disposer>
ETInlineHint ETForceInlineHint void HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ClearAndDispose(Disposer disposer) {
	for (ValueType& value : _container) {
		disposer(value);
	}

	_container.clear();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::SizeType HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetSize() const ETNoexceptHint {
	return _container.size();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint bool HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::IsEmpty() const ETNoexceptHint {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::operator bool() const ETNoexceptHint {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename const HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EqualityPredicateType& HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetEqualityPredicate() const ETNoexceptHint {
	return _container.key_eq();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename const HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashPredicateType& HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetHash() const ETNoexceptHint {
	return _container.hash_predicate();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename const HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::AllocatorType& HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetAllocator() const ETNoexceptHint {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint void Swap(HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& lhs, HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
