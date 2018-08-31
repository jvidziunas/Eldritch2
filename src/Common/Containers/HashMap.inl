/*==================================================================*\
  HashMap.inl
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename InputIterator>
ETInlineHint ETForceInlineHint HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashMap(const AllocatorType& allocator, SizeType bucketCount, const HashPredicateType& hash, const EqualityPredicateType& equal, InputIterator begin, InputIterator end) :
	_container(begin, end, hash, equal, allocator) {}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashMap(const AllocatorType& allocator, SizeType bucketCount, const HashPredicateType& hash, const EqualityPredicateType& equal) :
	_container(bucketCount, hash, equal, allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashMap(const AllocatorType& allocator, std::initializer_list<ValueType> map) :
	_container(map, 0u, HashPredicateType(), EqualityPredicateType(), allocator) {}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashMap(const AllocatorType& allocator, const HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& map) :
	_container(map.Begin(), map.End(), 0u, map.GetHash(), map.GetEqualityPredicate(), allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find(
	const Key2&        key,
	HashPredicate2     hash,
	EqualityPredicate2 equal) const {
	return _container.find_as<Key2, HashPredicate2, EqualityPredicate2>(key, eastl::move(hash), eastl::move(equal));
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find(
	const Key2&        key,
	HashPredicate2     hash,
	EqualityPredicate2 equal) {
	return _container.find_as<Key2, HashPredicate2, EqualityPredicate2>(key, eastl::move(hash), eastl::move(equal));
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find(const KeyType& key) {
	return _container.find(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find(const KeyType& key) const {
	return _container.find(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EraseIf(UnaryPredicate condition) {
	for (Iterator element(_container.begin()), end(_container.end()); element != end;) {
		if (condition(*element)) {
			element = _container.erase(element);
			continue;
		}

		++element;
	}
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint bool HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Contains(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal) const {
	return _container.find_as<Key2, HashPredicate2, EqualityPredicate2>(key, eastl::move(hash), eastl::move(equal)) != _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint bool HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Contains(const KeyType& key) const {
	return _container.find(key) != _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstBegin(SizeType bucketIndex) const {
	return _container.begin(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstBegin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstEnd(SizeType bucketIndex) const {
	return _container.end(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstEnd() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin(SizeType bucketIndex) const {
	return _container.begin(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::LocalIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin(SizeType bucketIndex) {
	return _container.begin(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End(SizeType bucketIndex) const {
	return _container.end(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::LocalIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End(SizeType bucketIndex) {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::MappedType& HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::operator[](const KeyType& key) {
	return _container[key];
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::MappedType& HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::operator[](KeyType&& key) {
	return _container[eastl::move(key)];
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint Pair<typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Insert(const ValueType& value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint Pair<typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Insert(ValueType&& value) {
	return _container.insert(eastl::move(value));
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename... Arguments>
ETInlineHint ETForceInlineHint Pair<typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::TryEmplace(const KeyType& key, Arguments&&... arguments) {
	return _container.try_emplace(key, eastl::forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename... Arguments>
ETInlineHint ETForceInlineHint Pair<typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::TryEmplace(KeyType&& key, Arguments&&... arguments) {
	return _container.try_emplace(eastl::move(key), eastl::forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename... Arguments>
ETInlineHint ETForceInlineHint Pair<typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Emplace(Arguments&&... arguments) {
	return _container.emplace(eastl::forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::SizeType HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase(const KeyType& key) {
	return _container.erase(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint void HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename Disposer>
ETInlineHint ETForceInlineHint void HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ClearAndDispose(Disposer disposer) {
	for (ValueType& value : _container) {
		disposer(value);
	}

	_container.clear();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::SizeType HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint bool HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename const HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EqualityPredicateType& HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetEqualityPredicate() const {
	return _container.key_eq();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename const HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashPredicateType& HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetHash() const {
	return _container.hash_function();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint typename const HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::AllocatorType& HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetAllocator() const {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint ETForceInlineHint void Swap(HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& lhs, HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
