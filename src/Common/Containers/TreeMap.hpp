/*==================================================================*\
  TreeMap.hpp
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
#include <eastl/map.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Key, typename Value, typename SortPredicate = LessThan<Key>, class Allocator = MallocAllocator>
class TreeMap {
	// - TYPE PUBLISHING ---------------------------------

protected:
	using UnderlyingContainer = eastl::map<Key, Value, SortPredicate, EaStlAllocatorMixin<Allocator>>;

public:
	using ValueType     = typename UnderlyingContainer::value_type;
	using KeyType       = typename UnderlyingContainer::key_type;
	using MappedType    = typename UnderlyingContainer::data_type;
	using AllocatorType = typename UnderlyingContainer::allocator_type::PublicType;
	using Iterator      = typename UnderlyingContainer::iterator;
	using ConstIterator = typename UnderlyingContainer::const_iterator;
	using SizeType      = typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref TreeMap instance.
	template <typename InputIterator>
	TreeMap(InputIterator begin, InputIterator end, const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref TreeMap instance.
	template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
	TreeMap(const TreeMap&, const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref TreeMap instance.
	explicit TreeMap(const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref TreeMap instance.
	TreeMap(TreeMap&&) = default;

	~TreeMap() = default;

	// - ALGORITHMS --------------------------------------

public:
	ConstIterator Find(const KeyType& key) const;
	Iterator      Find(const KeyType& key);

	template <typename Predicate>
	void EraseIf(Predicate predicate);

	// - ELEMENT ITERATION -------------------------------

public:
	ConstIterator ConstBegin() const;

	ConstIterator ConstEnd() const;

	ConstIterator Begin() const;
	Iterator      Begin();

	ConstIterator End() const;
	Iterator      End();

	// - ELEMENT ACCESS ----------------------------------

public:
	template <class = eastl::is_default_constructible<ValueType>::value>
	MappedType& operator[](const KeyType& key);

	// - CONTAINER DUPLICATION ---------------------------

public:
	TreeMap& operator=(const TreeMap&) = default;
	TreeMap& operator=(TreeMap&&) = default;

	// - CONTAINER MANIPULATION --------------------------

public:
	template <typename... ConstructorArguments>
	Pair<Iterator, bool> Emplace(ConstructorArguments&&... constructorArguments);

	Pair<Iterator, bool> Insert(const ValueType& value);
	Pair<Iterator, bool> Insert(ValueType&& value);

	Iterator Erase(Iterator begin, Iterator end);
	Iterator Erase(Iterator position);
	SizeType Erase(const KeyType& key);

	void Clear();

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const;

	bool IsEmpty() const;

	explicit operator bool() const;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Key, typename Value, typename SortPredicate, class Allocator>
	friend void Swap(TreeMap<Key, Value, SortPredicate, Allocator>&, TreeMap<Key, Value, SortPredicate, Allocator>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/TreeMap.inl>
//------------------------------------------------------------------//
