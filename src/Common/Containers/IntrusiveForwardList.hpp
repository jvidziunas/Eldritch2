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
		using UnderlyingContainer	= eastl::intrusive_slist<Value>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using Reference				= typename UnderlyingContainer::reference;
		using ConstReference		= typename UnderlyingContainer::const_reference;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using SizeType				= typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref IntrusiveForwardList instance.
		template <typename InputIterator>
		IntrusiveForwardList( InputIterator begin, InputIterator end );
	//! Disable copy construction.
		IntrusiveForwardList( const IntrusiveForwardList& ) = delete;
	//! Constructs this @ref IntrusiveForwardList instance.
		IntrusiveForwardList( IntrusiveForwardList&& );
	//! Constructs this @ref IntrusiveForwardList instance.
		IntrusiveForwardList() = default;

		~IntrusiveForwardList() = default;

	// - ALGORITHMS --------------------------------------

	public:
		template <typename Predicate>
		ConstIterator	FindIf( Predicate predicate, ConstIterator searchHint ) const;
		template <typename Predicate>
		ConstIterator	FindIf( Predicate predicate ) const;
		template <typename Predicate>
		Iterator		FindIf( Predicate predicate, Iterator searchHint );
		template <typename Predicate>
		Iterator		FindIf( Predicate predicate );

		template <typename Predicate, typename Disposer>
		void			EraseAndDisposeIf( Predicate predicate, Disposer disposer );

		template <typename Predicate>
		void			EraseIf( Predicate predicate );

		template <typename Predicate>
		void			Sort( Predicate predicate );

	// - ELEMENT ITERATION -------------------------------
		
	public:
		ConstIterator	ConstBegin() const;
		ConstIterator	ConstEnd() const;

		ConstIterator	Begin() const;
		Iterator		Begin();

		ConstIterator	End() const;
		Iterator		End();
		
		ConstIterator	GetIteratorTo( ConstReference element ) const;
		Iterator		GetIteratorTo( Reference element );

	// - END POINT MANIPULATION --------------------------

	public:
	//! Retrieves a reference to the first element stored in this @ref IntrusiveForwardList.
		ConstReference	Front() const;
	//! Retrieves a reference to the first element stored in this @ref IntrusiveForwardList.
		Reference		Front();

	//! Adds the passed-in item to the head of this @ref IntrusiveForwardList.
		void			Prepend( Reference item );

	//! Removes the head element of this @ref IntrusiveForwardList, reducing its size by one element.
		template <typename Disposer>
		void			PopFrontAndDispose( Disposer disposer );

		void			PopFront();

	// - CONTAINER DUPLICATION ---------------------------

	public:
	//!	Disable copy assignment.
		IntrusiveForwardList&	operator=( const IntrusiveForwardList& ) = delete;
		IntrusiveForwardList&	operator=( IntrusiveForwardList&& );

	// - CONTAINER MANIPULATION --------------------------

	public:
	//! Inserts an element, shifting all antecedent elements down one position.
		Iterator	Insert( Iterator location, Reference item );

		Iterator	InsertAfter( Iterator location, Reference item );

		Iterator	Erase( Iterator first, Iterator last );
		Iterator	Erase( Iterator position );

		Iterator	EraseAfter( Iterator beforeBegin, Iterator end );
		Iterator	EraseAfter( Iterator position );

		template <typename Disposer>
		void		EraseAndDispose( Iterator first, Iterator last, Disposer disposer );
		template <typename Disposer>
		Iterator	EraseAndDispose( Iterator position, Disposer disposer );

		template <typename Disposer>
		void		ClearAndDispose( Disposer disposer );

		void		Clear();

	// - CONTENT QUERY -----------------------------------

	public:
		SizeType			GetSize() const;

		bool				IsEmpty() const;

		explicit operator	bool() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_container;

	// ---------------------------------------------------

		template <class Value>
		friend void	Swap( IntrusiveForwardList<Value>&, IntrusiveForwardList<Value>& );
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/IntrusiveForwardList.inl>
//------------------------------------------------------------------//