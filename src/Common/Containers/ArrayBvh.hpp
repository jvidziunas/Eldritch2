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
#include <Common/Mpl/Queryable.hpp>
#include <Common/Mpl/Vector.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, typename SortPredicate, class Allocator = MallocAllocator>
class ArrayBvh : public Queryable<ArrayBvh<Value, SortPredicate, Allocator>, Value> {
	// - TYPE PUBLISHING ---------------------------------

public:
	enum : size_t {
		Sectors,
		Keys,
		Values,
	};

	// ---

public:
	template <typename Index = typename SoaListAlloc<Allocator, typename SortPredicate::KeyType, Value>::SizeType>
	struct Sector {
		Index minima[3];
		Index maxima[3];
		Index leftChild;
		Index rightChild;
		Box   bounds;
	};

	// ---

public:
	using LeafList           = SoaListAlloc<Allocator, Sector<> /*sectors*/, typename SortPredicate::KeyType /*keys*/, Value /*values*/>;
	using AllocatorType      = typename LeafList::AllocatorType;
	using ValueType          = typename LeafList::template ValueType<Values>;
	using KeyType            = typename LeafList::template ValueType<Keys>;
	using SectorType         = typename LeafList::template ValueType<Sectors>;
	using SplitConstIterator = typename LeafList::template LocalConstIterator<SectorType>;
	using SplitIterator      = typename LeafList::template LocalConstIterator<SectorType>;
	using ConstIterator      = typename LeafList::template LocalConstIterator<ValueType>;
	using Iterator           = typename LeafList::template LocalConstIterator<ValueType>;
	using SizeType           = typename LeafList::SizeType;
	using SortType           = SortPredicate;
	using ConstSliceType     = Span<ConstIterator>;
	using SliceType          = Span<Iterator>;

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
	template <typename... ExtraArgs>
	void Rebuild(ExtraArgs&... args);

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
	SizeType _sectors;

	// ---------------------------------------------------

	template <typename Value2, class Allocator2>
	friend void Swap(ArrayBvh<Value2, Allocator2>&, ArrayBvh<Value2, Allocator2>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/ArrayBvh.inl>
//------------------------------------------------------------------//
