/*==================================================================*\
  IntrusiveLinkedList.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/IntrusiveListHook.hpp>
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Mpl/TypeTraits.hpp>
//------------------------------------------------------------------//
#include <eastl/intrusive_list.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value>
class IntrusiveLinkedList {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::intrusive_list<Value>;

public:
	using ValueType            = typename UnderlyingContainer::value_type;
	using ConstPointer         = typename UnderlyingContainer::const_pointer;
	using Pointer              = typename UnderlyingContainer::pointer;
	using ConstReference       = typename UnderlyingContainer::const_reference;
	using Reference            = typename UnderlyingContainer::reference;
	using ReverseIterator      = typename UnderlyingContainer::reverse_iterator;
	using Iterator             = typename UnderlyingContainer::iterator;
	using ConstReverseIterator = typename UnderlyingContainer::const_reverse_iterator;
	using ConstIterator        = typename UnderlyingContainer::const_iterator;
	using SizeType             = typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Disable copy construction.
	IntrusiveLinkedList(const IntrusiveLinkedList&) = delete;
	//!	Constructs this @ref IntrusiveLinkedList instance.
	IntrusiveLinkedList(IntrusiveLinkedList&&) = default;
	//!	Constructs this @ref IntrusiveLinkedList instance.
	IntrusiveLinkedList() = default;

	~IntrusiveLinkedList() = default;

	// - ALGORITHMS --------------------------------------

public:
	template <typename UnaryPredicate>
	ConstIterator Find(ConstIterator searchHint, UnaryPredicate condition) const ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, ConstReference>());
	template <typename UnaryPredicate>
	Iterator Find(Iterator searchHint, UnaryPredicate condition) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, Reference>());
	template <typename UnaryPredicate>
	ConstIterator Find(UnaryPredicate condition) const ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, ConstReference>());
	template <typename UnaryPredicate>
	Iterator Find(UnaryPredicate condition) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, Reference>());

	template <typename UnaryPredicate, typename UnaryPredicate2>
	void EraseAndDisposeIf(UnaryPredicate condition, UnaryPredicate2 disposer);

	template <typename UnaryPredicate>
	void EraseIf(UnaryPredicate predicate);

	// - ELEMENT ITERATION -------------------------------

public:
	ConstIterator ConstBegin() const ETNoexceptHint;

	ConstIterator ConstEnd() const ETNoexceptHint;

	ConstIterator Begin() const ETNoexceptHint;
	Iterator      Begin() ETNoexceptHint;

	ConstIterator End() const ETNoexceptHint;
	Iterator      End() ETNoexceptHint;

	ConstIterator IteratorTo(ConstReference element) const;
	Iterator      IteratorTo(Reference element);

	// - END POINT MANIPULATION --------------------------

public:
	//!	Retrieves a reference to the first element stored in this @ref IntrusiveLinkedList.
	ConstReference Front() const;
	//!	Retrieves a reference to the first element stored in this @ref IntrusiveLinkedList.
	Reference Front();

	//!	Retrieves a reference to the last element stored in this @ref IntrusiveLinkedList.
	ConstReference Back() const;
	//!	Retrieves a reference to the last element stored in this @ref IntrusiveLinkedList.
	Reference Back();

	//!	Adds the passed-in item to the head of this @ref IntrusiveLinkedList.
	void Prepend(Reference value) ETNoexceptHint;

	//!	Adds the passed-in item to the tail of this @ref IntrusiveLinkedList.
	void Append(Reference value) ETNoexceptHint;

	//!	Removes the head element of this @ref IntrusiveLinkedList, reducing its size by one element.
	void PopFront() ETNoexceptHint;

	//!	Removes the tail element of this @ref IntrusiveLinkedList, reducing its size by one element.
	void Pop() ETNoexceptHint;

	//!	Removes the head element of this @ref IntrusiveLinkedList, reducing its size by one element.
	template <typename UnaryPredicate>
	void PopFrontAndDispose(UnaryPredicate disposer);

	//!	Removes the tail element of this @ref IntrusiveLinkedList, reducing its size by one element. Operator() is invoked on the specified disposer, with the popped element passed in as the sole parameter.
	template <typename UnaryPredicate>
	void PopAndDispose(UnaryPredicate disposer);

	// - CONTAINER DUPLICATION ---------------------------

public:
	template <typename UnaryPredicate, typename Cloner>
	void CloneFrom(const IntrusiveLinkedList& list, UnaryPredicate disposer, Cloner cloner);

	// - CONTAINER MANIPULATION --------------------------

public:
	//!	Inserts an element before the position specified.
	Iterator Insert(Iterator where, Reference item);

	Iterator Erase(Iterator first, Iterator last);
	Iterator Erase(Iterator where);
	static void Erase(Reference where);

	template <typename UnaryPredicate>
	Iterator EraseAndDispose(Iterator first, Iterator last, UnaryPredicate disposer);
	template <typename UnaryPredicate>
	Iterator EraseAndDispose(Iterator where, UnaryPredicate disposer);

	template <typename UnaryPredicate>
	void ClearAndDispose(UnaryPredicate disposer);

	void Clear() ETNoexceptHint;

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const ETNoexceptHint;

	bool IsEmpty() const ETNoexceptHint;

	explicit operator bool() const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Value2>
	friend void Swap(IntrusiveLinkedList<Value2>&, IntrusiveLinkedList<Value2>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/IntrusiveLinkedList.inl>
//------------------------------------------------------------------//
