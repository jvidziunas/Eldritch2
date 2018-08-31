/*==================================================================*\
  HashMap.hpp
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
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
#include <Common/Pair.hpp>
//------------------------------------------------------------------//
//	(4800) EASTL does some int to bool coercion MSVC doesn't like.
ET_PUSH_MSVC_WARNING_STATE(disable : 4800)
#include <eastl/hash_map.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Key, typename Value, class HashPredicate = Hash<Key>, class EqualityPredicate = EqualTo<Key>, class Allocator = MallocAllocator, bool cacheHashCode = false>
class HashMap {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::hash_map<Key, Value, HashPredicate, EqualityPredicate, EaStlAllocatorMixin<Allocator>, cacheHashCode>;

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

	//! Constructs this @ref HashMap instance.
	HashMap(const AllocatorType& allocator = AllocatorType(), SizeType bucketCount = 0u, const HashPredicateType& hash = HashPredicateType(), const EqualityPredicateType& equal = EqualityPredicateType());
	//! Constructs this @ref HashMap instance.
	template <typename InputIterator>
	HashMap(const AllocatorType& allocator, SizeType bucketCount, const HashPredicateType& hash, const EqualityPredicateType& equal, InputIterator begin, InputIterator end);
	//! Constructs this @ref HashMap instance.
	HashMap(const AllocatorType& allocator, std::initializer_list<ValueType>);
	//! Constructs this @ref HashMap instance.
	HashMap(const AllocatorType& allocator, const HashMap&);
	//! Constructs this @ref HashMap instance.
	HashMap(const HashMap&) = default;
	//! Constructs this @ref HashMap instance.
	HashMap(HashMap&&) = default;

	~HashMap() = default;

	// - ALGORITHMS --------------------------------------

public:
	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	ConstIterator Find(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal) const;
	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	Iterator      Find(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal);
	ConstIterator Find(const KeyType& key) const;
	Iterator      Find(const KeyType& key);

	//! Erases all elements for which the result of the predicate returns true.
	template <typename UnaryPredicate>
	void EraseIf(UnaryPredicate condition);

	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	bool Contains(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal) const;
	bool Contains(const KeyType& key) const;

	// - ELEMENT ITERATION -------------------------------

public:
	ConstLocalIterator ConstBegin(SizeType bucketIndex) const;
	ConstIterator      ConstBegin() const;

	ConstLocalIterator ConstEnd(SizeType bucketIndex) const;
	ConstIterator      ConstEnd() const;

	ConstLocalIterator Begin(SizeType bucketIndex) const;
	LocalIterator      Begin(SizeType bucketIndex);
	ConstIterator      Begin() const;
	Iterator           Begin();

	ConstLocalIterator End(SizeType bucketIndex) const;
	LocalIterator      End(SizeType bucketIndex);
	ConstIterator      End() const;
	Iterator           End();

	// - ELEMENT ACCESS ----------------------------------

public:
	MappedType& operator[](const KeyType& key);
	MappedType& operator[](KeyType&& key);

	// - CONTAINER DUPLICATION ---------------------------

public:
	HashMap& operator=(const HashMap&) = default;
	HashMap& operator=(HashMap&&) = default;

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

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	friend void Swap(HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>&, HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>&);
};

template <typename Key, typename Value, class HashPredicate = Hash<Key>, class EqualityPredicate = EqualTo<Key>, class Allocator = MallocAllocator>
using CachingHashMap = HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, true>;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/HashMap.inl>
//------------------------------------------------------------------//
