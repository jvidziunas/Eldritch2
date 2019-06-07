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
#include <Common/Containers/SoaList.hpp>
#include <Common/Mpl/Vector.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, typename SortPredicate, class Allocator = MallocAllocator>
class ArrayBvh {
	// - TYPE PUBLISHING ---------------------------------

public:
	enum : size_t {
		Values,
		Keys,
		Splits,
	};

	// ---

public:
	template <typename Index>
	struct Split {
		Index minima[3];
		Index maxima[3];
		Index leftChild;
		Index rightChild;
		Box   bounds;
	};

	// ---
	
public:
	using LeafList           = SoaListAlloc<Allocator, Value /*values*/, typename SortPredicate::KeyType /*keys*/, Split<uint32> /*splits*/>;
	using SortType           = SortPredicate;
	using AllocatorType      = typename LeafList::AllocatorType;
	using ValueType          = typename LeafList::template ValueType<Values>;
	using KeyType            = typename LeafList::template ValueType<Keys>;
	using SplitType          = typename LeafList::template ValueType<Splits>;
	using SplitConstIterator = typename LeafList::template LocalConstIterator<SplitType>;
	using SplitIterator      = typename LeafList::template LocalConstIterator<SplitType>;
	using ConstIterator      = typename LeafList::template LocalConstIterator<ValueType>;
	using Iterator           = typename LeafList::template LocalConstIterator<ValueType>;
	using ConstSliceType     = Span<ConstIterator>;
	using SliceType          = Span<Iterator>;
	using SizeType           = typename LeafList::SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ArrayBvh instance.
	ArrayBvh(const AllocatorType& allocator, const SortType& sort = SortType()) ETNoexceptHint;
	//!	Constructs this @ref ArrayBvh instance.
	ArrayBvh(const ArrayBvh&) = default;
	//!	Constructs this @ref ArrayBvh instance.
	ArrayBvh(ArrayBvh&&) ETNoexceptHint = default;

	~ArrayBvh() = default;

	// ---------------------------------------------------

public:
	const SplitType& operator[](SizeType index) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	template <typename... ExtraArgs>
	void Sort(ExtraArgs&... args);

	// ---------------------------------------------------

public:
	void Insert(const ValueType& value);

	SizeType Erase(const ValueType& value);

	// ---------------------------------------------------

public:
	void Clear() ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	LeafList _leaves;
	SortType _sort;
	SizeType _splits;

	// ---------------------------------------------------

	template <typename Value, class Allocator>
	friend void Swap(ArrayBvh<Value, Allocator>&, ArrayBvh<Value, Allocator>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/ArrayBvh.inl>
//------------------------------------------------------------------//
