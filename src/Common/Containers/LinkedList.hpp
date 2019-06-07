/*==================================================================*\
  LinkedList.hpp
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
//------------------------------------------------------------------//
#include <eastl/list.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, typename Allocator = MallocAllocator>
class LinkedList {
	// - TYPE PUBLISHING ---------------------------------

protected:
	using UnderlyingContainer = eastl::list<Value, EaStlAllocatorMixin<Allocator>>;

public:
	using AllocatorType        = typename UnderlyingContainer::allocator_type::PublicType;
	using ValueType            = typename UnderlyingContainer::value_type;
	using ConstPointer         = typename UnderlyingContainer::const_pointer;
	using Pointer              = typename UnderlyingContainer::pointer;
	using ConstReference       = typename UnderlyingContainer::const_reference;
	using Reference            = typename UnderlyingContainer::reference;
	using ConstIterator        = typename UnderlyingContainer::const_iterator;
	using Iterator             = typename UnderlyingContainer::iterator;
	using ConstReverseIterator = typename UnderlyingContainer::const_reverse_iterator;
	using ReverseIterator      = typename UnderlyingContainer::reverse_iterator;
	using SizeType             = typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref LinkedList instance.
	template <typename InputIterator>
	LinkedList(const AllocatorType& allocator, InputIterator first, InputIterator last);
	//! Constructs this @ref LinkedList instance.
	LinkedList(const AllocatorType& allocator, InitializerList<ValueType>);
	//! Constructs this @ref LinkedList instance.
	LinkedList(const AllocatorType& allocator, const LinkedList&);
	//! Constructs this @ref LinkedList instance.
	LinkedList(const AllocatorType& allocator = AllocatorType());
	//! Constructs this @ref LinkedList instance.
	LinkedList(const LinkedList&) = default;
	//! Constructs this @ref LinkedList instance.
	LinkedList(LinkedList&&) = default;

	~LinkedList() = default;

	// - ALGORITHMS --------------------------------------

public:
	ConstIterator Find(ConstReference value, ConstIterator where) const;
	Iterator      Find(ConstReference value, Iterator where);

	template <typename UnaryPredicate>
	Iterator EraseIf(UnaryPredicate condition);

	// - ELEMENT ITERATION -------------------------------

public:
	//! Retrieves a @ref ConstIterator pointing to the first element stored in this @ref LinkedList.
	ConstIterator ConstBegin() const ETNoexceptHint;

	//!	Retrieves a @ref ConstIterator pointing one position after the last element stored in this @ref LinkedList.
	ConstIterator ConstEnd() const ETNoexceptHint;

	//!	Retrieves a @ref ConstIterator pointing to the first element stored in this @ref LinkedList.
	ConstIterator Begin() const ETNoexceptHint;
	//!	Retrieves an @ref Iterator pointing to the first element stored in this @ref LinkedList.
	Iterator Begin() ETNoexceptHint;

	//!	Retrieves a @ref ConstIterator pointing one position after the last element stored in this @ref LinkedList.
	ConstIterator End() const ETNoexceptHint;
	//!	Retrieves an @ref Iterator pointing one position after the last element stored in this @ref LinkedList.
	Iterator End() ETNoexceptHint;

	// - END POINT MANIPULATION --------------------------

public:
	//!	Retrieves a reference to the first element stored in this @ref LinkedList.
	ConstReference Front() const;
	//!	Retrieves a reference to the first element stored in this @ref LinkedList.
	Reference Front();

	//!	Retrieves a reference to the last element stored in this @ref LinkedList.
	ConstReference Back() const;
	//!	Retrieves a reference to the last element stored in this @ref LinkedList.
	Reference Back();

	//!	Adds a duplicate of the passed-in item to the head of this @ref LinkedList.
	void Prepend(ConstReference value);

	//!	Adds a duplicate of the passed-in item to the tail of this @ref LinkedList.
	void Append(ConstReference value);

	template <typename... Arguments>
	void EmplaceFront(Arguments&&... arguments);

	template <typename... Arguments>
	void EmplaceBack(Arguments&&... arguments);

	//!	Removes the head element of this @ref LinkedList, reducing its size by one element.
	void PopFront();

	//!	Removes the tail element of this @ref LinkedList, reducing its size by one element.
	void Pop();

	// - CONTAINER MANIPULATION --------------------------

public:
	//!	Inserts an element at the position specified, shifting all antecedent elements down one position.
	Iterator Insert(Iterator where, ConstReference value);

	template <typename... Arguments>
	Iterator Emplace(Iterator where, Arguments&&... arguments);

	//!	Removes a range of elements in the range [first, last), reducing the size of the @ref LinkedList by <<distance( first, last )>>
	Iterator Erase(Iterator first, Iterator last);
	//!	Removes an element at the position specified, reducing the size of the @ref LinkedList by one element.
	Iterator Erase(Iterator where);

	template <typename UnaryPredicate>
	void ClearAndDispose(UnaryPredicate disposer);

	//!	Removes all elements from the @ref LinkedList.
	void Clear();

	// - CONTAINER DUPLICATION ---------------------------

public:
	LinkedList& operator=(const LinkedList&) = default;
	LinkedList& operator=(LinkedList&&) = default;

	// - CONTENT QUERY -----------------------------------

public:
	//!	Counts the number of elements contained within this @ref LinkedList.
	SizeType GetSize() const ETNoexceptHint;

	//!	Returns a bool indicating whether or not there are any elements contained in this @ref LinkedList.
	bool IsEmpty() const ETNoexceptHint;

	explicit operator bool() const ETNoexceptHint;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Value, typename Allocator>
	friend void Swap(LinkedList<Value, Allocator>&, LinkedList<Value, Allocator>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/LinkedList.inl>
//------------------------------------------------------------------//
