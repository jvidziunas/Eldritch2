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
	template <typename InputIterator>
	ETInlineHint IntrusiveForwardList<Value>::IntrusiveForwardList(
		InputIterator begin,
		InputIterator end
	) : _container( begin, end ) {}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint IntrusiveForwardList<Value>::IntrusiveForwardList(
		IntrusiveForwardList<Value>&& list
	) : _container( eastl::move( list._container ) ) {}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::FindIf( Predicate predicate, Iterator searchHint ) {
		return FindIf( searchHint, _container.end(), predicate );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::FindIf( Predicate predicate ) {
		return FindIf( _container.begin(), _container.end(), predicate );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::FindIf( Predicate predicate, ConstIterator searchHint ) const {
		return FindIf( searchHint, _container.end(), predicate );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::FindIf( Predicate predicate ) const {
		return FindIf( _container.begin(), _container.end(), predicate );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate>
	ETInlineHint void IntrusiveForwardList<Value>::EraseIf( Predicate predicate ) {
		EraseAndDisposeIf( predicate, [] ( Reference /*unused*/ ) {} );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Predicate, typename Disposer>
	ETInlineHint void IntrusiveForwardList<Value>::EraseAndDisposeIf( Predicate predicate, Disposer disposer ) {
		for (auto current( Begin() ), end( End() ); current != end;) {
			if (predicate( *current )) {
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
		_container.sort( predicate );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::ConstBegin() const {
		return _container.cbegin();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::ConstEnd() const {
		return _container.cend();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::Begin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Begin() {
		return _container.begin();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::End() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::End() {
		return _container.end();
	}

// ---------------------------------------------------
	
	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::GetIteratorTo( Reference element ) {
		return _container.locate( element );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstIterator IntrusiveForwardList<Value>::GetIteratorTo( ConstReference element ) const {
		return _container.locate( element );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::ConstReference IntrusiveForwardList<Value>::Front() const {
		return _container.front();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Reference IntrusiveForwardList<Value>::Front() {
		return _container.front();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint void IntrusiveForwardList<Value>::Prepend( Reference item ) {
		_container.push_front( item );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Disposer>
	ETInlineHint void IntrusiveForwardList<Value>::PopFrontAndDispose( Disposer disposer ) {
		auto&	front( _container.front() );

		_container.pop_front();

		disposer( front );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint void IntrusiveForwardList<Value>::PopFront() {
		_container.pop_front();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Insert( Iterator location, Reference item ) {
		return _container.insert( location, item );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::InsertAfter( Iterator location, Reference item ) {
		return _container.insert_after( location, item );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Erase( Iterator begin, Iterator end ) {
		return _container.erase( begin, end );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::Erase( Iterator position ) {
		return _container.erase( position );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::EraseAfter( Iterator beforeBegin, Iterator end ) {
		return _container.erase_after( beforeBegin, end );
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::EraseAfter( Iterator position ) {
		return _container.erase_after( position );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Disposer>
	ETInlineHint typename IntrusiveForwardList<Value>::Iterator IntrusiveForwardList<Value>::EraseAndDispose( Iterator position, Disposer disposer ) {
		auto&	element( *position );
		auto	result( _container.erase( position ) );

		disposer( element );

		return result;
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Disposer>
	ETInlineHint void IntrusiveForwardList<Value>::EraseAndDispose( Iterator begin, Iterator end, Disposer disposer ) {
		if (begin != end) {
			while (begin.next() != end.node()) {
				EraseAfterAndDispose( first, disposer );
			}
				
			EraseAfterAndDispose( _container.previous( first ), disposer );
		}

		return begin;
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint void IntrusiveForwardList<Value>::Clear() {
		_container.clear();
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Disposer>
	ETInlineHint void IntrusiveForwardList<Value>::ClearAndDispose( Disposer disposer ) {
		while (!_container.empty()) {
			PopFrontAndDispose( disposer );
		}
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveForwardList<Value>::SizeType IntrusiveForwardList<Value>::GetSize() const {
		return _container.size();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint bool IntrusiveForwardList<Value>::IsEmpty() const {
		return _container.empty();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint IntrusiveForwardList<Value>::operator bool() const {
		return !_container.empty();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint void Swap( IntrusiveForwardList<Value>& list0, IntrusiveForwardList<Value>& list1 ) {
		eastl::swap( list0._container, list1._container );
	}

}	// namespace Eldritch2