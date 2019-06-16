/*==================================================================*\
  Cache.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Cache(const AllocatorType& allocator, const HashType& hash, const EqualityType& equal) : _lru(allocator), _hashTable(hash, equal) {}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	template <typename InputIterator>
	ETInlineHint ETForceInlineHint Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Cache(const AllocatorType& allocator, const HashType& hash, const EqualityType& equal, InputIterator begin, InputIterator end) : _lru(allocator, begin, end), _hashTable(hash, equal) {
		SyncElements();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Cache(const AllocatorType& allocator, const HashType& hash, const EqualityType& equal, InitializerList<ValueType> values) : _lru(allocator, values), _hashTable(allocator, values) {
		SyncElements();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::ConstIterator Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Find(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal) const {
		return _hashTable.Find<Key2, HashPredicate2, EqualityPredicate2>(key, Move(hash), Move(equal));
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Iterator Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Find(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal) {
		return _hashTable.Find<Key2, HashPredicate2, EqualityPredicate2>(key, Move(hash), Move(equal));
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::ConstIterator Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Find(const KeyType& key) const {
		return _hashTable.Find(key);
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Iterator Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Find(const KeyType& key) {
		return _hashTable.Find(key);
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	ETInlineHint ETForceInlineHint bool Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Touch(const Key2& key, const HashPredicate2& hash, const EqualityPredicate2& equal) {
		Iterator where(_hashTable.Find(key, hash, equal));
		if (where == _hashTable.End()) {
			return false;
		}

		_lru.MoveToBack(LeastRecentlyUsedList::Iterator(*where));

		return true;
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint bool Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Touch(const KeyType& key) {
		Iterator where(_hashTable.Find(key));
		if (where == _hashTable.End()) {
			return false;
		}

		_lru.MoveToBack(LeastRecentlyUsedList::Iterator(*where));

		return true;
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	ETInlineHint ETForceInlineHint bool Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::ContainsKey(const Key2& key, const HashPredicate2& hash, const EqualityPredicate2& equal) const ETNoexceptHint {
		return this->Find(key, hash, equal) == _hashTable.End();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint bool Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::ContainsKey(const KeyType& key) const ETNoexceptHint {
		return this->Find(key) == _hashTable.End();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	template <typename... Arguments>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::InsertReturnType Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::TryEmplace(const KeyType& key, Arguments&&... arguments) {
		Iterator where(_hashTable.Find(key));
		if (where == _hashTable.End()) {
			_lru.EmplaceBack(key, Forward<Arguments>(arguments)...);
			return InsertReturnType(_hashTable.Insert(_lru.Back()), true);
		}

		return InsertReturnType(where, false);
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	template <typename... Arguments>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::InsertReturnType Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::TryEmplace(KeyType&& key, Arguments&&... arguments) {
		Iterator where(_hashTable.Find(key));
		if (where == _hashTable.End()) {
			_lru.EmplaceBack(Move(key), Forward<Arguments>(arguments)...);
			return InsertReturnType(_hashTable.Insert(_lru.Back()), true);
		}

		return InsertReturnType(where, false);
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	template <typename... Arguments>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::InsertReturnType Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Emplace(Arguments&&... arguments) {
		_lru.EmplaceBack(Forward<Arguments>(arguments)...);
		return _hashTable.Insert(_lru.Back());
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	template <typename... Arguments>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Iterator Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::ReplaceOldest(const KeyType& key, Arguments&&... arguments) {
		ETAssert(!this->ContainsKey(key), "Cache already contains entry! Use Touch() or Find() to locate existing elements before replacing.");

		if (_lru) {
			_hashTable.Erase(_lru.Front());
			_lru.PopFront();
		}

		return this->Emplace(key, Forward<Arguments>(arguments));
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	template <typename... Arguments>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Iterator Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::ReplaceOldest(KeyType&& key, Arguments&&... arguments) {
		ETAssert(!this->ContainsKey(key), "Cache already contains entry! Use Touch() or Find() to locate existing elements before replacing.");

		if (_lru) {
			_hashTable.Erase(_lru.Front());
			_lru.PopFront();
		}

		return this->Emplace(Move(key), Forward<Arguments>(arguments));
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Iterator Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Erase(Iterator where) {
		LeastRecentlyUsedList::Iterator value();
		where = _hashTable.Erase(where);
		_lru.Erase(where->second);

		return where;
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::SizeType Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Erase(const KeyType& key) {
		const Iterator where(_hashTable.Find(key));
		if (where == _hashTable.End()) {
			return SizeType(0u);
		}

		_hashTable.Erase(where);
		_lru.Erase(where->second);
		return SizeType(1u);
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	template <typename UnaryPredicate>
	ETInlineHint ETForceInlineHint void Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::ClearAndDispose(UnaryPredicate disposer) {
		_hashTable.Clear();
		_lru.ClearAndDispose<UnaryPredicate>(Move(disposer));
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint void Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::Clear() {
		_hashTable.Clear();
		_lru.Clear();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::SizeType Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::GetSize() const ETNoexceptHint {
		return _hashTable.Size();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint bool Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::IsEmpty() const ETNoexceptHint {
		return _hashTable.IsEmpty();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::operator bool() const ETNoexceptHint {
		return !_hashTable.IsEmpty();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::EqualityType Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::GetEquality() const ETNoexceptHint {
		return _hashTable.GetEquality();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::HashType Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::GetHash() const ETNoexceptHint {
		return _hashTable.GetHash();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint typename const Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::AllocatorType& Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::GetAllocator() const ETNoexceptHint {
		return _hashTable.GetAllocator();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint void Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>::SyncElements() {
		for (typename LeastRecentlyUsedList::Reference value : _lru) {
			_hashTable.Insert(value);
		}
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, size_t HashBuckets, class Allocator>
	ETInlineHint ETForceInlineHint void Swap(Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>& lhs, Cache<Key, Value, HashPredicate, EqualityPredicate, HashBuckets, Allocator>& rhs) ETNoexceptHint {
		Swap(lhs._hashTable, rhs._hashTable);
		Swap(lhs._lru, rhs._lru);
	}

}	// namespace Eldritch2
