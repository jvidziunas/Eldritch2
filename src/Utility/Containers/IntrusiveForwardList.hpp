/*==================================================================*\
  IntrusiveForwardList.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class StoredObject>
	class IntrusiveForwardList {
	// - TYPE PUBLISHING ---------------------------------
	private:
		using UnderlyingContainer	= ::rde::intrusive_slist<StoredObject>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using Reference				= ValueType&;
		using ConstReference		= const ValueType&;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using SizeType				= size_t;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref IntrusiveForwardList instance.
		ETInlineHint IntrusiveForwardList( ::Eldritch2::IntrusiveForwardList<StoredObject>&& sourceContainer );
		//! Constructs this @ref IntrusiveForwardList instance.
		ETInlineHint IntrusiveForwardList( const ::Eldritch2::IntrusiveForwardList<StoredObject>& ) = delete;
		//! Constructs this @ref IntrusiveForwardList instance.
		template <typename SourceIterator>
		ETInlineHint IntrusiveForwardList( SourceIterator elementBegin, SourceIterator elementEnd );
		//! Constructs this @ref IntrusiveForwardList instance.
		ETInlineHint IntrusiveForwardList() = default;

	// - ALGORITHMS --------------------------------------

		template <typename Predicate>
		ETInlineHint ConstIterator	Find( Predicate predicate, ConstIterator searchHint ) const;
		template <typename Predicate>
		ETInlineHint ConstIterator	Find( Predicate predicate ) const;
		template <typename Predicate>
		ETInlineHint Iterator		Find( Predicate predicate, Iterator searchHint );
		template <typename Predicate>
		ETInlineHint Iterator		Find( Predicate predicate );

		template <typename Predicate>
		ETInlineHint void	RemoveIf( Predicate predicate );

		template <typename Predicate, typename Disposer>
		ETInlineHint void	RemoveAndDisposeIf( Predicate predicate, Disposer disposer );

		template <typename Predicate>
		ETInlineHint void	Sort( Predicate predicate );

	// - ELEMENT ITERATION -------------------------------

		ConstIterator	Begin() const;
		Iterator		Begin();

		ConstIterator	ConstBegin() const;

		ConstIterator	End() const;
		Iterator		End();

		ConstIterator	ConstEnd() const;
		
		ConstIterator	IteratorTo( ConstReference element ) const;
		Iterator		IteratorTo( Reference element );

	// - END POINT MANIPULATION --------------------------

		//! Retrieves a reference to the first element stored in this @ref IntrusiveForwardList.
		ConstReference	Front() const;
		//! Retrieves a reference to the first element stored in this @ref IntrusiveForwardList.
		Reference		Front();

		//! Removes the head element of this @ref IntrusiveForwardList, reducing its size by one element.
		template <typename Disposer>
		void			PopFrontAndDispose( Disposer disposer );

		//! Adds the passed-in item to the head of this @ref IntrusiveForwardList.
		void			PushFront( Reference item );

	// - CONTAINER DUPLICATION ---------------------------

		template <typename Disposer, typename ElementCloner>
		ETInlineHint void	CloneFrom( const ::Eldritch2::IntrusiveForwardList<StoredObject>& containerTemplate, Disposer disposer, ElementCloner cloner );

		ETInlineHint void	Swap( ::Eldritch2::IntrusiveForwardList<StoredObject>& other );

	// - CONTAINER MANIPULATION --------------------------

		//! Inserts an element, shifting all antecedent elements down one position.
		ETInlineHint Iterator	Insert( const Iterator location, Reference item );

		ETInlineHint Iterator	InsertAfter( const Iterator location, Reference item );

		ETInlineHint Iterator	Erase( const Iterator position );
		ETInlineHint void		Erase( const Iterator begin, const Iterator end );

		template <typename Disposer>
		ETInlineHint Iterator	EraseAndDispose( const Iterator position, Disposer disposer );
		template <typename Disposer>
		ETInlineHint void		EraseAndDispose( const Iterator begin, const Iterator end, Disposer disposer );

		ETInlineHint void		EraseAfter( const Iterator location );

		template <typename Disposer>
		ETInlineHint void		EraseAfterAndDispose( const Iterator location, Disposer disposer );

		ETInlineHint void		Clear();

		template <typename Disposer>
		ETInlineHint void		ClearAndDispose( Disposer disposer );

	// - CONTENT QUERY -----------------------------------

		ETInlineHint operator bool() const;

		ETInlineHint SizeType	Size() const;

		ETInlineHint bool		Empty() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_container;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/IntrusiveForwardList.inl>
//------------------------------------------------------------------//