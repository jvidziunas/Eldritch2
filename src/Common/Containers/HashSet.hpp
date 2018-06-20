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
ET_PUSH_COMPILER_WARNING_STATE()
//	(4800) EASTL does some int to bool coercion MSVC doesn't like.
ET_SET_MSVC_WARNING_STATE(disable : 4800)
#include <eastl/hash_set.h>
ET_POP_COMPILER_WARNING_STATE()
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
	HashSet(const HashPredicate& hashPredicate, const EqualityPredicate& equalityPredicate, const AllocatorType& allocator);
	//! Constructs this @ref HashSet instance.
	template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
	HashSet(const HashSet&, const AllocatorType& allocator);
	//! Constructs this @ref HashSet instance.
	explicit HashSet(const AllocatorType& allocator);
	//! Constructs this @ref HashSet instance.
	HashSet(HashSet&&);

	~HashSet() = default;

	// - ALGORITHMS --------------------------------------

public:
	//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	template <typename AlternateValue, typename AlternateHashPredicate, typename AlternateEqualityPredicate>
	ConstIterator Find(const AlternateValue& value, AlternateHashPredicate hashPredicate, AlternateEqualityPredicate equalityPredicate) const;
	//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	template <typename AlternateValue, typename AlternateHashPredicate, typename AlternateEqualityPredicate>
	Iterator Find(const AlternateValue& value, AlternateHashPredicate hashPredicate, AlternateEqualityPredicate equalityPredicate);
	//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	ConstIterator Find(const ValueType& value) const;
	//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	Iterator Find(const ValueType& value);

	bool Contains(const ValueType& value) const;

	//! Erases all elements for which the result of the predicate returns true.
	template <typename Predicate>
	void EraseIf(Predicate predicate);

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

	// - CONTAINER DUPLICATION ---------------------------

public:
	template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
	HashSet& operator=(const HashSet&);
	HashSet& operator=(HashSet&&);

	// - CONTAINER MANIPULATION --------------------------

public:
	template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
	Pair<Iterator, bool> Insert(const ValueType& value);
	Pair<Iterator, bool> Insert(ValueType&& value);

	template <typename... Args>
	Pair<Iterator, bool> Emplace(Args&&... args);

	Iterator Erase(Iterator begin, Iterator end);
	Iterator Erase(Iterator position);
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

	// - ALLOCATOR ACCESS --------------------------------

public:
	const EqualityPredicateType& GetEqualityPredicate() const;

	const HashPredicateType& GetHashPredicate() const;

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
