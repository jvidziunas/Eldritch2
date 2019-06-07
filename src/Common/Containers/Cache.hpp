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
#include <Common/Containers/IntrusiveLinkedList.hpp>
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/Containers/HashMap.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Key, typename Value, class HashPredicate = Hash<Key>, class EqualityPredicate = EqualTo<Key>, class Allocator = MallocAllocator>
class Cache {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer   = HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator>;
	using LeastRecentlyUsedList = IntrusiveLinkedList<typename UnderlyingContainer::ValueType>;

public:
	using ValueType             = typename UnderlyingContainer::ValueType;
	using KeyType               = typename UnderlyingContainer::KeyType;
	using MappedType            = typename UnderlyingContainer::MappedType;
	using ConstReference        = typename UnderlyingContainer::ConstReference;
	using Reference             = typename UnderlyingContainer::Reference;
	using AllocatorType         = typename UnderlyingContainer::AllocatorType;
	using SizeType              = typename UnderlyingContainer::SizeType;
	using HashCode              = typename UnderlyingContainer::HashCode;
	using ConstIterator         = typename UnderlyingContainer::ConstIterator;
	using Iterator              = typename UnderlyingContainer::Iterator;
	using ConstLocalIterator    = typename UnderlyingContainer::ConstLocalIterator;
	using LocalIterator         = typename UnderlyingContainer::LocalIterator;
	using EqualityPredicateType = typename UnderlyingContainer::EqualityPredicateType;
	using HashPredicateType     = typename UnderlyingContainer::HashPredicateType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref Cache instance.
	Cache(const AllocatorType& allocator = AllocatorType(), const HashPredicateType& hash = HashPredicateType(), const EqualityPredicateType& equal = EqualityPredicateType());
	//! Constructs this @ref Cache instance.
	template <typename InputIterator>
	Cache(const AllocatorType& allocator, const HashPredicateType& hash, const EqualityPredicateType& equal, InputIterator begin, InputIterator end);
	//! Constructs this @ref Cache instance.
	Cache(const AllocatorType& allocator, const HashPredicateType& hash, const EqualityPredicateType& equal, InitializerList<ValueType>);
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
	bool Touch(const KeyType& key);

	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	bool ContainsKey(const Key2& key, const HashPredicate2& hash, const EqualityPredicate2& equal) const ETNoexceptHint;
	bool ContainsKey(const KeyType& key) const ETNoexceptHint;

	// - CONTAINER MANIPULATION --------------------------

public:
	Pair<Iterator, bool> Insert(const ValueType&);
	Pair<Iterator, bool> Insert(ValueType&&);

	template <typename... Arguments>
	Pair<Iterator, bool> TryEmplace(const KeyType&, Arguments&&...);
	template <typename... Arguments>
	Pair<Iterator, bool> TryEmplace(KeyType&&, Arguments&&...);

	template <typename... Arguments>
	Pair<Iterator, bool> Emplace(Arguments&&...);

	template <typename... Arguments>
	Iterator ReplaceOldest(const KeyType&, Arguments&&...);
	template <typename... Arguments>
	Iterator ReplaceOldest(KeyType&&, Arguments&&...);

	Iterator Erase(Iterator where);
	SizeType Erase(const KeyType&);

	template <typename UnaryPredicate>
	void ClearAndDispose(UnaryPredicate disposer);

	void Clear();

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const ETNoexceptHint;

	bool IsEmpty() const ETNoexceptHint;

	explicit operator bool() const ETNoexceptHint;

	// - ALLOCATOR ACCESS --------------------------------

public:
	EqualityPredicateType GetEqualityPredicate() const ETNoexceptHint;

	HashPredicateType GetHash() const ETNoexceptHint;

	const AllocatorType& GetAllocator() const ETNoexceptHint;

	// - CONTAINER DUPLICATION ---------------------------

public:
	Cache& operator=(Cache&&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer   _container;
	LeastRecentlyUsedList _lru;

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
