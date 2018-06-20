/*==================================================================*\
  TreeSet.hpp
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
//------------------------------------------------------------------//
#include <eastl/set.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, typename SortPredicate = LessThan<Value>, class Allocator = MallocAllocator>
class TreeSet {
	// - TYPE PUBLISHING ---------------------------------

protected:
	using UnderlyingContainer = eastl::set<Value, LessThan<Value>, EaStlAllocatorMixin<Allocator>>;

public:
	using ValueType     = typename UnderlyingContainer::value_type;
	using AllocatorType = typename UnderlyingContainer::allocator_type::PublicType;
	using Iterator      = typename UnderlyingContainer::iterator;
	using ConstIterator = typename UnderlyingContainer::const_iterator;
	using SizeType      = typename AllocatorType::SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref TreeSet instance.
	explicit TreeSet(const AllocatorType& allocator);
	//!	Constructs this @ref TreeSet instance.
	TreeSet(const TreeSet&) = default;
	//!	Constructs this @ref TreeSet instance.
	TreeSet(TreeSet&&) = default;

	~TreeSet() = default;

	// - ALGORITHMS --------------------------------------

public:
	ConstIterator Find(const ValueType& value) const;
	Iterator      Find(const ValueType& value);

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
	TreeSet& operator=(const TreeSet&) = default;
	TreeSet& operator=(TreeSet&&) = default;

	// - CONTAINER MANIPULATION --------------------------

public:
	Pair<Iterator, bool> Insert(const ValueType& value);
	Pair<Iterator, bool> Insert(ValueType&& value);

	Iterator Erase(Iterator begin, Iterator end);
	Iterator Erase(Iterator position);

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

	template <typename Value, typename SortPredicate, class Allocator>
	friend void Swap(TreeSet<Value, SortPredicate, Allocator>&, TreeSet<Value, SortPredicate, Allocator>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/TreeSet.inl>
//------------------------------------------------------------------//
