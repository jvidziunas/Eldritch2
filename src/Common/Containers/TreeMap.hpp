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
	using ValueType      = typename UnderlyingContainer::value_type;
	using KeyType        = typename UnderlyingContainer::key_type;
	using MappedType     = typename UnderlyingContainer::data_type;
	using ConstPointer   = typename UnderlyingContainer::const_pointer;
	using Pointer        = typename UnderlyingContainer::pointer;
	using ConstReference = typename UnderlyingContainer::const_reference;
	using Reference      = typename UnderlyingContainer::reference;
	using ConstIterator  = typename UnderlyingContainer::const_iterator;
	using Iterator       = typename UnderlyingContainer::iterator;
	using AllocatorType  = typename UnderlyingContainer::allocator_type::PublicType;
	using SizeType       = typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref TreeMap instance.
	TreeMap(const AllocatorType& allocator = AllocatorType(), const SortPredicate& sort = SortPredicate());
	//!	Constructs this @ref TreeMap instance.
	template <typename InputIterator>
	TreeMap(const AllocatorType& allocator, const SortPredicate& sort, InputIterator begin, InputIterator end);
	//!	Constructs this @ref TreeMap instance.
	TreeMap(const AllocatorType& allocator, const SortPredicate& sort, InitializerList<ValueType>);
	//!	Constructs this @ref TreeMap instance.
	TreeMap(const AllocatorType& allocator, const TreeMap&);
	//!	Constructs this @ref TreeMap instance.
	TreeMap(const TreeMap&) = default;
	//!	Constructs this @ref TreeMap instance.
	TreeMap(TreeMap&&) = default;

	~TreeMap() = default;

	// - ALGORITHMS --------------------------------------

public:
	ConstIterator Find(const KeyType& key) const;
	Iterator      Find(const KeyType& key);

	template <typename UnaryPredicate>
	void EraseIf(UnaryPredicate condition);

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
	MappedType& operator[](const KeyType& key);

	// - CONTAINER DUPLICATION ---------------------------

public:
	TreeMap& operator=(const TreeMap&) = default;
	TreeMap& operator=(TreeMap&&) = default;

	// - CONTAINER MANIPULATION --------------------------

public:
	template <typename... Arguments>
	Pair<Iterator, bool> Emplace(Arguments&&... arguments);

	Pair<Iterator, bool> Insert(const ValueType& value);
	Pair<Iterator, bool> Insert(ValueType&& value);

	Iterator Erase(Iterator begin, Iterator end);
	Iterator Erase(Iterator where);
	SizeType Erase(const KeyType& key);

	template <typename UnaryPredicate>
	void ClearAndDispose(UnaryPredicate disposer);

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
