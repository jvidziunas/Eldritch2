/*==================================================================*\
  Dequeue.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Memory/MallocAllocator.hpp>
//------------------------------------------------------------------//
#include <eastl/deque.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, class Allocator = MallocAllocator, unsigned int granularity = (256u / sizeof(Value))>
class Dequeue {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::deque<Value, EaStlAllocatorMixin<Allocator>, granularity>;

public:
	using AllocatorType        = typename UnderlyingContainer::allocator_type::PublicType;
	using ConstReverseIterator = typename UnderlyingContainer::const_reverse_iterator;
	using ReverseIterator      = typename UnderlyingContainer::reverse_iterator;
	using ConstIterator        = typename UnderlyingContainer::const_iterator;
	using Iterator             = typename UnderlyingContainer::iterator;
	using ValueType            = typename UnderlyingContainer::value_type;
	using ConstPointer         = typename UnderlyingContainer::const_pointer;
	using Pointer              = typename UnderlyingContainer::pointer;
	using ConstReference       = typename UnderlyingContainer::const_reference;
	using Reference            = typename UnderlyingContainer::reference;
	using SizeType             = typename UnderlyingContainer::size_type;

	enum : unsigned int {
		Granularity = granularity
	};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Dequeue instance.
	template <typename InputIterator>
	Dequeue(const AllocatorType& allocator, InputIterator begin, InputIterator end);
	//!	Constructs this @ref Dequeue instance.
	Dequeue(const AllocatorType& allocator, InitializerList<ValueType>);
	//!	Constructs this @ref Dequeue instance.
	Dequeue(const AllocatorType& allocator, const Dequeue&);
	//!	Constructs this @ref Dequeue instance.
	Dequeue(const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref Dequeue instance.
	Dequeue(const Dequeue&) = default;
	//!	Constructs this @ref Dequeue instance.
	Dequeue(Dequeue&&) = default;

	~Dequeue() = default;

	// - END POINT MANIPULATION --------------------------

public:
	ConstReference Front() const;
	Reference      Front();

	ConstReference Back() const;
	Reference      Back();

	void      Prepend(const ValueType& value);
	void      Prepend(ValueType&& value);
	Reference Prepend();

	void      Append(const ValueType& value);
	void      Append(ValueType&& value);
	Reference Append();

	void PopFront();

	void Pop();

	// ---------------------------------------------------

public:
	template <typename InputIterator>
	void Assign(InputIterator first, InputIterator last);
	void Assign(InitializerList<ValueType> values);
	void Assign(SizeType n, const ValueType& value);

	// - ELEMENT ITERATION -------------------------------

public:
	ConstReverseIterator ConstReverseBegin() const;

	ConstReverseIterator ConstReverseEnd() const;

	ConstReverseIterator ReverseBegin() const;
	ReverseIterator      ReverseBegin();

	ConstReverseIterator ReverseEnd() const;
	ReverseIterator      ReverseEnd();

	ConstIterator ConstBegin() const;
	ConstIterator ConstEnd() const;

	ConstIterator Begin() const;
	Iterator      Begin();

	ConstIterator End() const;
	Iterator      End();

	// ---------------------------------------------------

public:
	void Resize(SizeType size, const ValueType& value);
	void Resize(SizeType size);

	void SetCapacity(SizeType capacity = UnderlyingContainer::npos);

	void ShrinkToFit();

	// - ELEMENT ACCESS ----------------------------------

public:
	ConstReference operator[](SizeType index) const;
	Reference      operator[](SizeType index);

	ConstReference At(SizeType index) const;
	Reference      At(SizeType index);

	// - CONTAINER MANIPULATION --------------------------

public:
	template <class... Args>
	Iterator Emplace(ConstIterator position, Args&&... args);

	template <class... Args>
	void EmplaceFront(Args&&... args);

	template <class... Args>
	void EmplaceBack(Args&&... args);

	template <typename InputIterator>
	void     Insert(ConstIterator position, InputIterator first, InputIterator last);
	Iterator Insert(ConstIterator position, InitializerList<ValueType>);
	void     Insert(ConstIterator position, SizeType n, const ValueType& value);
	Iterator Insert(ConstIterator position, const ValueType& value);
	Iterator Insert(ConstIterator position, ValueType&& value);

	ReverseIterator Erase(ReverseIterator first, ReverseIterator last);
	ReverseIterator Erase(ReverseIterator position);
	Iterator        Erase(ConstIterator first, ConstIterator last);
	Iterator        Erase(ConstIterator position);

	void Clear();

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const;

	bool IsEmpty() const;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	friend void Swap(Dequeue<Value, Allocator, granularity>&, Dequeue<Value, Allocator, granularity>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/Dequeue.inl>
//------------------------------------------------------------------//
