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

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename InputIterator>
ETInlineHint ETForceInlineHint HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashMap(const AllocatorType& allocator, SizeType bucketCount, const HashType& hash, const EqualityType& equal, InputIterator begin, InputIterator end) :
	_container(begin, end, hash, equal, allocator) {}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashMap(const AllocatorType& allocator, SizeType bucketCount, const HashType& hash, const EqualityType& equal) :
	_container(bucketCount, hash, equal, allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashMap(const AllocatorType& allocator, InitializerList<ValueType> values) :
	_container(values, 0u, HashType(), EqualityType(), allocator) {}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashMap(const AllocatorType& allocator, const HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>& map) :
	_container(map.Begin(), map.End(), 0u, map.GetHash(), map.GetEquality(), allocator) {
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Find(
	const Key2&        key,
	HashPredicate2     hash,
	EqualityPredicate2 equal) const {
	return _container.find_as<Key2, HashPredicate2, EqualityPredicate2>(key, Move(hash), Move(equal));
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Find(
	const Key2&        key,
	HashPredicate2     hash,
	EqualityPredicate2 equal) {
	return _container.find_as<Key2, HashPredicate2, EqualityPredicate2>(key, Move(hash), Move(equal));
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Find(const KeyType& key) {
	return _container.find(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Find(const KeyType& key) const {
	return _container.find(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::EraseIf(UnaryPredicate condition) {
	for (Iterator element(_container.begin()), end(_container.end()); element != end;) {
		if (condition(*element)) {
			element = _container.erase(element);
			continue;
		}

		++element;
	}
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint bool HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Contains(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal) const {
	return _container.find_as<Key2, HashPredicate2, EqualityPredicate2>(key, Move(hash), Move(equal)) != _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint bool HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Contains(const KeyType& key) const {
	return _container.find(key) != _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstBegin(SizeType bucketIndex) const ETNoexceptHint {
	return _container.begin(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstBegin() const ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstEnd(SizeType bucketIndex) const ETNoexceptHint {
	return _container.end(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstEnd() const ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Begin(SizeType bucketIndex) const ETNoexceptHint {
	return _container.begin(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::LocalIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Begin(SizeType bucketIndex) ETNoexceptHint {
	return _container.begin(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Begin() const ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Begin() ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::End(SizeType bucketIndex) const ETNoexceptHint {
	return _container.end(bucketIndex);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::LocalIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::End(SizeType bucketIndex) ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::End() const ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::End() ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::MappedType& HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::operator[](const KeyType& key) {
	return _container[key];
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::MappedType& HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::operator[](KeyType&& key) {
	return _container[Move(key)];
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint Pair<typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Insert(const ValueType& value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint Pair<typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Insert(ValueType&& value) {
	return _container.insert(Move(value));
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename... Arguments>
ETInlineHint ETForceInlineHint Pair<typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::TryEmplace(const KeyType& key, Arguments&&... arguments) {
	return _container.try_emplace(key, Forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename... Arguments>
ETInlineHint ETForceInlineHint Pair<typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::TryEmplace(KeyType&& key, Arguments&&... arguments) {
	return _container.try_emplace(Move(key), Forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename... Arguments>
ETInlineHint ETForceInlineHint Pair<typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Emplace(Arguments&&... arguments) {
	return _container.emplace(Forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::SizeType HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Erase(const Value2& value, HashPredicate2 hash, EqualityPredicate2 equal) {
	const auto where(_container.find_as<Value2, HashPredicate2, EqualityPredicate2>(value, Move(hash), Move(equal)));
	if (where == _container.end()) {
		return 0;
	}

	_container.erase(where);
	return 1;
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::SizeType HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Erase(const KeyType& key) {
	return _container.erase(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint void HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ClearAndDispose(UnaryPredicate disposer) {
	for (Reference value : _container) {
		disposer(value.second);
	}

	_container.clear();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::SizeType HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::GetSize() const ETNoexceptHint {
	return _container.size();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint bool HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::IsEmpty() const ETNoexceptHint {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::operator bool() const ETNoexceptHint {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::EqualityType HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::GetEquality() const ETNoexceptHint {
	return _container.key_eq();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashType HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::GetHash() const ETNoexceptHint {
	return _container.hash_function();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename const HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::AllocatorType& HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::GetAllocator() const ETNoexceptHint {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint void Swap(HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>& lhs, HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>& rhs) ETNoexceptHint {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
