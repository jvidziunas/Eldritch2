/*==================================================================*\
  ArraySet.hpp
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
#include <eastl/vector_set.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, class SortPredicate = LessThan<Value>, class Allocator = MallocAllocator>
class ArraySet {
	// - TYPE PUBLISHING ---------------------------------

protected:
	using UnderlyingContainer = eastl::vector_set<Value, SortPredicate, EaStlAllocatorMixin<Allocator>>;

public:
	using ValueType      = typename UnderlyingContainer::value_type;
	using ConstPointer   = typename UnderlyingContainer::const_pointer;
	using Pointer        = typename UnderlyingContainer::pointer;
	using ConstReference = typename UnderlyingContainer::const_reference;
	using Reference      = typename UnderlyingContainer::reference;
	using AllocatorType  = typename UnderlyingContainer::allocator_type::PublicType;
	using ConstIterator  = typename UnderlyingContainer::const_iterator;
	using Iterator       = typename UnderlyingContainer::iterator;
	using SizeType       = typename UnderlyingContainer::size_type;
	using SorterType     = SortPredicate;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ArraySet instance.
	ArraySet(const AllocatorType& allocator = AllocatorType(), const SorterType& sort = SorterType(), SizeType capacityHint = 0u);
	//!	Constructs this @ref ArraySet instance.
	template <typename InputIterator>
	ArraySet(const AllocatorType& allocator, const SorterType& sort, InputIterator begin, InputIterator end);
	//!	Constructs this @ref ArraySet instance.
	ArraySet(const AllocatorType& allocator, const SorterType& sort, InitializerList<ValueType>);
	//!	Constructs this @ref ArraySet instance.
	ArraySet(const AllocatorType& allocator, const ArraySet&);
	//!	Constructs this @ref ArraySet instance.
	ArraySet(const ArraySet&) = default;
	//!	Constructs this @ref ArraySet instance.
	ArraySet(ArraySet&&) = default;

	~ArraySet() = default;

	// - ALGORITHMS --------------------------------------

public:
	ConstIterator LowerBound(const ValueType& value) const;
	Iterator      LowerBound(const ValueType& value);

	ConstIterator UpperBound(const ValueType& value) const;
	Iterator      UpperBound(const ValueType& value);

	template <typename Value2, typename SortPredicate2>
	ConstIterator Find(const Value2& value, SortPredicate2 sort) const;
	template <typename Value2, typename SortPredicate2>
	Iterator      Find(const Value2& value, SortPredicate2 sort);
	ConstIterator Find(const ValueType& value) const;
	Iterator      Find(const ValueType& value);

	template <typename Value2, typename SortPredicate2>
	bool Contains(const Value2& value, SortPredicate2 sort) const;
	bool Contains(const ValueType& value) const;

	template <typename UnaryPredicate>
	void RemoveIf(UnaryPredicate filter);

	// - ELEMENT ITERATION -------------------------------

public:
	ConstIterator ConstBegin() const;

	ConstIterator ConstEnd() const;

	ConstIterator Begin() const;
	Iterator      Begin();

	ConstIterator End() const;
	Iterator      End();

	// - ELEMENT ACCESS ----------------------------------

	ConstPointer GetData() const;
	Pointer      GetData();

	// - CONTAINER MANIPULATION --------------------------

public:
	template <typename... Arguments>
	Pair<Iterator, bool> Emplace(Arguments&&... arguments);

	Pair<Iterator, bool> Insert(const ValueType& value);
	Pair<Iterator, bool> Insert(ValueType&& value);

	Iterator Erase(Iterator begin, Iterator end);
	Iterator Erase(Iterator where);
	SizeType Erase(const ValueType& value);

	template <typename UnaryPredicate>
	void ClearAndDispose(UnaryPredicate disposer);

	void Clear();

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const;

	bool IsEmpty() const;

	explicit operator bool() const;

	// - CAPACITY QUERY ----------------------------------

public:
	SizeType GetCapacity() const;

	void SetCapacity(SizeType capacity);

	void Reserve(SizeType sizeHint);

	// - SORT PREDICATE ACCESS ---------------------------

public:
	const SorterType& GetSorter() const;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const;

	// - CONTAINER DUPLICATION ---------------------------

public:
	ArraySet& operator=(const ArraySet&) = default;
	ArraySet& operator=(ArraySet&&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	friend void Swap(ArraySet<Value, SortPredicate, Allocator>&, ArraySet<Value, SortPredicate, Allocator>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/ArraySet.inl>
//------------------------------------------------------------------//
