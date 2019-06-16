/*==================================================================*\
  Cache.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/IntrusiveHashTableHook.hpp>
#include <Common/Containers/IntrusiveHashTable.hpp>
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/Containers/LinkedList.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Key, typename Value, class HashPredicate = Hash<Key>, class EqualityPredicate = EqualTo<Key>, size_t HashBuckets = 16u, class Allocator = MallocAllocator>
class Cache {
	// - TYPE PUBLISHING ---------------------------------

public:
	struct ValueType : public IntrusiveHashMapHook<Key> {
	public:
		template <typename... Arguments>
		ValueType(const Key& key, Arguments&&...);
		template <typename... Arguments>
		ValueType(Key&& key, Arguments&&...);

		~ValueType() = default;

	public:
		Value value;
	};

	// ---

private:
	using HashTable             = IntrusiveHashTable<typename ValueType::KeyType, ValueType, HashPredicate, EqualityPredicate, HashBuckets, false, true>;
	using LeastRecentlyUsedList = LinkedList<typename HashTable::ValueType, Allocator>;
	using UnderlyingContainer   = LeastRecentlyUsedList;

public:
	using KeyType          = typename HashTable::KeyType;
	using MappedType       = Value;
	using ConstReference   = typename UnderlyingContainer::ConstReference;
	using Reference        = typename UnderlyingContainer::Reference;
	using AllocatorType    = typename UnderlyingContainer::AllocatorType;
	using SizeType         = typename UnderlyingContainer::SizeType;
	using HashCode         = typename HashTable::HashCode;
	using ConstIterator    = typename HashTable::ConstIterator;
	using Iterator         = typename HashTable::Iterator;
	using EqualityType     = typename HashTable::EqualityType;
	using HashType         = typename HashTable::HashType;
	using InsertReturnType = typename HashTable::InsertReturnType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref Cache instance.
	Cache(const AllocatorType& allocator = AllocatorType(), const HashType& hash = HashType(), const EqualityType& equal = EqualityType());
	//! Constructs this @ref Cache instance.
	template <typename InputIterator>
	Cache(const AllocatorType& allocator, const HashType& hash, const EqualityType& equal, InputIterator begin, InputIterator end);
	//! Constructs this @ref Cache instance.
	Cache(const AllocatorType& allocator, const HashType& hash, const EqualityType& equal, InitializerList<ValueType>);
	//! Constructs this @ref Cache instance.
	Cache(Cache&&) ETNoexceptHint = default;

	~Cache() = default;

	// - ALGORITHMS --------------------------------------

public:
	//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	ConstIterator Find(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal) const;
	//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	Iterator Find(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal);
	//! Retrieves an iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	ConstIterator Find(const KeyType& key) const;
	//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	Iterator Find(const KeyType& key);

	//! Erases all elements for which the result of the predicate returns true.
	template <typename UnaryPredicate>
	void EraseIf(UnaryPredicate filter);

	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	bool Touch(const Key2& key, const HashPredicate2& hash, const EqualityPredicate2& equal);
	bool Touch(const KeyType&);

	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	bool ContainsKey(const Key2& key, const HashPredicate2& hash, const EqualityPredicate2& equal) const ETNoexceptHint;
	bool ContainsKey(const KeyType&) const ETNoexceptHint;

	// - CONTAINER MANIPULATION --------------------------

public:
	template <typename... Arguments>
	InsertReturnType TryEmplace(const KeyType&, Arguments&&...);
	template <typename... Arguments>
	InsertReturnType TryEmplace(KeyType&&, Arguments&&...);

	template <typename... Arguments>
	InsertReturnType Emplace(Arguments&&...);

	template <typename... Arguments>
	Iterator ReplaceOldest(const KeyType&, Arguments&&...);
	template <typename... Arguments>
	Iterator ReplaceOldest(KeyType&&, Arguments&&...);

	SizeType Erase(const KeyType&);
	Iterator Erase(Iterator);

	template <typename UnaryPredicate>
	void ClearAndDispose(UnaryPredicate disposer);

	void Clear();

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const ETNoexceptHint;

	bool IsEmpty() const ETNoexceptHint;

	explicit operator bool() const ETNoexceptHint;

	// - INDEXING ACCESS ---------------------------------

public:
	EqualityType GetEquality() const ETNoexceptHint;

	HashType GetHash() const ETNoexceptHint;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const ETNoexceptHint;

	// - CONTAINER DUPLICATION ---------------------------

public:
	Cache& operator=(Cache&&) = default;

	// ---------------------------------------------------

private:
	void SyncElements();

	// - DATA MEMBERS ------------------------------------

private:
	LeastRecentlyUsedList _lru;
	HashTable             _hashTable;

	// ---------------------------------------------------

	template <typename Key2, typename Value2, class HashPredicate2, class EqualityPredicate2, class Allocator2>
	friend void Swap(Cache<Key2, Value2, HashPredicate2, EqualityPredicate2, Allocator2>&, Cache<Key2, Value2, HashPredicate2, EqualityPredicate2, Allocator2>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/Cache.inl>
//------------------------------------------------------------------//
