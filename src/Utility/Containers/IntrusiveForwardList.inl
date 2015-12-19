/*==================================================================*\
  IntrusiveForwardList.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class StoredObject>
	template <typename SourceIterator>
	ETInlineHint IntrusiveForwardList<StoredObject>::IntrusiveForwardList( SourceIterator elementBegin, SourceIterator elementEnd ) {
		auto	current( Begin() );

		while( elementBegin != elementEnd ) {
			current = InsertAfter( current, elementBegin++ );
		}
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint IntrusiveForwardList<StoredObject>::IntrusiveForwardList( ::Eldritch2::IntrusiveForwardList<StoredObject>&& sourceContainer ) {
		auto&	thisContainer( *this );
		auto	current( Begin() );

		sourceContainer.ClearAndDispose( [&current, &thisContainer] ( Reference newElement ) {
			current = thisContainer.InsertAfter( current, newElement );
		} );
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Predicate>
	ETInlineHint typename IntrusiveForwardList<StoredObject>::Iterator IntrusiveForwardList<StoredObject>::Find( Predicate predicate ) {
		return ::Eldritch2::Utility::Find( _container.begin(), _container.end(), predicate );
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Predicate>
	ETInlineHint typename IntrusiveForwardList<StoredObject>::Iterator IntrusiveForwardList<StoredObject>::Find( Predicate predicate, Iterator searchHint ) {
		return ::Eldritch2::Utility::Find( searchHint, _container.end(), predicate );
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Predicate>
	ETInlineHint typename IntrusiveForwardList<StoredObject>::ConstIterator IntrusiveForwardList<StoredObject>::Find( Predicate predicate ) const {
		return ::Eldritch2::Utility::Find( _container.begin(), _container.end(), predicate );
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Predicate>
	ETInlineHint typename IntrusiveForwardList<StoredObject>::ConstIterator IntrusiveForwardList<StoredObject>::Find( Predicate predicate, ConstIterator searchHint ) const {
		return ::Eldritch2::Utility::Find( searchHint, _container.end(), predicate );
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Predicate>
	ETInlineHint void IntrusiveForwardList<StoredObject>::RemoveIf( Predicate predicate ) {
		RemoveAndDisposeIf( predicate, [] ( Reference /*unused*/ ) {} );
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Predicate, typename Disposer>
	ETInlineHint void IntrusiveForwardList<StoredObject>::RemoveAndDisposeIf( Predicate predicate, Disposer disposer ) {
		for( Iterator current( Begin() ), end( End() ); current != end; ) {
			if( predicate( *current ) ) {
				current = EraseAndDispose( current, disposer );
				continue;
			}

			++current;
		}
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Predicate>
	ETInlineHint void IntrusiveForwardList<StoredObject>::Sort( Predicate predicate ) {
		::Eldritch2::Utility::Sort( Begin(), End(), predicate );
	}

// ---------------------------------------------------

	template <class StoredObject>
	typename IntrusiveForwardList<StoredObject>::Iterator IntrusiveForwardList<StoredObject>::Begin() {
		return _container.begin();
	}

// ---------------------------------------------------

	template <class StoredObject>
	typename IntrusiveForwardList<StoredObject>::ConstIterator IntrusiveForwardList<StoredObject>::Begin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <class StoredObject>
	typename IntrusiveForwardList<StoredObject>::ConstIterator IntrusiveForwardList<StoredObject>::ConstBegin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <class StoredObject>
	typename IntrusiveForwardList<StoredObject>::Iterator IntrusiveForwardList<StoredObject>::End() {
		return _container.end();
	}

// ---------------------------------------------------

	template <class StoredObject>
	typename IntrusiveForwardList<StoredObject>::ConstIterator IntrusiveForwardList<StoredObject>::End() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <class StoredObject>
	typename IntrusiveForwardList<StoredObject>::ConstIterator IntrusiveForwardList<StoredObject>::ConstEnd() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <class StoredObject>
	typename IntrusiveForwardList<StoredObject>::Iterator IntrusiveForwardList<StoredObject>::IteratorTo( Reference element ) {
		return UnderlyingContainer::get_iterator( &element );
	}

// ---------------------------------------------------

	template <class StoredObject>
	typename IntrusiveForwardList<StoredObject>::ConstIterator IntrusiveForwardList<StoredObject>::IteratorTo( ConstReference element ) const {
		return UnderlyingContainer::get_iterator( &element );
	}

// ---------------------------------------------------

	template <class StoredObject>
	typename IntrusiveForwardList<StoredObject>::Reference IntrusiveForwardList<StoredObject>::Front() {
		return *_container.front();
	}

// ---------------------------------------------------

	template <class StoredObject>
	typename IntrusiveForwardList<StoredObject>::ConstReference IntrusiveForwardList<StoredObject>::Front() const {
		return *_container.front();
	}

// ---------------------------------------------------

	template <class StoredObject>
	void IntrusiveForwardList<StoredObject>::PushFront( Reference item ) {
		_container.push_front( &item );
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Disposer>
	void IntrusiveForwardList<StoredObject>::PopFrontAndDispose( Disposer disposer ) {
		auto&	front( Front() );
		_container.pop_front();

		disposer( front );
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Disposer, typename ElementCloner>
	ETInlineHint void IntrusiveForwardList<StoredObject>::CloneFrom( const ::Eldritch2::IntrusiveForwardList<StoredObject>& containerTemplate, Disposer disposer, ElementCloner cloner ) {
		ClearAndDispose( disposer );

		Iterator	current( Begin() );

		for( const auto& sourceElement : containerTemplate ) {
			current = InsertAfter( current, cloner( sourceElement ) );
		}
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint void IntrusiveForwardList<StoredObject>::Swap( ::Eldritch2::IntrusiveForwardList<StoredObject>& other ) {
		_container.swap( other );
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint typename IntrusiveForwardList<StoredObject>::Iterator IntrusiveForwardList<StoredObject>::Insert( const Iterator location, Reference item ) {
		return _container.insert( location, &item );
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint typename IntrusiveForwardList<StoredObject>::Iterator IntrusiveForwardList<StoredObject>::InsertAfter( const Iterator location, Reference item ) {
		return _container.insert_after( location, &item );
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint typename IntrusiveForwardList<StoredObject>::Iterator IntrusiveForwardList<StoredObject>::Erase( const Iterator position ) {
		return EraseAndDispose( position, [] ( Reference /*unused*/ ) {} );
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint void IntrusiveForwardList<StoredObject>::Erase( const Iterator begin, const Iterator end ) {
		_container.erase( begin, end );
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Disposer>
	ETInlineHint typename IntrusiveForwardList<StoredObject>::Iterator IntrusiveForwardList<StoredObject>::EraseAndDispose( const Iterator position, Disposer disposer ) {
		auto&	element( *position );
		auto	result( _container.erase( position ) );

		disposer( element );

		return result;
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Disposer>
	ETInlineHint void IntrusiveForwardList<StoredObject>::EraseAndDispose( const Iterator begin, const Iterator end, Disposer disposer ) {
		if( begin != end ) {
			while( begin.next() != end.node() ) {
				EraseAfterAndDispose( first, disposer );
			}
				
			EraseAfterAndDispose( _container.previous( first ), disposer );
		}

		return begin;
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint void IntrusiveForwardList<StoredObject>::EraseAfter( const Iterator location ) {
		EraseAfterAndDispose( location, [] ( Reference /*unused*/ ) {} );
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Disposer>
	ETInlineHint void IntrusiveForwardList<StoredObject>::EraseAfterAndDispose( const Iterator location, Disposer disposer ) {
		auto&	elementToDispose( location->next );

		_container.erase_after( location );

		disposer( elementToDispose );
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint void IntrusiveForwardList<StoredObject>::Clear() {
		ClearAndDispose( [] ( Reference /*unused*/ ) {} );
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Disposer>
	ETInlineHint void IntrusiveForwardList<StoredObject>::ClearAndDispose( Disposer disposer ) {
		while( !Empty() ) {
			PopFrontAndDispose( disposer );
		}
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint typename IntrusiveForwardList<StoredObject>::SizeType IntrusiveForwardList<StoredObject>::Size() const {
		return _container.size();
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint bool IntrusiveForwardList<StoredObject>::Empty() const {
		return _container.empty();
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint IntrusiveForwardList<StoredObject>::operator bool() const {
		return !this->Empty();
	}

}	// namespace Eldritch2

namespace std {

	template <class StoredObject>
	ETInlineHint ETNoAliasHint auto begin( ::Eldritch2::IntrusiveForwardList<StoredObject>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint ETNoAliasHint auto begin( const ::Eldritch2::IntrusiveForwardList<StoredObject>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint ETNoAliasHint auto end( ::Eldritch2::IntrusiveForwardList<StoredObject>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <class StoredObject>
	ETInlineHint ETNoAliasHint auto end( const ::Eldritch2::IntrusiveForwardList<StoredObject>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std