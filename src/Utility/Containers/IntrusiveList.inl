/*==================================================================*\
  IntrusiveList.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Algorithms.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, typename NodeTraits>
	IntrusiveList<StoredObject, NodeTraits>::IntrusiveList( const UnderlyingValueTraits& valueTraits ) : _underlyingContainer( valueTraits ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	IntrusiveList<StoredObject, NodeTraits>::~IntrusiveList() {}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename Predicate>
	typename IntrusiveList<StoredObject, NodeTraits>::Iterator IntrusiveList<StoredObject, NodeTraits>::Find( Predicate predicate ) {
		return this->Find( predicate, this->Begin() );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename Predicate>
	typename IntrusiveList<StoredObject, NodeTraits>::Iterator IntrusiveList<StoredObject, NodeTraits>::Find( Predicate predicate, Iterator searchHint ) {
		return ::Eldritch2::Utility::Find( searchHint, this->End(), predicate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename Predicate>
	typename IntrusiveList<StoredObject, NodeTraits>::ConstIterator IntrusiveList<StoredObject, NodeTraits>::Find( Predicate predicate ) const {
		return this->Find( predicate, this->Begin() );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename Predicate>
	typename IntrusiveList<StoredObject, NodeTraits>::ConstIterator IntrusiveList<StoredObject, NodeTraits>::Find( Predicate predicate, ConstIterator searchHint ) const {
		return ::Eldritch2::Utility::Find( searchHint, this->End(), predicate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename Predicate>
	void IntrusiveList<StoredObject, NodeTraits>::RemoveIf( Predicate predicate ) {
		_underlyingContainer.remove_if( predicate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename Predicate, typename Disposer>
	void IntrusiveList<StoredObject, NodeTraits>::RemoveAndDisposeIf( Predicate predicate, Disposer disposer ) {
		_underlyingContainer.remove_and_dispose_if( predicate, [&disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	typename IntrusiveList<StoredObject, NodeTraits>::Iterator IntrusiveList<StoredObject, NodeTraits>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	typename IntrusiveList<StoredObject, NodeTraits>::ConstIterator IntrusiveList<StoredObject, NodeTraits>::Begin() const {
		return _underlyingContainer.cbegin();
	}

// ---------------------------------------------------

	template <class StoredObject, class ValueTraits>
	typename IntrusiveList<StoredObject, ValueTraits>::ConstIterator IntrusiveList<StoredObject, ValueTraits>::ConstBegin() const {
		return _underlyingContainer.cbegin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	typename IntrusiveList<StoredObject, NodeTraits>::Iterator IntrusiveList<StoredObject, NodeTraits>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	typename IntrusiveList<StoredObject, NodeTraits>::ConstIterator IntrusiveList<StoredObject, NodeTraits>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <class StoredObject, class ValueTraits>
	typename IntrusiveList<StoredObject, ValueTraits>::ConstIterator IntrusiveList<StoredObject, ValueTraits>::ConstEnd() const {
		return _underlyingContainer.cend();
	}

// ---------------------------------------------------

	template <class StoredObject, class ValueTraits>
	typename IntrusiveList<StoredObject, ValueTraits>::Iterator IntrusiveList<StoredObject, ValueTraits>::IteratorTo( Reference element ) {
		return _underlyingContainer.iterator_to( element );
	}

// ---------------------------------------------------

	template <class StoredObject, class ValueTraits>
	typename IntrusiveList<StoredObject, ValueTraits>::ConstIterator IntrusiveList<StoredObject, ValueTraits>::IteratorTo( ConstReference element ) const {
		return _underlyingContainer.iterator_to( element );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	typename IntrusiveList<StoredObject, NodeTraits>::Reference IntrusiveList<StoredObject, NodeTraits>::Front() {
		return _underlyingContainer.front();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	typename IntrusiveList<StoredObject, NodeTraits>::ConstReference IntrusiveList<StoredObject, NodeTraits>::Front() const {
		return _underlyingContainer.front();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	void IntrusiveList<StoredObject, NodeTraits>::PushFront( Reference item ) {
		_underlyingContainer.push_front( item );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	void IntrusiveList<StoredObject, NodeTraits>::PopFront() {
		_underlyingContainer.pop_front();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename Disposer>
	void IntrusiveList<StoredObject, NodeTraits>::PopFrontAndDispose( Disposer disposer ) {
		_underlyingContainer.pop_front_and_dispose( [disposer] ( ValueType* const value ) {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	typename IntrusiveList<StoredObject, NodeTraits>::Reference IntrusiveList<StoredObject, NodeTraits>::Back() {
		return _underlyingContainer.back();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	typename IntrusiveList<StoredObject, NodeTraits>::ConstReference IntrusiveList<StoredObject, NodeTraits>::Back() const {
		return _underlyingContainer.back();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	void IntrusiveList<StoredObject, NodeTraits>::PushBack( Reference item ) {
		_underlyingContainer.push_back( item );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	void IntrusiveList<StoredObject, NodeTraits>::PopBack() {
		_underlyingContainer.pop_back();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename Disposer>
	void IntrusiveList<StoredObject, NodeTraits>::PopBackAndDispose( Disposer disposer ) {
		_underlyingContainer.pop_back_and_dispose( [disposer] ( ValueType* const value ) {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename AlternateNodeTraits, typename Disposer, typename ElementCloner>
	void IntrusiveList<StoredObject, NodeTraits>::CloneFrom( const ::Eldritch2::IntrusiveList<StoredObject, AlternateNodeTraits>& containerTemplate, Disposer disposer, ElementCloner cloner ) {
		_underlyingContainer.clone_from( containerTemplate, cloner, [disposer] ( ValueType* const existingElement ) mutable {
			disposer( *existingElement );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	void IntrusiveList<StoredObject, NodeTraits>::Swap( ::Eldritch2::IntrusiveList<StoredObject, NodeTraits>& other ) {
		_underlyingContainer.swap( other );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	typename IntrusiveList<StoredObject, NodeTraits>::Iterator IntrusiveList<StoredObject, NodeTraits>::Insert( Iterator location, Reference item ) {
		return _underlyingContainer.insert( location, item );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	typename IntrusiveList<StoredObject, NodeTraits>::Iterator IntrusiveList<StoredObject, NodeTraits>::InsertAfter( Iterator location, Reference item ) {
		return _underlyingContainer.insert_after( location, item );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	void IntrusiveList<StoredObject, NodeTraits>::Erase( Iterator position ) {
		_underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	void IntrusiveList<StoredObject, NodeTraits>::Erase( Iterator begin, Iterator end ) {
		_underlyingContainer.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename Disposer>
	void IntrusiveList<StoredObject, NodeTraits>::EraseAndDispose( Reference item, Disposer disposer ) {
		this->EraseAndDispose( _underlyingContainer.iterator_to( item ), disposer );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename Disposer>
	void IntrusiveList<StoredObject, NodeTraits>::EraseAndDispose( Iterator position, Disposer disposer ) {
		_underlyingContainer.erase_and_dispose( position, [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename Disposer>
	void IntrusiveList<StoredObject, NodeTraits>::EraseAndDispose( Iterator begin, Iterator end, Disposer disposer ) {
		_underlyingContainer.erase_and_dispose( begin, end, [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	void IntrusiveList<StoredObject, NodeTraits>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	template <typename Disposer>
	void IntrusiveList<StoredObject, NodeTraits>::ClearAndDispose( Disposer disposer ) {
		_underlyingContainer.clear_and_dispose( [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	typename IntrusiveList<StoredObject, NodeTraits>::SizeType IntrusiveList<StoredObject, NodeTraits>::Size() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	bool IntrusiveList<StoredObject, NodeTraits>::Empty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	IntrusiveList<StoredObject, NodeTraits>::operator bool() const {
		return !Empty();
	}

}	// namespace Eldritch2

namespace std {

	template <typename StoredObject, typename NodeTraits>
	ETInlineHint ETNoAliasHint auto begin( ::Eldritch2::IntrusiveList<StoredObject, NodeTraits>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	ETInlineHint ETNoAliasHint auto begin( const ::Eldritch2::IntrusiveList<StoredObject, NodeTraits>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	ETInlineHint ETNoAliasHint auto end( ::Eldritch2::IntrusiveList<StoredObject, NodeTraits>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename NodeTraits>
	ETInlineHint ETNoAliasHint auto end( const ::Eldritch2::IntrusiveList<StoredObject, NodeTraits>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std