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
	using ValueType     = typename UnderlyingContainer::value_type;
	using ConstPointer  = typename UnderlyingContainer::const_pointer;
	using Pointer       = typename UnderlyingContainer::pointer;
	using AllocatorType = typename UnderlyingContainer::allocator_type::PublicType;
	using ConstIterator = typename UnderlyingContainer::const_iterator;
	using Iterator      = typename UnderlyingContainer::iterator;
	using SizeType      = typename UnderlyingContainer::size_type;
	using SorterType    = SortPredicate;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ArraySet instance.
	template <typename InputIterator>
	ArraySet(InputIterator begin, InputIterator end, const SorterType& orderingPredicate = SorterType(), const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref ArraySet instance.
	ArraySet(SizeType reservedSizeInElements, const SorterType& orderingPredicate = SorterType(), const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref ArraySet instance.
	template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
	ArraySet(const ArraySet& set, const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref ArraySet instance.
	ArraySet(const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref ArraySet instance.
	ArraySet(ArraySet&&);

	~ArraySet() = default;

	// - ALGORITHMS --------------------------------------

public:
	ConstIterator LowerBound(const ValueType& value) const;
	Iterator      LowerBound(const ValueType& value);

	ConstIterator UpperBound(const ValueType& value) const;
	Iterator      UpperBound(const ValueType& value);

	template <typename AlternateValue, typename BinaryPredicate>
	ConstIterator Find(const AlternateValue& value, BinaryPredicate comparisonPredicate) const;
	template <typename AlternateValue, typename BinaryPredicate>
	Iterator      Find(const AlternateValue& value, BinaryPredicate comparisonPredicate);
	ConstIterator Find(const ValueType& value) const;
	Iterator      Find(const ValueType& value);

	template <typename AlternateValue, typename BinaryPredicate>
	bool Contains(const AlternateValue& value, BinaryPredicate comparisonPredicate) const;
	bool Contains(const ValueType& value) const;

	template <typename Predicate>
	void RemoveIf(Predicate predicate);

	// - ELEMENT ITERATION -------------------------------

public:
	ConstIterator ConstBegin() const;

	ConstIterator ConstEnd() const;

	ConstIterator Begin() const;
	Iterator      Begin();

	ConstIterator End() const;
	Iterator      End();

	// - CONTAINER DUPLICATION ---------------------------

public:
	ArraySet& operator=(const ArraySet&) = default;
	ArraySet& operator=(ArraySet&&) = default;

	// - ELEMENT ACCESS ----------------------------------

	ConstPointer GetData() const;
	Pointer      GetData();

	// - CONTAINER MANIPULATION --------------------------

public:
	template <typename... Arguments>
	Pair<Iterator, bool> Emplace(Arguments&&... arguments);

	template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
	Pair<Iterator, bool> Insert(const ValueType& value);
	Pair<Iterator, bool> Insert(ValueType&& value);

	Iterator Erase(Iterator beginPosition, Iterator endPosition);
	Iterator Erase(Iterator position);
	SizeType Erase(const ValueType& value);

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
