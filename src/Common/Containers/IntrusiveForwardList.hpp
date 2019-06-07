/*==================================================================*\
  IntrusiveForwardList.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/IntrusiveForwardListHook.hpp>
#include <Common/Containers/RangeAdapters.hpp>
//------------------------------------------------------------------//
#include <eastl/bonus/intrusive_slist.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <class Value>
class IntrusiveForwardList {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::intrusive_slist<Value>;

public:
	using ValueType      = typename UnderlyingContainer::value_type;
	using ConstPointer   = typename UnderlyingContainer::const_pointer;
	using Pointer        = typename UnderlyingContainer::pointer;
	using ConstReference = typename UnderlyingContainer::const_reference;
	using Reference      = typename UnderlyingContainer::reference;
	using ConstIterator  = typename UnderlyingContainer::const_iterator;
	using Iterator       = typename UnderlyingContainer::iterator;
	using SizeType       = typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref IntrusiveForwardList instance.
	template <typename InputIterator>
	IntrusiveForwardList(InputIterator begin, InputIterator end);
	//! Disable copy construction.
	IntrusiveForwardList(const IntrusiveForwardList&) = delete;
	//! Constructs this @ref IntrusiveForwardList instance.
	IntrusiveForwardList(IntrusiveForwardList&&) = default;
	//! Constructs this @ref IntrusiveForwardList instance.
	IntrusiveForwardList() = default;

	~IntrusiveForwardList() = default;

	// - ALGORITHMS --------------------------------------

public:
	template <typename UnaryPredicate>
	ConstIterator FindIf(UnaryPredicate predicate, ConstIterator where) const;
	template <typename UnaryPredicate>
	ConstIterator FindIf(UnaryPredicate predicate) const;
	template <typename UnaryPredicate>
	Iterator FindIf(UnaryPredicate predicate, Iterator where);
	template <typename UnaryPredicate>
	Iterator FindIf(UnaryPredicate predicate);

	template <typename UnaryPredicate, typename UnaryPredicate2>
	void EraseAndDisposeIf(UnaryPredicate condition, UnaryPredicate2 disposer);

	template <typename UnaryPredicate>
	void EraseIf(UnaryPredicate condition);

	template <typename UnaryPredicate>
	void Sort(UnaryPredicate sort);

	// - ELEMENT ITERATION -------------------------------

public:
	ConstIterator ConstBegin() const;
	ConstIterator ConstEnd() const;

	ConstIterator Begin() const;
	Iterator      Begin();

	ConstIterator End() const;
	Iterator      End();

	ConstIterator GetIteratorTo(ConstReference element) const;
	Iterator      GetIteratorTo(Reference element);

	// - END POINT MANIPULATION --------------------------

public:
	//! Retrieves a reference to the first element stored in this @ref IntrusiveForwardList.
	ConstReference Front() const;
	//! Retrieves a reference to the first element stored in this @ref IntrusiveForwardList.
	Reference Front();

	//! Adds the passed-in item to the head of this @ref IntrusiveForwardList.
	void Prepend(Reference value);

	//! Removes the head element of this @ref IntrusiveForwardList, reducing its size by one element.
	template <typename UnaryPredicate>
	void PopFrontAndDispose(UnaryPredicate disposer);

	void PopFront();

	// - CONTAINER DUPLICATION ---------------------------

public:
	//!	Disable copy assignment.
	IntrusiveForwardList& operator=(const IntrusiveForwardList&) = delete;
	IntrusiveForwardList& operator=(IntrusiveForwardList&&) = default;

	// - CONTAINER MANIPULATION --------------------------

public:
	//! Inserts an element, shifting all antecedent elements down one position.
	Iterator Insert(Iterator where, Reference item);

	Iterator InsertAfter(Iterator where, Reference item);

	Iterator Erase(Iterator first, Iterator last);
	Iterator Erase(Iterator where);

	Iterator EraseAfter(Iterator beforeBegin, Iterator end);
	Iterator EraseAfter(Iterator where);

	template <typename UnaryPredicate>
	void EraseAndDispose(Iterator first, Iterator last, UnaryPredicate disposer);
	template <typename UnaryPredicate>
	Iterator EraseAndDispose(Iterator where, UnaryPredicate disposer);

	template <typename UnaryPredicate>
	void ClearAndDispose(UnaryPredicate disposer);

	void Clear();

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const;

	bool IsEmpty() const;

	explicit operator bool() const;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <class Value>
	friend void Swap(IntrusiveForwardList<Value>&, IntrusiveForwardList<Value>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/IntrusiveForwardList.inl>
//------------------------------------------------------------------//
