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
#include <Common/Containers/SoArrayList.hpp>
#include <Common/Memory/ChildAllocator.hpp>
#include <Common/Containers/ArrayList.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, class Allocator = MallocAllocator>
class ArrayBvh {
	// - TYPE PUBLISHING ---------------------------------

public:
	using LeafIndex  = uint32;
	using SplitIndex = uint32;
	using SortKey    = uintptr;

	// ---

public:
	struct Split {
		LeafIndex minima[3];
		LeafIndex maxima[3];

		SplitIndex leftChild;
		SplitIndex rightChild;
	};

	// ---

	enum : size_t {
		TargetKey,
		SourceKey,
	};

	// ---

private:
	using LeafContainer  = SoArrayListAlloc<ChildAllocator, SortKey /*targetKey*/, SortKey /*sourceKey*/, Value>;
	using SplitContainer = ArrayList<Split, ChildAllocator>;

public:
	using SplitConstIterator = typename SplitContainer::ConstIterator;
	using SplitIterator      = typename SplitContainer::Iterator;
	using LeafConstIterator  = typename LeafContainer::ConstIterator;
	using LeafIterator       = typename LeafContainer::Iterator;
	using SizeType           = typename LeafContainer::SizeType;
	using AllocatorType      = Allocator;
	using ValueType          = Value;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ArrayBvh instance.
	ArrayBvh(const AllocatorType& allocator);
	//!	Constructs this @ref ArrayBvh instance.
	ArrayBvh(const ArrayBvh&) = default;

	~ArrayBvh() = default;

	// ---------------------------------------------------

public:
	SplitConstIterator ConstBegin() const ETNoexceptHint;

	SplitConstIterator ConstEnd() const ETNoexceptHint;

	SplitConstIterator Begin() const ETNoexceptHint;

	SplitConstIterator End() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	LeafConstIterator Find(const ValueType& value) const ETNoexceptHint;
	LeafIterator      Find(const ValueType& value) ETNoexceptHint;

	// ---------------------------------------------------

public:
	void Insert(const ValueType& value);

	LeafIterator Erase(const ValueType& value);

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
