/*==================================================================*\
  SoaList.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/Containers/Span.hpp>
#include <Common/Mpl/Tuple.hpp>
//------------------------------------------------------------------//
#include <eastl/bonus/tuple_vector.h>
#include <eastl/iterator.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <class Allocator, typename... Types>
class SoaListAlloc {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::tuple_vector_alloc<EaStlAllocatorMixin<Allocator>, Types...>;

public:
	template <size_t Index>
	using ValueType            = TupleElement<Index, typename UnderlyingContainer::value_tuple>;
	using AllocatorType        = typename EaStlAllocatorMixin<Allocator>::PublicType;
	using ConstReverseIterator = typename UnderlyingContainer::const_reverse_iterator;
	using ReverseIterator      = typename UnderlyingContainer::reverse_iterator;
	using ConstIterator        = typename UnderlyingContainer::const_iterator;
	using Iterator             = typename UnderlyingContainer::iterator;
	using ConstPointer         = typename UnderlyingContainer::const_ptr_tuple;
	using Pointer              = typename UnderlyingContainer::ptr_tuple;
	using ConstReference       = typename UnderlyingContainer::const_reference_tuple;
	using Reference            = typename UnderlyingContainer::reference_tuple;
	using ValueTuple           = typename UnderlyingContainer::value_tuple;
	using SizeType             = typename UnderlyingContainer::size_type;
	using ConstSliceType       = Span<ConstIterator>;
	using SliceType            = Span<Iterator>;
	template <typename Type>
	using LocalConstIterator = const Type*;
	template <typename Type>
	using LocalIterator = Type*;
	template <typename Type>
	using LocalConstReverseIterator = eastl::reverse_iterator<LocalConstIterator<Type>>;
	template <typename Type>
	using LocalReverseIterator = eastl::reverse_iterator<LocalIterator<Type>>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref SoArrayListAlloc instance.
	SoaListAlloc(const AllocatorType& allocator = AllocatorType(), SizeType capacityHint = 0u);
	//!	Constructs this @ref SoArrayListAlloc instance.
	SoaListAlloc(const AllocatorType& allocator, const SoaListAlloc&);
	//!	Constructs this @ref SoArrayListAlloc instance.
	SoaListAlloc(const SoaListAlloc&) = default;
	//!	Constructs this @ref SoArrayListAlloc instance.
	SoaListAlloc(SoaListAlloc&&) ETNoexceptHint = default;

	~SoaListAlloc() = default;

	// - ELEMENT ITERATION -------------------------------

public:
	template <size_t Index>
	LocalConstReverseIterator<ValueType<Index>> ConstReverseBegin() const ETNoexceptHint;
	template <typename Type>
	LocalConstReverseIterator<Type> ConstReverseBegin() const ETNoexceptHint;
	ConstReverseIterator            ConstReverseBegin() const ETNoexceptHint;

	template <size_t Index>
	LocalConstReverseIterator<ValueType<Index>> ConstReverseEnd() const ETNoexceptHint;
	template <typename Type>
	LocalConstReverseIterator<Type> ConstReverseEnd() const ETNoexceptHint;
	ConstReverseIterator            ConstReverseEnd() const ETNoexceptHint;

	template <size_t Index>
	LocalConstReverseIterator<ValueType<Index>> ReverseBegin() const ETNoexceptHint;
	template <typename Type>
	LocalConstReverseIterator<Type> ReverseBegin() const ETNoexceptHint;
	ConstReverseIterator            ReverseBegin() const ETNoexceptHint;
	template <size_t Index>
	LocalReverseIterator<ValueType<Index>> ReverseBegin() ETNoexceptHint;
	template <typename Type>
	LocalReverseIterator<Type> ReverseBegin() ETNoexceptHint;
	ReverseIterator            ReverseBegin() ETNoexceptHint;

	template <size_t Index>
	LocalConstReverseIterator<ValueType<Index>> ReverseEnd() const ETNoexceptHint;
	template <typename Type>
	LocalConstReverseIterator<Type> ReverseEnd() const ETNoexceptHint;
	ConstReverseIterator            ReverseEnd() const ETNoexceptHint;
	template <size_t Index>
	LocalReverseIterator<ValueType<Index>> ReverseEnd() ETNoexceptHint;
	template <typename Type>
	LocalReverseIterator<Type> ReverseEnd() ETNoexceptHint;
	ReverseIterator            ReverseEnd() ETNoexceptHint;

	template <size_t Index>
	LocalConstIterator<ValueType<Index>> ConstBegin() const ETNoexceptHint;
	template <typename Type>
	LocalConstIterator<Type> ConstBegin() const ETNoexceptHint;
	ConstIterator            ConstBegin() const ETNoexceptHint;

	template <size_t Index>
	LocalConstIterator<ValueType<Index>> ConstEnd() const ETNoexceptHint;
	template <typename Type>
	LocalConstIterator<Type> ConstEnd() const ETNoexceptHint;
	ConstIterator            ConstEnd() const ETNoexceptHint;

	template <size_t Index>
	LocalConstIterator<ValueType<Index>> Begin() const ETNoexceptHint;
	template <typename Type>
	LocalConstIterator<Type> Begin() const ETNoexceptHint;
	ConstIterator            Begin() const ETNoexceptHint;
	template <size_t Index>
	LocalIterator<ValueType<Index>> Begin() ETNoexceptHint;
	template <typename Type>
	LocalIterator<Type> Begin() ETNoexceptHint;
	Iterator            Begin() ETNoexceptHint;

	template <size_t Index>
	LocalConstIterator<ValueType<Index>> End() const ETNoexceptHint;
	template <typename Type>
	LocalConstIterator<Type> End() const ETNoexceptHint;
	ConstIterator            End() const ETNoexceptHint;
	template <size_t Index>
	LocalIterator<ValueType<Index>> End() ETNoexceptHint;
	template <typename Type>
	LocalIterator<Type> End() ETNoexceptHint;
	Iterator            End() ETNoexceptHint;

	ConstSliceType Slice(SizeType offset, SizeType length) const ETNoexceptHint;
	SliceType      Slice(SizeType offset, SizeType length) ETNoexceptHint;

	operator ConstSliceType() const ETNoexceptHint;
	operator SliceType() ETNoexceptHint;

	// ---------------------------------------------------

public:
	ConstReference operator[](SizeType offset) const ETNoexceptHint;
	Reference      operator[](SizeType offset) ETNoexceptHint;

	ConstReference Front() const ETNoexceptHint;
	Reference      Front() ETNoexceptHint;

	ConstReference Back() const ETNoexceptHint;
	Reference      Back() ETNoexceptHint;

	template <size_t Index>
	const ValueType<Index>* GetData() const ETNoexceptHint;
	template <typename Type>
	const Type*  GetData() const ETNoexceptHint;
	ConstPointer GetData() const ETNoexceptHint;
	template <size_t Index>
	ValueType<Index>* GetData() ETNoexceptHint;
	template <typename Type>
	Type*   GetData() ETNoexceptHint;
	Pointer GetData() ETNoexceptHint;

	// ---------------------------------------------------

public:
	Reference EmplaceBack(Types&&...);

	void Append(const Types&...);

	// ---------------------------------------------------

public:
	Iterator Emplace(ConstIterator where, Types&&... arguments);

	Iterator Insert(ConstIterator where, const Types&... arguments);

	Iterator Erase(ConstIterator begin, ConstIterator end);
	Iterator Erase(ConstIterator where);

	Iterator EraseUnordered(ConstIterator where);

	// ---------------------------------------------------

public:
	SizeType GetCapacity() const ETNoexceptHint;

	SizeType GetSize() const ETNoexceptHint;

	void Reserve(SizeType capacityHint);

	template <typename NaryPredicate>
	void ClearAndDispose(NaryPredicate disposer) ETNoexceptHintIf(noexcept(disposer(Declval<Types&>()...)));

	void Clear() ETNoexceptHint;

	bool IsEmpty() const ETNoexceptHint;

	explicit operator bool() const ETNoexceptHint;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const ETNoexceptHint;

	// - CONTAINER DUPLICATION ---------------------------

public:
	SoaListAlloc& operator=(const SoaListAlloc&) = default;
	SoaListAlloc& operator=(SoaListAlloc&&) ETNoexceptHint = default;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Allocator, typename... Types>
	friend void Swap(SoaListAlloc<Allocator, Types...>&, SoaListAlloc<Allocator, Types...>&) ETNoexceptHint;
};

// ---

template <typename... Types>
using SoaList = typename SoaListAlloc<MallocAllocator, Types...>;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/SoaList.inl>
//------------------------------------------------------------------//
