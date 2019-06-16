/*==================================================================*\
  IntrusiveHashTable.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::IntrusiveHashTable(const HashType& hash, const EqualityType& equal) ETNoexceptHintIf(IsNoThrowCopyConstructible<HashType>() && IsNoThrowCopyConstructible<EqualityType>()) : _container(hash, equal) {}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::ConstIterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Find(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal) const {
	return _container.find_as<Key2, HashPredicate2, EqualityPredicate2>(key, Move(hash), Move(equal));
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Iterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Find(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal) {
	return _container.find_as<Key2, HashPredicate2, EqualityPredicate2>(key, Move(hash), Move(equal));
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::ConstIterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Find(const KeyType& key) const {
	return _container.find(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Iterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Find(const KeyType& key) {
	return _container.find(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::ConstLocalIterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Begin(SizeType bucket) const ETNoexceptHint {
	return _container.begin(bucket);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::ConstIterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Begin() const ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::LocalIterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Begin(SizeType bucket) ETNoexceptHint {
	return _container.begin(bucket);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Iterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Begin() ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::ConstLocalIterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::End(SizeType bucket) const ETNoexceptHint {
	return _container.end(bucket);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::ConstIterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::End() const ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::LocalIterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::End(SizeType bucket) ETNoexceptHint {
	return _container.end(bucket);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Iterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::End() ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::InsertReturnType IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Insert(Reference value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Iterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Erase(ConstIterator first, ConstIterator last) {
	return _container.erase(first, last);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Iterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Erase(ConstIterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Iterator IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Erase(Reference value) {
	return _container.remove(value);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::SizeType IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::Erase(const KeyType& key) {
	return _container.erase(key);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::SizeType IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::GetSize(SizeType bucket) const ETNoexceptHint {
	return _container.bucket_size(bucket);
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::SizeType IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::GetSize() const ETNoexceptHint {
	return _container.size();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint bool IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::IsEmpty() const ETNoexceptHint {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::operator bool() const ETNoexceptHint {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::EqualityType IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::GetEquality() const ETNoexceptHint {
	return _container.key_eq();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint typename IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::HashType IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>::GetHash() const ETNoexceptHint {
	return _container.hash_function();
}

// ---------------------------------------------------

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
ETInlineHint ETForceInlineHint void Swap(IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>& lhs, IntrusiveHashTable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>& rhs) ETNoexceptHint {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
