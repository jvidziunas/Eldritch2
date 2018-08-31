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
//	(4800) EASTL does some int to bool coercion MSVC doesn't like.
ET_PUSH_MSVC_WARNING_STATE(disable : 4800)
#include <eastl/bonus/intrusive_slist.h>
#include <eastl/intrusive_hash_map.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Key, typename Value, size_t bucketCount, class HashPredicate = Hash<Key>, class EqualityPredicate = EqualTo<Key>, class Allocator = MallocAllocator>
class Cache {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::intrusive_hash_map<Key, Value, bucketCount, HashPredicate, EqualityPredicate>;

public:
	using ValueType             = typename UnderlyingContainer::value_type;
	using KeyType               = typename UnderlyingContainer::key_type;
	using MappedType            = typename UnderlyingContainer::mapped_type;
	using NodeType              = typename UnderlyingContainer::node_type;
	using AllocatorType         = typename UnderlyingContainer::allocator_type::PublicType;
	using SizeType              = typename UnderlyingContainer::size_type;
	using HashCode              = typename UnderlyingContainer::hash_code_t;
	using Iterator              = typename UnderlyingContainer::iterator;
	using ConstIterator         = typename UnderlyingContainer::const_iterator;
	using LocalIterator         = typename UnderlyingContainer::local_iterator;
	using ConstLocalIterator    = typename UnderlyingContainer::const_local_iterator;
	using EqualityPredicateType = EqualityPredicate;
	using HashPredicateType     = HashPredicate;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	//! Constructs this @ref Cache instance.
	Cache(const AllocatorType& allocator = AllocatorType(), const HashPredicateType& hash = HashPredicateType(), const EqualityPredicateType& equal = EqualityPredicateType());
	//! Constructs this @ref Cache instance.
	template <typename InputIterator>
	Cache(const AllocatorType& allocator, const HashPredicateType& hash, const EqualityPredicateType& equal, InputIterator begin, InputIterator end);
	//! Constructs this @ref Cache instance.
	Cache(const AllocatorType& allocator, const HashPredicateType& hash, const EqualityPredicateType& equal, std::initializer_list<ValueType>);
	//! Constructs this @ref Cache instance.
	Cache(const AllocatorType& allocator, const Cache&);
	//! Constructs this @ref Cache instance.
	Cache(const Cache&) = default;
	//! Constructs this @ref Cache instance.
	Cache(Cache&&) = default;

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
	bool ContainsKey(const Key2& key, const HashPredicate2& hash, const EqualityPredicate2& equal) const;
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
	Iterator Erase(Iterator where);
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
	const EqualityPredicateType& GetEqualityPredicate() const;

	const HashPredicateType& GetHash() const;

	const AllocatorType& GetAllocator() const;

	// - CONTAINER DUPLICATION ---------------------------

public:
	Cache& operator=(const Cache&) = default;
	Cache& operator=(Cache&&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	AllocatorType       _allocator;
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Key, typename Value, size_t bucketCount, class HashPredicate, class EqualityPredicate, class Allocator>
	friend void Swap(Cache<Key, Value, bucketCount, HashPredicate, EqualityPredicate, Allocator>&, Cache<Key, Value, bucketCount, HashPredicate, EqualityPredicate, Allocator>&);
};

} // namespace Eldritch2
