/*==================================================================*\
  HashMultiMap.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/Containers/Span.hpp>
#include <Common/EqualTo.hpp>
#include <Common/Hash.hpp>
#include <Common/Pair.hpp>
//------------------------------------------------------------------//
#include <eastl/hash_map.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Key, typename Value, class HashPredicate = Hash<Key>, class EqualityPredicate = EqualTo<Key>, class Allocator = MallocAllocator, bool CacheHashCode = false>
class HashMultiMap {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::hash_multimap<Key, Value, HashPredicate, EqualityPredicate, EaStlAllocatorMixin<Allocator>, CacheHashCode>;

public:
	using ValueType          = typename UnderlyingContainer::value_type;
	using KeyType            = typename UnderlyingContainer::key_type;
	using MappedType         = typename UnderlyingContainer::mapped_type;
	using ConstPointer       = typename UnderlyingContainer::const_pointer;
	using Pointer            = typename UnderlyingContainer::pointer;
	using ConstReference     = typename UnderlyingContainer::const_reference;
	using Reference          = typename UnderlyingContainer::reference;
	using AllocatorType      = typename UnderlyingContainer::allocator_type::PublicType;
	using SizeType           = typename UnderlyingContainer::size_type;
	using ConstIterator      = typename UnderlyingContainer::const_iterator;
	using Iterator           = typename UnderlyingContainer::iterator;
	using ConstLocalIterator = typename UnderlyingContainer::const_local_iterator;
	using LocalIterator      = typename UnderlyingContainer::local_iterator;
	using EqualityType       = EqualityPredicate;
	using HashType           = HashPredicate;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref HashMultiMap instance.
	HashMultiMap(const AllocatorType& allocator = AllocatorType(), SizeType bucketCount = 0u, const HashType& hash = HashType(), const EqualityType& equal = EqualityType());
	//! Constructs this @ref HashMultiMap instance.
	template <typename InputIterator>
	HashMultiMap(const AllocatorType& allocator, SizeType bucketCount, const HashType& hash, const EqualityType& equal, InputIterator begin, InputIterator end);
	//! Constructs this @ref HashMultiMap instance.
	HashMultiMap(const AllocatorType& allocator, SizeType bucketCount, const HashType& hash, const EqualityType& equal, InitializerList<ValueType>);
	//! Constructs this @ref HashMultiMap instance.
	HashMultiMap(const AllocatorType& allocator, const HashMultiMap&);
	//! Constructs this @ref HashMultiMap instance.
	HashMultiMap(const HashMultiMap&) = default;
	//! Constructs this @ref HashMultiMap instance.
	HashMultiMap(HashMultiMap&&) = default;

	~HashMultiMap() = default;

	// - ALGORITHMS --------------------------------------

public:
	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	Span<ConstIterator> Slice(const Key2& key, const HashPredicate2& hash, const EqualityPredicate2& equal) const;
	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	Span<Iterator>      Slice(const Key2& key, const HashPredicate2& hash, const EqualityPredicate2& equal);
	Span<ConstIterator> Slice(const KeyType& key) const;
	Span<Iterator>      Slice(const KeyType& key);

	//! Erases all elements for which the result of the predicate returns true.
	template <typename UnaryPredicate>
	void EraseIf(UnaryPredicate filter);

	// - ELEMENT ITERATION -------------------------------

public:
	//!	Retrieves a @ref ConstLocalIterator to the first element stored in the specified bucket of this @ref HashMultiMap.
	ConstLocalIterator ConstBegin(SizeType bucketIndex) const;
	//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashMultiMap.
	ConstIterator ConstBegin() const;

	//!	Retrieves a @ref ConstLocalIterator to one past the end of all elements stored in this @ref HashMultiMap.
	ConstLocalIterator ConstEnd(SizeType bucketIndex) const;
	//!	Retrieves a @ref ConstIterator to one past the end of all elements stored in this @ref HashMultiMap.
	ConstIterator ConstEnd() const;

	//!	Retrieves a @ref ConstLocalIterator to the first element stored in this @ref HashMultiMap.
	ConstLocalIterator Begin(SizeType bucketIndex) const;
	//!	Retrieves a @ref LocalIterator to the first element stored in this @ref HashMultiMap.
	LocalIterator Begin(SizeType bucketIndex);
	//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashMultiMap.
	ConstIterator Begin() const;
	//!	Retrieves an @ref Iterator to the first element stored in this @ref HashMultiMap.
	Iterator Begin();

	//! Retrieves a @ref ConstLocalIterator to one past the end of all elements stored in this @ref HashMultiMap.
	ConstLocalIterator End(SizeType bucketIndex) const;
	//!	Retrieves an @ref LocalIterator to one past the end of all elements stored in this @ref HashMultiMap.
	LocalIterator End(SizeType bucketIndex);
	//! Retrieves a @ref ConstIterator to one past the end of all elements stored in this @ref HashMultiMap.
	ConstIterator End() const;
	//!	Retrieves an @ref Iterator to one past the end of all elements stored in this @ref HashMultiMap.
	Iterator End();

	// - CONTAINER MANIPULATION --------------------------

public:
	Pair<Iterator, bool> Insert(const ValueType&);
	Pair<Iterator, bool> Insert(ValueType&&);

	template <typename... Arguments>
	Pair<Iterator, bool> Emplace(Arguments&&... arguments);

	Iterator Erase(Iterator begin, Iterator end);
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
	EqualityType GetEqualityPredicate() const ETNoexceptHint;

	HashType GetHash() const ETNoexceptHint;

	const AllocatorType& GetAllocator() const ETNoexceptHint;

	// - CONTAINER DUPLICATION ---------------------------

public:
	HashMultiMap& operator=(const HashMultiMap&) = default;
	HashMultiMap& operator=(HashMultiMap&&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Key2, typename Value2, class HashPredicate2, class EqualityPredicate2, class Allocator2, bool CacheHashCode2>
	friend void Swap(HashMultiMap<Key2, Value2, HashPredicate2, EqualityPredicate2, Allocator2, CacheHashCode2>&, HashMultiMap<Key2, Value2, HashPredicate2, EqualityPredicate2, Allocator2, CacheHashCode2>&);
};

template <typename Key, typename Value, class HashPredicate = Hash<Key>, class EqualityPredicate = EqualTo<Key>, class Allocator = MallocAllocator>
using CachingHashMultiMap = HashMultiMap<Key, Value, HashPredicate, EqualityPredicate, Allocator, true>;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/HashMultiMap.inl>
//------------------------------------------------------------------//
