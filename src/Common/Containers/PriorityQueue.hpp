/*==================================================================*\
  PriorityQueue.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/ArrayList.hpp>
#include <Common/LessThan.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, class Container = ArrayList<Value>, typename Comparator = LessThan<Value>>
class PriorityQueue {
	// - TYPE PUBLISHING ---------------------------------

public:
	using ContainerType  = Container;
	using ComparatorType = Comparator;
	using ConstReference = typename ContainerType::ConstReference;
	using Reference      = typename ContainerType::Reference;
	using ValueType      = typename Container::ValueType;
	using SizeType       = typename ContainerType::SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref PriorityQueue instance.
	template <typename InputIterator>
	PriorityQueue(InputIterator first, InputIterator last, const ComparatorType& sorter, const ContainerType& container);
	//!	Constructs this @ref PriorityQueue instance.
	template <class InputIterator>
	PriorityQueue(InputIterator first, InputIterator last, const ComparatorType& sorter, ContainerType&& container);
	//!	Constructs this @ref PriorityQueue instance.
	template <typename InputIterator>
	PriorityQueue(InputIterator first, InputIterator last, const ComparatorType& sorter);
	//!	Constructs this @ref PriorityQueue instance.
	template <typename InputIterator>
	PriorityQueue(InputIterator first, InputIterator last);
	//!	Constructs this @ref PriorityQueue instance.
	explicit PriorityQueue(const ComparatorType& sorter, ContainerType&& container);
	//!	Constructs this @ref PriorityQueue instance.
	PriorityQueue(const ComparatorType& sorter, const ContainerType& container);
	//!	Constructs this @ref PriorityQueue instance.
	template <class Allocator>
	PriorityQueue(const PriorityQueue& x, const Allocator& allocator);
	//!	Constructs this @ref PriorityQueue instance.
	template <class Allocator>
	PriorityQueue(PriorityQueue&&, const Allocator& allocator);
	//!	Constructs this @ref PriorityQueue instance.
	explicit PriorityQueue(const ComparatorType& sorter);
	//!	Constructs this @ref PriorityQueue instance.
	template <class Allocator>
	explicit PriorityQueue(const Allocator& allocator);
	//!	Constructs this @ref PriorityQueue instance.
	PriorityQueue();

	~PriorityQueue() = default;

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const;

	bool IsEmpty() const;

	explicit operator bool() const;

	// ---------------------------------------------------

public:
	ConstReference Top() const;
	Reference      Top();

	// ---------------------------------------------------

public:
	void Push(const ValueType& value);
	void Push(ValueType&& value);

	template <class... Args>
	void Emplace(Args&&... args);

	void Pop(ValueType& value);
	void Pop();

	// ---------------------------------------------------

public:
	void Change(SizeType index);

	void Remove(SizeType index);

	// ---------------------------------------------------

public:
	const ContainerType& GetContainer() const;
	ContainerType&       GetContainer();

	// - DATA MEMBERS ------------------------------------

protected:
	ContainerType  c;
	ComparatorType comp;

	// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	friend void Swap(PriorityQueue<Value, Container, Comparator>&, PriorityQueue<Value, Container, Comparator>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/PriorityQueue.inl>
//------------------------------------------------------------------//
