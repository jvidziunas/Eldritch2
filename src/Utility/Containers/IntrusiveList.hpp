/*==================================================================*\
  IntrusiveList.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <boost/intrusive/list.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, typename NodeTraits>
	class IntrusiveList {
	// - TYPE PUBLISHING ---------------------------------

		typedef ::boost::intrusive::list<StoredObject, NodeTraits, ::boost::intrusive::cache_last<true>>	UnderlyingContainer;

	public:
		typedef typename UnderlyingContainer::value_traits				UnderlyingValueTraits;
		typedef typename UnderlyingContainer::value_type				ValueType;
		typedef	typename UnderlyingContainer::reference					Reference;
		typedef	typename UnderlyingContainer::const_reference			ConstReference;
		typedef typename UnderlyingContainer::iterator					Iterator;
		typedef typename UnderlyingContainer::reverse_iterator			ReverseIterator;
		typedef typename UnderlyingContainer::const_iterator			ConstIterator;
		typedef typename UnderlyingContainer::const_reverse_iterator	ConstReverseIterator;
		typedef size_t													SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this IntrusiveList instance.
		IntrusiveList( const UnderlyingValueTraits& valueTraits = UnderlyingValueTraits() );

		// Destroys this IntrusiveList instance.
		~IntrusiveList();

	// - ALGORITHMS --------------------------------------

		template <typename Predicate>
		ETInlineHint Iterator		Find( Predicate predicate );
		template <typename Predicate>
		ETInlineHint Iterator		Find( Predicate predicate, Iterator searchHint );
		template <typename Predicate>
		ETInlineHint ConstIterator	Find( Predicate predicate ) const;
		template <typename Predicate>
		ETInlineHint ConstIterator	Find( Predicate predicate, ConstIterator searchHint ) const;

		template <typename Predicate>
		ETInlineHint void	RemoveIf( Predicate predicate );

		template <typename Predicate, typename Disposer>
		ETInlineHint void	RemoveAndDisposeIf( Predicate predicate, Disposer disposer );

	// - ELEMENT ITERATION -------------------------------

		Iterator		Begin();
		ConstIterator	Begin() const;

		ConstIterator	ConstBegin() const;

		Iterator		End();
		ConstIterator	End() const;

		ConstIterator	ConstEnd() const;

		Iterator		IteratorTo( Reference element );
		ConstIterator	IteratorTo( ConstReference element ) const;

	// - END POINT MANIPULATION --------------------------

		// Retrieves a reference to the first element stored in this IntrusiveList.
		Reference		Front();
		// Retrieves a reference to the first element stored in this IntrusiveList.
		ConstReference	Front() const;

		// Adds the passed-in item to the head of this IntrusiveList.
		void			PushFront( Reference item );

		// Removes the head element of this IntrusiveList, reducing its size by one element.
		void			PopFront();

		// Removes the head element of this IntrusiveList, reducing its size by one element.
		template <typename Disposer>
		void			PopFrontAndDispose( Disposer disposer );

		// Retrieves a reference to the last element stored in this IntrusiveList.
		Reference		Back();
		// Retrieves a reference to the last element stored in this IntrusiveList.
		ConstReference	Back() const;

		// Adds the passed-in item to the tail of this IntrusiveList.
		void			PushBack( Reference item );

		// Removes the tail element of this IntrusiveList, reducing its size by one element.
		void			PopBack();

		// Removes the tail element of this IntrusiveList, reducing its size by one element. Operator() is invoked on the specified disposer, with the popped element passed in as the sole parameter.
		template <typename Disposer>
		void			PopBackAndDispose( Disposer disposer );

	// - CONTAINER DUPLICATION ---------------------------

		template <typename AlternateNodeTraits, typename Disposer, typename ElementCloner>
		ETInlineHint void	CloneFrom( const ::Eldritch2::IntrusiveList<StoredObject, AlternateNodeTraits>& containerTemplate, Disposer disposer, ElementCloner cloner );

		ETInlineHint void	Swap( ::Eldritch2::IntrusiveList<StoredObject, NodeTraits>& other );

	// - CONTAINER MANIPULATION --------------------------

		// Inserts an element at the position specified, shifting all antecedent elements down one position.
		ETInlineHint Iterator	Insert( Iterator location, Reference item );

		ETInlineHint Iterator	InsertAfter( Iterator location, Reference item );

		ETInlineHint void	Erase( Reference item );
		ETInlineHint void	Erase( Iterator position );
		ETInlineHint void	Erase( Iterator begin, Iterator end );

		template <typename Disposer>
		ETInlineHint void	EraseAndDispose( Reference item, Disposer disposer );
		template <typename Disposer>
		ETInlineHint void	EraseAndDispose( Iterator position, Disposer disposer );
		template <typename Disposer>
		ETInlineHint void	EraseAndDispose( Iterator begin, Iterator end, Disposer disposer );

		ETInlineHint void	Clear();

		template <typename Disposer>
		ETInlineHint void	ClearAndDispose( Disposer disposer );

	// - CONTENT QUERY -----------------------------------

		ETInlineHint SizeType	Size() const;

		ETInlineHint bool	Empty() const;

		ETInlineHint	operator bool() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_underlyingContainer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/IntrusiveList.inl>
//------------------------------------------------------------------//