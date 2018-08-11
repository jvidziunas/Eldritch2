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
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/EqualTo.hpp>
#include <Common/Hash.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(4800) EASTL does some int to bool coercion MSVC doesn't like.
ET_SET_MSVC_WARNING_STATE(disable : 4800)
#include <eastl/bonus/intrusive_slist.h>
#include <eastl/intrusive_hash_map.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Key, typename Value, size_t bucketCount, class HashPredicate = Hash<Key>, class KeyEqualityPredicate = EqualTo<Key>, class Allocator = MallocAllocator>
class Cache {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::intrusive_hash_map<Key, Value, bucketCount, HashPredicate, KeyEqualityPredicate>;

public:
	using ValueType             = typename UnderlyingContainer::value_type;
	using KeyType               = typename UnderlyingContainer::key_type;
	using MappedType            = typename UnderlyingContainer::mapped_type;
	using NodeType              = typename UnderlyingContainer::node_type;
	using AllocatorType         = typename UnderlyingContainer::allocator_type::PublicType;
	using SizeType              = typename UnderlyingContainer::size_type;
	using HashPredicateType     = HashPredicate;
	using EqualityPredicateType = KeyEqualityPredicate;
	using HashCode              = typename UnderlyingContainer::hash_code_t;
	using Iterator              = typename UnderlyingContainer::iterator;
	using ConstIterator         = typename UnderlyingContainer::const_iterator;
	using LocalIterator         = typename UnderlyingContainer::local_iterator;
	using ConstLocalIterator    = typename UnderlyingContainer::const_local_iterator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	//! Constructs this @ref Cache instance.
	explicit Cache(
		const HashPredicateType&     hash              = HashPredicateType(),
		const EqualityPredicateType& equalityPredicate = EqualityPredicateType(),
		const AllocatorType&         allocator         = AllocatorType());
	//! Constructs this @ref Cache instance.
	Cache(const HashPredicateType& hash, const EqualityPredicateType& equalityPredicate, const AllocatorType& allocator);
	//! Constructs this @ref Cache instance.
	Cache(Cache&&, const AllocatorType& allocator);
	//! Constructs this @ref Cache instance.
	Cache(const AllocatorType& allocator = AllocatorType());
	//! Constructs this @ref Cache instance.
	Cache(const Cache&);
	//! Constructs this @ref Cache instance.
	Cache(Cache&&);

	~Cache() = default;

	// - ALGORITHMS --------------------------------------

public:
	//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
	ConstIterator Find(const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate) const;
	//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
	Iterator Find(const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate);
	//! Retrieves an iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	ConstIterator Find(const KeyType& key) const;
	//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	Iterator Find(const KeyType& key);

	//! Erases all elements for which the result of the predicate returns true.
	template <typename Predicate>
	void EraseIf(Predicate predicate);

	template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
	bool ContainsKey(const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate) const;
	bool ContainsKey(const KeyType& key) const;

	// - ELEMENT ITERATION -------------------------------

public:
	//!	Retrieves an iterator to the first element stored in this @ref Cache.
	ConstLocalIterator ConstBegin(SizeType bucketIndex) const;
	ConstIterator      ConstBegin() const;

	//!	Retrieves an iterator to one past the end of all elements stored in this @ref Cache.
	ConstLocalIterator ConstEnd(SizeType bucketIndex) const;
	ConstIterator      ConstEnd() const;

	//!	Retrieves a local iterator to the first element stored in the specified bucket of this @ref Cache.
	ConstLocalIterator Begin(SizeType bucketIndex) const;
	LocalIterator      Begin(SizeType bucketIndex);

	//!	Retrieves an iterator to the first element stored in this @ref Cache.
	ConstIterator Begin() const;
	Iterator      Begin();

	//!	Retrieves a local iterator to one past the end of all elements stored in the specified bucket of this @ref Cache.
	ConstLocalIterator End(SizeType bucketIndex) const;
	LocalIterator      End(SizeType bucketIndex);

	//! Retrieves an iterator to one past the end of all elements stored in this @ref Cache.
	ConstIterator End() const;
	Iterator      End();

	// - CONTAINER DUPLICATION ---------------------------

public:
	Cache& operator=(const Cache&);
	Cache& operator=(Cache&&);

	// - CONTAINER MANIPULATION --------------------------

public:
	Pair<Iterator, bool> Insert(const ValueType& value);
	Pair<Iterator, bool> Insert(ValueType&& value);

	template <typename... Arguments>
	Pair<Iterator, bool> TryEmplace(const KeyType& key, Arguments&&... arguments);
	template <typename... Arguments>
	Pair<Iterator, bool> TryEmplace(KeyType&& key, Arguments&&... arguments);

	template <typename... Arguments>
	Pair<Iterator, bool> Emplace(Arguments&&... arguments);

	Iterator Erase(Iterator first, Iterator last);
	Iterator Erase(Iterator position);
	SizeType Erase(const KeyType& key);

	template <typename Disposer>
	void ClearAndDispose(Disposer disposer);

	void Clear();

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const;

	bool IsEmpty() const;

	explicit operator bool() const;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const EqualityPredicateType& GetKeyEqualityPredicate() const;

	const HashPredicateType& GetHashPredicate() const;

	const AllocatorType& GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

private:
	AllocatorType       _allocator;
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Key, typename Value, size_t bucketCount, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	friend void Swap(Cache<Key, Value, bucketCount, HashPredicate, KeyEqualityPredicate, Allocator>&, Cache<Key, Value, bucketCount, HashPredicate, KeyEqualityPredicate, Allocator>&);
};

} // namespace Eldritch2
