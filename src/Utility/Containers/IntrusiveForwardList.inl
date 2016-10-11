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

	template <class Value>
	template <typename SourceIterator>
	ETInlineHint IntrusiveForwardList<Value>::IntrusiveForwardList( SourceIterator elementBegin, SourceIterator elementEnd ) : _underlyingContainer( elementBegin, elementEnd ) {}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint IntrusiveForwardList<Value>::IntrusiveForwardList( Eldritch2::IntrusiveForwardList<Value>&& sourceContainer ) : _underlyingContainer( eastl::move( sourceContainer._underlyingContainer ) ) {
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Find( Predicate predicate, Iterator searchHint ) {
		return Eldritch2::Utility::Find( searchHint, _underlyingContainer.end(), predicate );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Find( Predicate predicate ) {
		return Eldritch2::Utility::Find( _underlyingContainer.begin(), _underlyingContainer.end(), predicate );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::Find( Predicate predicate, ConstIterator searchHint ) const {
		return Eldritch2::Utility::Find( searchHint, _underlyingContainer.end(), predicate );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::Find( Predicate predicate ) const {
		return Eldritch2::Utility::Find( _underlyingContainer.begin(), _underlyingContainer.end(), predicate );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate>
	ETInlineHint void IntrusiveForwardList<Value>::RemoveIf( Predicate predicate ) {
		RemoveAndDisposeIf( predicate, [] ( Reference /*unused*/ ) {} );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate, typename Disposer>
	ETInlineHint void IntrusiveForwardList<Value>::RemoveAndDisposeIf( Predicate predicate, Disposer disposer ) {
		for( Iterator current( Begin() ), end( End() ); current != end; ) {
			if( predicate( *current ) ) {
				current = EraseAndDispose( current, disposer );
				continue;
			}

			++current;
		}
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate>
	ETInlineHint void IntrusiveForwardList<Value>::Sort( Predicate predicate ) {
		_underlyingContainer.sort( predicate );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::ConstBegin() const {
		return _underlyingContainer.cbegin();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::ConstEnd() const {
		return _underlyingContainer.cend();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------
	
	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::IteratorTo( Reference element ) {
		return _underlyingContainer.locate( element );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::IteratorTo( ConstReference element ) const {
		return _underlyingContainer.locate( element );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstReference IntrusiveForwardList<Value>::Front() const {
		return _underlyingContainer.front();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Reference IntrusiveForwardList<Value>::Front() {
		return _underlyingContainer.front();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint void IntrusiveForwardList<Value>::PushFront( Reference item ) {
		_underlyingContainer.push_front( item );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Disposer>
	ETInlineHint void IntrusiveForwardList<Value>::PopFrontAndDispose( Disposer disposer ) {
		auto&	front( _underlyingContainer.front() );

		_underlyingContainer.pop_front();

		disposer( front );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint void IntrusiveForwardList<Value>::Swap( Eldritch2::IntrusiveForwardList<Value>& other ) {
		_underlyingContainer.swap( other );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Insert( Iterator location, Reference item ) {
		return _underlyingContainer.insert( location, item );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::InsertAfter( Iterator location, Reference item ) {
		return _underlyingContainer.insert_after( location, item );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Erase( Iterator begin, Iterator end ) {
		return _underlyingContainer.erase( begin, end );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Erase( Iterator position ) {
		return _underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::EraseAfter( Iterator beforeBegin, Iterator end ) {
		return _underlyingContainer.erase_after( beforeBegin, end );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::EraseAfter( Iterator position ) {
		return _underlyingContainer.erase_after( position );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Disposer>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::EraseAndDispose( Iterator position, Disposer disposer ) {
		auto&	element( *position );
		auto	result( _underlyingContainer.erase( position ) );

		disposer( element );

		return result;
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Disposer>
	ETInlineHint void IntrusiveForwardList<Value>::EraseAndDispose( Iterator begin, Iterator end, Disposer disposer ) {
		if( begin != end ) {
			while( begin.next() != end.node() ) {
				EraseAfterAndDispose( first, disposer );
			}
				
			EraseAfterAndDispose( _underlyingContainer.previous( first ), disposer );
		}

		return begin;
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint void IntrusiveForwardList<Value>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Disposer>
	ETInlineHint void IntrusiveForwardList<Value>::ClearAndDispose( Disposer disposer ) {
		while( !_underlyingContainer.empty() ) {
			PopFrontAndDispose( disposer );
		}
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::SizeType IntrusiveForwardList<Value>::GetSize() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint bool IntrusiveForwardList<Value>::IsEmpty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint IntrusiveForwardList<Value>::operator bool() const {
		return !_underlyingContainer.empty();
	}

}	// namespace Eldritch2