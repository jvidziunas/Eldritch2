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

template <typename Value, class HashPredicate = Hash<Value>, class EqualityPredicate = EqualTo<Value>, class Allocator = MallocAllocator, bool CacheHashCode = false>
class HashSet {
	// - TYPE PUBLISHING ---------------------------------

protected:
	using UnderlyingContainer = eastl::hash_set<Value, HashPredicate, EqualityPredicate, EaStlAllocatorMixin<Allocator>, CacheHashCode>;

public:
	using ValueType             = typename UnderlyingContainer::value_type;
	using ConstReference        = typename UnderlyingContainer::const_reference;
	using Reference             = typename UnderlyingContainer::reference;
	using AllocatorType         = typename UnderlyingContainer::allocator_type::PublicType;
	using ConstIterator         = typename UnderlyingContainer::const_iterator;
	using Iterator              = typename UnderlyingContainer::iterator;
	using ConstLocalIterator    = typename UnderlyingContainer::const_local_iterator;
	using LocalIterator         = typename UnderlyingContainer::local_iterator;
	using SizeType              = typename UnderlyingContainer::size_type;
	using HashPredicateType     = HashPredicate;
	using EqualityPredicateType = EqualityPredicate;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref HashSet instance.
	HashSet(const AllocatorType& allocator = AllocatorType(), SizeType buckets = 0u, const HashPredicateType& hash = HashPredicateType(), const EqualityPredicateType& equal = EqualityPredicateType());
	//! Constructs this @ref HashSet instance.
	template <typename InputIterator>
	HashSet(const AllocatorType& allocator, SizeType buckets, const HashPredicateType& hash, const EqualityPredicateType& equal, InputIterator begin, InputIterator end);
	//! Constructs this @ref HashSet instance.
	HashSet(const AllocatorType& allocator, SizeType buckets, const HashPredicateType& hash, const EqualityPredicateType& equal, InitializerList<ValueType>);
	//! Constructs this @ref HashSet instance.
	HashSet(const AllocatorType& allocator, const HashSet&);
	//! Constructs this @ref HashSet instance.
	HashSet(const HashSet&) = default;
	//! Constructs this @ref HashSet instance.
	HashSet(HashSet&&) ETNoexceptHint = default;

	~HashSet() = default;

	// - ALGORITHMS --------------------------------------

public:
	//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
	ConstIterator Find(const Value2& value, HashPredicate2 hash, EqualityPredicate2 equal) const ETNoexceptHint;
	//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
	Iterator Find(const Value2& value, HashPredicate2 hash, EqualityPredicate2 equal) ETNoexceptHint;
	//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	ConstIterator Find(ConstReference) const ETNoexceptHint;
	//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
	Iterator Find(ConstReference) ETNoexceptHint;

	template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
	bool Contains(const Value2& value, HashPredicate2 hash, EqualityPredicate2 equal) const ETNoexceptHint;
	bool Contains(ConstReference) const ETNoexceptHint;

	//! Erases all elements for which the result of the predicate returns true.
	template <typename UnaryPredicate>
	void EraseIf(UnaryPredicate condition);

	// - ELEMENT ITERATION -------------------------------

public:
	//!	Retrieves a @ref ConstLocalIterator to the first element stored in the specified bucket of this @ref HashSet.
	ConstLocalIterator ConstBegin(SizeType bucket) const ETNoexceptHint;
	//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashSet.
	ConstIterator ConstBegin() const ETNoexceptHint;

	//!	Retrieves a @ref ConstLocalIterator to one past the end of all elements stored in this @ref HashSet.
	ConstLocalIterator ConstEnd(SizeType bucket) const ETNoexceptHint;
	//!	Retrieves a @ref ConstIterator to one past the end of all elements stored in this @ref HashSet.
	ConstIterator ConstEnd() const ETNoexceptHint;

	//!	Retrieves a @ref ConstLocalIterator to the first element stored in this @ref HashSet.
	ConstLocalIterator Begin(SizeType bucket) const ETNoexceptHint;
	//!	Retrieves a @ref LocalIterator to the first element stored in this @ref HashSet.
	LocalIterator Begin(SizeType bucket) ETNoexceptHint;
	//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashSet.
	ConstIterator Begin() const ETNoexceptHint;
	//!	Retrieves an @ref Iterator to the first element stored in this @ref HashSet.
	Iterator Begin() ETNoexceptHint;

	//! Retrieves a @ref ConstLocalIterator to one past the end of all elements stored in this @ref HashSet.
	ConstIterator End(SizeType bucket) const ETNoexceptHint;
	//!	Retrieves an @ref LocalIterator to one past the end of all elements stored in this @ref HashSet.
	Iterator End(SizeType bucket) ETNoexceptHint;
	//! Retrieves a @ref ConstIterator to one past the end of all elements stored in this @ref HashSet.
	ConstIterator End() const ETNoexceptHint;
	//!	Retrieves an @ref Iterator to one past the end of all elements stored in this @ref HashSet.
	Iterator End() ETNoexceptHint;

	// - CONTAINER MANIPULATION --------------------------

public:
	Pair<Iterator, bool> Insert(ConstReference);
	Pair<Iterator, bool> Insert(ValueType&&);

	template <typename... Args>
	Pair<Iterator, bool> Emplace(Args&&... args);

	Iterator Erase(Iterator begin, Iterator end);
	Iterator Erase(Iterator where);
	template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
	SizeType Erase(const Value2& value, HashPredicate2 hash, EqualityPredicate2 equal);
	SizeType Erase(ConstReference);

	template <typename UnaryPredicate>
	void ClearAndDispose(UnaryPredicate disposer) ETNoexceptHint(noexcept(disposer(Declval<Reference>())));

	void Clear() ETNoexceptHint;

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const ETNoexceptHint;

	bool IsEmpty() const ETNoexceptHint;

	explicit operator bool() const ETNoexceptHint;

	// - CAPACITY QUERY ----------------------------------

public:
	void Reserve(SizeType minimumSizeHint);

	// - CONTAINER DUPLICATION ---------------------------

public:
	HashSet& operator=(const HashSet&) = default;
	HashSet& operator=(HashSet&&) ETNoexceptHint = default;

	// - ALLOCATOR ACCESS --------------------------------

public:
	EqualityPredicateType GetEqualityPredicate() const ETNoexceptHint;

	HashPredicateType GetHash() const ETNoexceptHint;

	const AllocatorType& GetAllocator() const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Value2, class HashPredicate2, class EqualityPredicate2, class Allocator2, bool CacheHashCode2>
	friend void Swap(HashSet<Value2, HashPredicate2, EqualityPredicate2, Allocator2, CacheHashCode2>&, HashSet<Value2, HashPredicate2, EqualityPredicate2, Allocator2, CacheHashCode2>&) ETNoexceptHint;
};

template <typename Value, class HashPredicate = Hash<Value>, class EqualityPredicate = EqualTo<Value>, class Allocator = MallocAllocator>
using CachingHashSet = HashSet<Value, HashPredicate, EqualityPredicate, Allocator, true>;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/HashSet.inl>
//------------------------------------------------------------------//
