/*==================================================================*\
  ArrayMap.hpp
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
#include <Common/LessThan.hpp>
#include <Common/Pair.hpp>
//------------------------------------------------------------------//
#include <eastl/vector_map.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Key, typename Value, class SortPredicate = LessThan<Key>, class Allocator = MallocAllocator>
class ArrayMap {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::vector_map<Key, Value, SortPredicate, EaStlAllocatorMixin<Allocator>>;

public:
	using ValueType            = typename UnderlyingContainer::value_type;
	using KeyType              = typename UnderlyingContainer::key_type;
	using MappedType           = typename UnderlyingContainer::mapped_type;
	using SortPredicateType    = typename UnderlyingContainer::key_compare;
	using AllocatorType        = typename UnderlyingContainer::allocator_type::PublicType;
	using Iterator             = typename UnderlyingContainer::iterator;
	using ReverseIterator      = typename UnderlyingContainer::reverse_iterator;
	using ConstIterator        = typename UnderlyingContainer::const_iterator;
	using ConstReverseIterator = typename UnderlyingContainer::const_reverse_iterator;
	using SizeType             = typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref ArrayMap instance.
	template <typename InputIterator>
	ArrayMap(InputIterator begin, InputIterator end, const SortPredicateType& sortPredicate, const AllocatorType& allocator = AllocatorType());
	//! Constructs this @ref ArrayMap instance.
	explicit ArrayMap(const AllocatorType& allocator = AllocatorType());
	//! Constructs this @ref ArrayMap instance.
	ArrayMap(const ArrayMap&);
	//! Constructs this @ref ArrayMap instance.
	ArrayMap(ArrayMap&&);

	~ArrayMap() = default;

	// - ALGORITHMS --------------------------------------

public:
	ConstIterator LowerBound(const KeyType& key) const;
	Iterator      LowerBound(const KeyType& key);

	Iterator UpperBound(const KeyType& key) const;
	Iterator UpperBound(const KeyType& key);

	template <typename AlternateKeyType, typename AlternateSortPredicate>
	ConstIterator Find(const AlternateKeyType& key, AlternateSortPredicate sortPredicate) const;
	template <typename AlternateKeyType, typename AlternateSortPredicate>
	Iterator      Find(const AlternateKeyType& key, AlternateSortPredicate sortPredicate);
	ConstIterator Find(const KeyType& key) const;
	Iterator      Find(const KeyType& key);

	template <typename AlternateKey, typename AlternateSortPredicate>
	bool ContainsKey(const AlternateKey& key, const AlternateSortPredicate& sortPredicate) const;
	bool ContainsKey(const KeyType& key) const;

	// - ELEMENT ITERATION -------------------------------

public:
	ConstReverseIterator ConstReverseBegin() const;

	ConstReverseIterator ConstReverseEnd() const;

	ConstReverseIterator ReverseBegin() const;
	ReverseIterator      ReverseBegin();

	ConstReverseIterator ReverseEnd() const;
	ReverseIterator      ReverseEnd();

	ConstIterator ConstBegin() const;

	ConstIterator ConstEnd() const;

	ConstIterator Begin() const;
	Iterator      Begin();

	ConstIterator End() const;
	Iterator      End();

	// - CONTAINER MANIPULATION --------------------------

public:
	MappedType& operator[](const KeyType& key);
	MappedType& operator[](KeyType&& key);

	Pair<Iterator, bool> Insert(const ValueType& value);
	Pair<Iterator, bool> Insert(ValueType&& value);

	template <typename... Args>
	Pair<Iterator, bool> TryEmplace(const KeyType& key, Args&&... args);
	template <typename... Args>
	Pair<Iterator, bool> TryEmplace(KeyType&& key, Args&&... args);
	template <typename... Args>
	Pair<Iterator, bool> Emplace(ConstIterator position, Args&&... args);
	template <typename... Args>
	Pair<Iterator, bool> Emplace(Args&&... args);

	Iterator Erase(Iterator begin, Iterator end);
	Iterator Erase(Iterator position);
	SizeType Erase(const KeyType& key);

	void Clear();

	// ---------------------------------------------------

public:
	ArrayMap& operator=(const ArrayMap&);
	ArrayMap& operator=(ArrayMap&&);

	// ---------------------------------------------------

public:
	void SetCapacity(SizeType sizeInElements);

	void Reserve(SizeType sizeInElements);

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const;

	bool IsEmpty() const;

	explicit operator bool() const;

	// - ORDERING PREDICATE ACCESS -----------------------

public:
	const SortPredicateType& GetSortPredicate() const;
	SortPredicateType&       GetSortPredicate();

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator();

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Key, typename Value, class SortPredicate, class Allocator>
	friend void Swap(ArrayMap<Key, Value, SortPredicate, Allocator>&, ArrayMap<Key, Value, SortPredicate, Allocator>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/ArrayMap.inl>
//------------------------------------------------------------------//
