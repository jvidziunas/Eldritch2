/*==================================================================*\
  Queue.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Dequeue.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, typename Allocator = MallocAllocator, typename Container = Dequeue<Value, Allocator>>
class Queue {
	// - TYPE PUBLISHING ---------------------------------

public:
	using ContainerType = Container;
	using AllocatorType = typename ContainerType::AllocatorType;
	using SizeType      = typename ContainerType::SizeType;
	using ValueType     = typename ContainerType::ValueType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Queue instance.
	Queue(const Queue&, const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref Queue instance.
	Queue(const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref Queue instance.
	Queue(Queue&&);

	~Queue() = default;

	// - END POINT MANIPULATION --------------------------

public:
	const ValueType& Front() const;
	ValueType&       Front();

	const ValueType& Back() const;
	ValueType&       Back();

	template <typename... Args>
	void EmplaceBack(Args&&... args);

	void Push(const ValueType& value);
	void Push(ValueType&& value);

	void Pop();

	// - CONTAINER MANIPULATION --------------------------

public:
	const ContainerType& GetContainer() const;
	ContainerType&       GetContainer();

	const AllocatorType& GetAllocator() const;

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const;

	bool IsEmpty() const;

	// - DATA MEMBERS ------------------------------------

public:
	ContainerType c;

	// ---------------------------------------------------

	template <typename Value, typename Allocator, typename Container>
	friend void Swap(Queue<Value, Allocator, Container>&, Queue<Value, Allocator, Container>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/Queue.inl>
//------------------------------------------------------------------//
