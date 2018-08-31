/*==================================================================*\
  HashSet.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
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
#include <eastl/hash_set.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, class HashPredicate = Hash<Value>, class EqualityPredicate = EqualTo<Value>, class Allocator = MallocAllocator, bool cacheHashCode = false>
class HashSet {
	// - TYPE PUBLISHING ---------------------------------

protected:
	using UnderlyingContainer = eastl::hash_set<Value, HashPredicate, EqualityPredicate, EaStlAllocatorMixin<Allocator>, cacheHashCode>;

public:
	using ValueType             = typename UnderlyingContainer::value_type;
	using AllocatorType         = typename UnderlyingContainer::allocator_type::PublicType;
	using SizeType              = typename UnderlyingContainer::size_type;
	using Iterator              = typename UnderlyingContainer::iterator;
	using ConstIterator         = typename UnderlyingContainer::const_iterator;
	using LocalIterator         = typename UnderlyingContainer::local_iterator;
	using ConstLocalIterator    = typename UnderlyingContainer::const_local_iterator;
	using HashPredicateType     = HashPredicate;
	using EqualityPredicateType = EqualityPredicate;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref HashSet instance.
	HashSet(const AllocatorType& allocator = AllocatorType(), SizeType bucketCount = 0u, const HashPredicateType& hash = HashPredicateType(), const EqualityPredicateType& equal = EqualityPredicateType());
	//! Constructs this @ref HashSet instance.
	template <typename InputIterator>
	HashSet(const AllocatorType& allocator, SizeType bucketCount, const HashPredicateType& hash, const EqualityPredicateType& equal, InputIterator begin, InputIterator end);
	//! Constructs this @ref HashSet instance.
	HashSet(const AllocatorType& allocator, SizeType bucketCount, const HashPredicateType& hash, const EqualityPredicateType& equal, std::initializer_list<ValueType>);
	//! Constructs this @ref HashSet instance.
	HashSet(const AllocatorType& allocator, const HashSet&);
	//! Constructs this @ref HashSet instance.
	HashSet(const HashSet&) = default;
	//! Constructs this @ref HashSet instance.
	HashSet(HashSet&&) = default;

	~HashSet() = default;

	// - ALGORITHMS --------------------------------------

public:
	//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
	ConstIterator Find(const Value2& value, HashPredicate2 hash, EqualityPredicate2 equal) const;
	//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
	Iterator Find(const Value2& value, HashPredicate2 hash, EqualityPredicate2 equal);
	//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	ConstIterator Find(const ValueType& value) const;
	//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	Iterator Find(const ValueType& value);

	template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
	bool Contains(const Value2& value, HashPredicate2 hash, EqualityPredicate2 equal) const;
	bool Contains(const ValueType& value) const;

	//! Erases all elements for which the result of the predicate returns true.
	template <typename UnaryPredicate>
	void EraseIf(UnaryPredicate condition);

	// - ELEMENT ITERATION -------------------------------

public:
	//!	Retrieves a @ref ConstLocalIterator to the first element stored in the specified bucket of this @ref HashSet.
	ConstLocalIterator ConstBegin(SizeType bucketIndex) const;
	//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashSet.
	ConstIterator ConstBegin() const;

	//!	Retrieves a @ref ConstLocalIterator to one past the end of all elements stored in this @ref HashSet.
	ConstLocalIterator ConstEnd(SizeType bucketIndex) const;
	//!	Retrieves a @ref ConstIterator to one past the end of all elements stored in this @ref HashSet.
	ConstIterator ConstEnd() const;

	//!	Retrieves a @ref ConstLocalIterator to the first element stored in this @ref HashSet.
	ConstLocalIterator Begin(SizeType bucketIndex) const;
	//!	Retrieves a @ref LocalIterator to the first element stored in this @ref HashSet.
	LocalIterator Begin(SizeType bucketIndex);
	//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashSet.
	ConstIterator Begin() const;
	//!	Retrieves an @ref Iterator to the first element stored in this @ref HashSet.
	Iterator Begin();

	//! Retrieves a @ref ConstLocalIterator to one past the end of all elements stored in this @ref HashSet.
	ConstIterator End(SizeType bucketIndex) const;
	//!	Retrieves an @ref LocalIterator to one past the end of all elements stored in this @ref HashSet.
	Iterator End(SizeType bucketIndex);
	//! Retrieves a @ref ConstIterator to one past the end of all elements stored in this @ref HashSet.
	ConstIterator End() const;
	//!	Retrieves an @ref Iterator to one past the end of all elements stored in this @ref HashSet.
	Iterator End();

	// - CONTAINER MANIPULATION --------------------------

public:
	Pair<Iterator, bool> Insert(const ValueType& value);
	Pair<Iterator, bool> Insert(ValueType&& value);

	template <typename... Args>
	Pair<Iterator, bool> Emplace(Args&&... args);

	Iterator Erase(Iterator begin, Iterator end);
	Iterator Erase(Iterator where);
	SizeType Erase(const ValueType& key);

	void Clear();

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const;

	bool IsEmpty() const;

	explicit operator bool() const;

	// - CAPACITY QUERY ----------------------------------

public:
	void Reserve(SizeType minimumSizeHint);

	// - CONTAINER DUPLICATION ---------------------------

public:
	HashSet& operator=(const HashSet&) = default;
	HashSet& operator=(HashSet&&) = default;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const EqualityPredicateType& GetEqualityPredicate() const;

	const HashPredicateType& GetHash() const;

	const AllocatorType& GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	friend void Swap(HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>&, HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>&);
};

template <typename Value, class HashPredicate = Hash<Value>, class EqualityPredicate = EqualTo<Value>, class Allocator = MallocAllocator>
using CachingHashSet = HashSet<Value, HashPredicate, EqualityPredicate, Allocator, true>;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/HashSet.inl>
//------------------------------------------------------------------//
