/*==================================================================*\
  IntrusiveHashTable.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/RangeAdapters.hpp>
//------------------------------------------------------------------//
#include <eastl/internal/intrusive_hashtable.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Key, typename Value, typename HashPredicate, typename EqualityPredicate, size_t BucketCount, bool UseConstIterators, bool UseUniqueKeys>
class IntrusiveHashTable {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::intrusive_hashtable<Key, Value, HashPredicate, EqualityPredicate, BucketCount, UseConstIterators, UseUniqueKeys>;

public:
	using KeyType            = typename UnderlyingContainer::key_type;
	using ValueType          = typename UnderlyingContainer::value_type;
	using ConstReference     = typename UnderlyingContainer::const_reference;
	using Reference          = typename UnderlyingContainer::reference;
	using ConstIterator      = typename UnderlyingContainer::const_iterator;
	using Iterator           = typename UnderlyingContainer::iterator;
	using ConstLocalIterator = typename UnderlyingContainer::const_local_iterator;
	using LocalIterator      = typename UnderlyingContainer::local_iterator;
	using SizeType           = typename UnderlyingContainer::size_type;
	using InsertReturnType   = typename UnderlyingContainer::insert_return_type;
	using EqualityType       = EqualityPredicate;
	using HashType           = Hash;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref IntrusiveHashTable instance.
	IntrusiveHashTable(const HashType&, const EqualityType&) ETNoexceptHintIf(IsNoThrowCopyConstructible<HashType>() && IsNoThrowCopyConstructible<EqualityType>());
	//!	Disable copy construction.
	IntrusiveHashTable(const IntrusiveHashTable&) = delete;

	~IntrusiveHashTable() = default;

	// - ALGORITHMS --------------------------------------

public:
	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	ConstIterator Find(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal) const;
	template <typename Key2, typename HashPredicate2, typename EqualityPredicate2>
	Iterator      Find(const Key2& key, HashPredicate2 hash, EqualityPredicate2 equal);
	ConstIterator Find(const KeyType& key) const;
	Iterator      Find(const KeyType& key);

	// - ELEMENT ITERATION -------------------------------

public:
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
	InsertReturnType Insert(Reference value);

	Iterator Erase(ConstIterator first, ConstIterator last);
	Iterator Erase(ConstIterator);
	Iterator Erase(Reference value);
	SizeType Erase(const KeyType&);

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize(SizeType bucket) const ETNoexceptHint;
	SizeType GetSize() const ETNoexceptHint;

	bool IsEmpty() const ETNoexceptHint;

	explicit operator bool() const ETNoexceptHint;

	// - INDEXING ACCESS ---------------------------------

public:
	EqualityType GetEquality() const ETNoexceptHint;

	HashType GetHash() const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Key2, typename Value2, typename HashPredicate2, typename EqualityPredicate2, size_t BucketCount2, bool UseConstIterators2, bool UseUniqueKeys2>
	friend void Swap(IntrusiveHashTable<Key2, Value2, HashPredicate2, EqualityPredicate2, BucketCount2, UseConstIterators2, UseUniqueKeys2>&, IntrusiveHashTable<Key2, Value2, HashPredicate2, EqualityPredicate2, BucketCount2, UseConstIterators2, UseUniqueKeys2>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/IntrusiveHashTable.inl>
//------------------------------------------------------------------//