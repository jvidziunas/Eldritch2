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
//------------------------------------------------------------------//
#include <eastl/intrusive_list.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value>
	class IntrusiveLinkedList {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using UnderlyingContainer	= eastl::intrusive_list<Value>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using Reference				= typename UnderlyingContainer::reference;
		using ConstReference		= typename UnderlyingContainer::const_reference;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ReverseIterator		= typename UnderlyingContainer::reverse_iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using ConstReverseIterator	= typename UnderlyingContainer::const_reverse_iterator;
		using SizeType				= typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		IntrusiveLinkedList( const IntrusiveLinkedList& ) = delete;
	//!	Constructs this @ref IntrusiveList instance.
		IntrusiveLinkedList( IntrusiveLinkedList&& );
	//!	Constructs this @ref IntrusiveList instance.
		IntrusiveLinkedList() = default;

		~IntrusiveLinkedList() = default;

	// - ALGORITHMS --------------------------------------

	public:
		template <typename Predicate>
		ConstIterator	Find( ConstIterator searchHint, Predicate predicate ) const;
		template <typename Predicate>
		Iterator		Find( Iterator searchHint, Predicate predicate );
		template <typename Predicate>
		ConstIterator	Find( Predicate predicate ) const;
		template <typename Predicate>
		Iterator		Find( Predicate predicate );

		template <typename Predicate, typename Disposer>
		void			EraseAndDisposeIf( Predicate filter, Disposer disposer );

		template <typename Predicate>
		void			EraseIf( Predicate predicate );

	// - ELEMENT ITERATION -------------------------------

	public:
		ConstIterator	ConstBegin() const;

		ConstIterator	ConstEnd() const;

		ConstIterator	Begin() const;
		Iterator		Begin();
		
		ConstIterator	End() const;
		Iterator		End();

		ConstIterator	IteratorTo( ConstReference element ) const;
		Iterator		IteratorTo( Reference element );

	// - END POINT MANIPULATION --------------------------

	public:
	//	Retrieves a reference to the first element stored in this @ref IntrusiveList.
		ConstReference	Front() const;
	//	Retrieves a reference to the first element stored in this @ref IntrusiveList.
		Reference		Front();

	//	Retrieves a reference to the last element stored in this @ref IntrusiveList.
		ConstReference	Back() const;
	//	Retrieves a reference to the last element stored in this @ref IntrusiveList.
		Reference		Back();
	
	//	Adds the passed-in item to the head of this @ref IntrusiveList.
		void			Prepend( Reference item );

	//	Adds the passed-in item to the tail of this @ref IntrusiveList.
		void			Append( Reference item );

	//	Removes the head element of this @ref IntrusiveList, reducing its size by one element.
		void			PopFront();

	//	Removes the tail element of this @ref IntrusiveList, reducing its size by one element.
		void			Pop();

	//	Removes the head element of this @ref IntrusiveList, reducing its size by one element.
		template <typename Disposer>
		void			PopFrontAndDispose( Disposer disposer );

	//	Removes the tail element of this @ref IntrusiveList, reducing its size by one element. Operator() is invoked on the specified disposer, with the popped element passed in as the sole parameter.
		template <typename Disposer>
		void			PopAndDispose( Disposer disposer );

	// - CONTAINER DUPLICATION ---------------------------

	public:
		template <typename Disposer, typename ElementCloner>
		void	CloneFrom( const IntrusiveLinkedList& containerTemplate, Disposer disposer, ElementCloner cloner );

	// - CONTAINER MANIPULATION --------------------------

	public:
	//	Inserts an element at the position specified, shifting all antecedent elements down one position.
		Iterator	Insert( Iterator location, Reference item );

		Iterator	Erase( Iterator first, Iterator last );
		Iterator	Erase( Iterator position );

		template <typename Disposer>
		Iterator	EraseAndDispose( Iterator first, Iterator last, Disposer disposer );
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

		template <typename Value>
		friend void	Swap( IntrusiveLinkedList<Value>&, IntrusiveLinkedList<Value>& );
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/IntrusiveLinkedList.inl>
//------------------------------------------------------------------//