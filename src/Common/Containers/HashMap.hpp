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

template <typename Key, typename Value, class HashPredicate = Hash<Key>, class EqualityPredicate = EqualTo<Key>, class Allocator = MallocAllocator, bool CacheHashCode = false>
class HashMap {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::hash_map<Key, Value, HashPredicate, EqualityPredicate, EaStlAllocatorMixin<Allocator>, CacheHashCode>;

public:
	using ValueType          = typename UnderlyingContainer::value_type;
	using KeyType            = typename UnderlyingContainer::key_type;
	using MappedType         = typename UnderlyingContainer::mapped_type;
	using NodeType           = typename UnderlyingContainer::node_type;
	using ConstReference     = typename UnderlyingContainer::const_reference;
	using Reference          = typename UnderlyingContainer::reference;
	using AllocatorType      = typename UnderlyingContainer::allocator_type::PublicType;
	using SizeType           = typename UnderlyingContainer::size_type;
	using HashCode           = typename UnderlyingContainer::hash_code_t;
	using ConstIterator      = typename UnderlyingContainer::const_iterator;
	using Iterator           = typename UnderlyingContainer::iterator;
	using ConstLocalIterator = typename UnderlyingContainer::const_local_iterator;
	using LocalIterator      = typename UnderlyingContainer::local_iterator;
	using EqualityType       = EqualityPredicate;
	using HashType           = HashPredicate;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	//! Constructs this @ref HashMap instance.
	HashMap(const AllocatorType& allocator = AllocatorType(), SizeType bucketCount = 0u, const HashType& hash = HashType(), const EqualityType& equal = EqualityType());
	//! Constructs this @ref HashMap instance.
	template <typename InputIterator>
	HashMap(const AllocatorType& allocator, SizeType bucketCount, const HashType& hash, const EqualityType& equal, InputIterator begin, InputIterator end);
	//! Constructs this @ref HashMap instance.
	HashMap(const AllocatorType& allocator, InitializerList<ValueType>);
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
	ConstLocalIterator ConstBegin(SizeType bucket) const ETNoexceptHint;
	ConstIterator      ConstBegin() const ETNoexceptHint;

	ConstLocalIterator ConstEnd(SizeType bucket) const ETNoexceptHint;
	ConstIterator      ConstEnd() const ETNoexceptHint;

	ConstLocalIterator Begin(SizeType bucket) const ETNoexceptHint;
	LocalIterator      Begin(SizeType bucket) ETNoexceptHint;
	ConstIterator      Begin() const ETNoexceptHint;
	Iterator           Begin() ETNoexceptHint;

	ConstLocalIterator End(SizeType bucket) const ETNoexceptHint;
	LocalIterator      End(SizeType bucket) ETNoexceptHint;
	ConstIterator      End() const ETNoexceptHint;
	Iterator           End() ETNoexceptHint;

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
	template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
	SizeType Erase(const Value2& value, HashPredicate2 hash, EqualityPredicate2 equal);
	SizeType Erase(const KeyType& key);

	template <typename UnaryPredicate>
	void ClearAndDispose(UnaryPredicate disposer);

	void Clear();

	// - ELEMENT ACCESS ----------------------------------

public:
	MappedType& operator[](const KeyType&);
	MappedType& operator[](KeyType&&);

	// - CONTAINER DUPLICATION ---------------------------

public:
	HashMap& operator=(const HashMap&) = default;
	HashMap& operator=(HashMap&&) = default;

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

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Key2, typename Value2, class HashPredicate2, class EqualityPredicate2, class Allocator2, bool CacheHashCode2>
	friend void Swap(HashMap<Key2, Value2, HashPredicate2, EqualityPredicate2, Allocator2, CacheHashCode2>&, HashMap<Key2, Value2, HashPredicate2, EqualityPredicate2, Allocator2, CacheHashCode2>&) ETNoexceptHint;
};

template <typename Key, typename Value, class HashPredicate = Hash<Key>, class EqualityPredicate = EqualTo<Key>, class Allocator = MallocAllocator>
using CachingHashMap = HashMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, true>;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/HashMap.inl>
//------------------------------------------------------------------//
