/*==================================================================*\
  ArrayBvh.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/ChildAllocator.hpp>
#include <Common/Containers/ArrayList.hpp>
#include <Common/Mpl/IntTypes.hpp>
#include <Common/Pair.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, class Allocator = MallocAllocator>
class ArrayBvh {
	// - TYPE PUBLISHING ---------------------------------

public:
	using SplitIndex = uint32_t;
	using SortKey    = uintptr;

	// ---

public:
	struct Split {
		SplitIndex minima[3];
		SplitIndex maxima[3];

		SplitIndex leftChild;
		SplitIndex rightChild;
	};

	// ---

private:
	using LeafContainer  = ArrayList<Value, ChildAllocator>;
	using SplitContainer = ArrayList<Split, ChildAllocator>;

public:
	using ConstLeafIterator = typename LeafContainer::ConstIterator;
	using LeafIterator      = typename LeafContainer::Iterator;
	using SizeType          = typename LeafContainer::SizeType;
	using AllocatorType     = Allocator;
	using ValueType         = Value;
	using LeafType          = typename LeafContainer::ValueType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ArrayBvh instance.
	ArrayBvh(const AllocatorType& allocator);
	//!	Constructs this @ref ArrayBvh instance.
	ArrayBvh(const ArrayBvh&) = default;

	~ArrayBvh() = default;

	// ---------------------------------------------------

public:
	ConstLeafIterator ConstBegin() const;

	ConstLeafIterator ConstEnd() const;

	ConstLeafIterator Begin() const;
	LeafIterator      Begin();

	ConstLeafIterator End() const;
	LeafIterator      End();

	// ---------------------------------------------------

public:
	void Insert(const ValueType& value);

	LeafIterator Find(const ValueType& value);

	LeafIterator Erase(LeafIterator position);

	// ---------------------------------------------------

public:
	void Clear();

	// - DATA MEMBERS ------------------------------------

private:
	Allocator      _allocator;
	LeafContainer  _leaves;
	SplitContainer _splits;

	// ---------------------------------------------------

	template <typename Value, class Allocator>
	friend void Swap(ArrayBvh<Value, Allocator>&, ArrayBvh<Value, Allocator>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/ArrayBvh.inl>
//------------------------------------------------------------------//
