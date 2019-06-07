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
	using ConstPointer         = typename UnderlyingContainer::const_pointer;
	using Pointer              = typename UnderlyingContainer::pointer;
	using ConstReference       = typename UnderlyingContainer::const_reference;
	using Reference            = typename UnderlyingContainer::reference;
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
	ArrayMap(const AllocatorType& allocator, const SortPredicateType& sort, InputIterator begin, InputIterator end);
	//! Constructs this @ref ArrayMap instance.
	ArrayMap(const AllocatorType& allocator, const SortPredicateType& sort, InitializerList<ValueType>);
	//! Constructs this @ref ArrayMap instance.
	ArrayMap(const AllocatorType& allocator = AllocatorType(), const SortPredicateType& sort = SortPredicateType(), SizeType capacity = 0u);
	//! Constructs this @ref ArrayMap instance.
	ArrayMap(const ArrayMap&) = default;
	//! Constructs this @ref ArrayMap instance.
	ArrayMap(ArrayMap&&) = default;

	~ArrayMap() = default;

	// - ALGORITHMS --------------------------------------

public:
	ConstIterator LowerBound(const KeyType& key) const;
	Iterator      LowerBound(const KeyType& key);

	Iterator UpperBound(const KeyType& key) const;
	Iterator UpperBound(const KeyType& key);

	template <typename Key2, typename SortPredicate2>
	ConstIterator Find(const Key2& key, SortPredicate2 sort) const;
	template <typename Key2, typename SortPredicate2>
	Iterator      Find(const Key2& key, SortPredicate2 sort);
	ConstIterator Find(const KeyType& key) const;
	Iterator      Find(const KeyType& key);

	template <typename Key2, typename SortPredicate2>
	bool Contains(const Key2& key, SortPredicate2 sort) const;
	bool Contains(const KeyType& key) const;

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

	template <typename... Arguments>
	Pair<Iterator, bool> TryEmplace(const KeyType& key, Arguments&&... arguments);
	template <typename... Arguments>
	Pair<Iterator, bool> TryEmplace(KeyType&& key, Arguments&&... arguments);
	template <typename... Arguments>
	Pair<Iterator, bool> Emplace(ConstIterator position, Arguments&&... arguments);
	template <typename... Arguments>
	Pair<Iterator, bool> Emplace(Arguments&&... arguments);

	Iterator Erase(Iterator begin, Iterator end);
	Iterator Erase(Iterator where);
	SizeType Erase(const KeyType& key);

	template <typename UnaryPredicate>
	void ClearAndDispose(UnaryPredicate disposer);

	void Clear();

	// ---------------------------------------------------

public:
	ArrayMap& operator=(const ArrayMap&) = default;
	ArrayMap& operator=(ArrayMap&&) = default;

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
