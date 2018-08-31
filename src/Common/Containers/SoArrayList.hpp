/*==================================================================*\
  SoArrayList.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Memory/MallocAllocator.hpp>
//------------------------------------------------------------------//
#include <eastl/bonus/tuple_vector.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <class Allocator, typename... Types>
class SoArrayListAlloc {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::tuple_vector_alloc<EaStlAllocatorMixin<Allocator>, Types...>;

public:
	using AllocatorType        = typename EaStlAllocatorMixin<Allocator>::PublicType;
	using ConstIterator        = typename UnderlyingContainer::const_iterator;
	using Iterator             = typename UnderlyingContainer::iterator;
	using ConstReverseIterator = typename UnderlyingContainer::const_reverse_iterator;
	using ReverseIterator      = typename UnderlyingContainer::reverse_iterator;
	using SizeType             = typename UnderlyingContainer::size_type;
	using ConstPointer         = typename UnderlyingContainer::const_ptr_tuple;
	using Pointer              = typename UnderlyingContainer::ptr_tuple;
	using ConstReference       = typename UnderlyingContainer::const_reference_tuple;
	using Reference            = typename UnderlyingContainer::reference_tuple;
	using ValueType            = typename UnderlyingContainer::value_tuple;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref SoArrayListAlloc instance.
	SoArrayListAlloc(const AllocatorType& allocator = AllocatorType(), SizeType capacityHint = 0u);
	//!	Constructs this @ref SoArrayListAlloc instance.
	SoArrayListAlloc(const AllocatorType& allocator, const SoArrayListAlloc&);
	//!	Constructs this @ref SoArrayListAlloc instance.
	SoArrayListAlloc(const SoArrayListAlloc&) = default;
	//!	Constructs this @ref SoArrayListAlloc instance.
	SoArrayListAlloc(SoArrayListAlloc&&) = default;

	~SoArrayListAlloc() = default;

	// ---------------------------------------------------

public:
	ConstReverseIterator ConstReverseBegin() const ETNoexceptHint;
	ConstReverseIterator ConstReverseEnd() const ETNoexceptHint;

	ConstReverseIterator ReverseBegin() const ETNoexceptHint;
	ReverseIterator      ReverseBegin() ETNoexceptHint;

	ConstReverseIterator ReverseEnd() const ETNoexceptHint;
	ReverseIterator      ReverseEnd() ETNoexceptHint;

	ConstIterator ConstBegin() const ETNoexceptHint;

	ConstIterator ConstEnd() const ETNoexceptHint;

	ConstIterator Begin() const ETNoexceptHint;
	Iterator      Begin() ETNoexceptHint;

	ConstIterator End() const ETNoexceptHint;
	Iterator      End() ETNoexceptHint;

	// ---------------------------------------------------

public:
	ConstReference operator[](SizeType offset) const ETNoexceptHint;
	Reference      operator[](SizeType offset) ETNoexceptHint;

	ConstReference Front() const ETNoexceptHint;
	Reference      Front() ETNoexceptHint;

	ConstReference Back() const ETNoexceptHint;
	Reference      Back() ETNoexceptHint;

	ConstPointer GetData() const ETNoexceptHint;
	Pointer      GetData() ETNoexceptHint;

	template <typename Type>
	const Type* Get() const ETNoexceptHint;
	template <typename Type>
	Type* Get() ETNoexceptHint;

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

	void Clear() ETNoexceptHint;

	bool IsEmpty() const ETNoexceptHint;

	explicit operator bool() const ETNoexceptHint;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const ETNoexceptHint;

	// - CONTAINER DUPLICATION ---------------------------

public:
	SoArrayListAlloc& operator=(const SoArrayListAlloc&) = default;
	SoArrayListAlloc& operator=(SoArrayListAlloc&&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Allocator, typename... Types>
	friend void Swap(SoArrayListAlloc<Allocator, Types...>&, SoArrayListAlloc<Allocator, Types...>&);
};

// ---

template <typename... Types>
using SoArrayList = typename SoArrayListAlloc<MallocAllocator, Types...>;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/SoArrayList.inl>
//------------------------------------------------------------------//
