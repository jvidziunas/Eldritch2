/*==================================================================*\
  IntrusiveList.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value>
	ETInlineHint IntrusiveList<Value>::IntrusiveList( IntrusiveList&& moveSource ) : _underlyingContainer( eastl::move( moveSource._underlyingContainer ) ) {}

// ---------------------------------------------------

	template <typename Value>
	template <typename Predicate>
	ETInlineHint typename IntrusiveList<Value>::ConstIterator IntrusiveList<Value>::Find( ConstIterator searchHint, Predicate predicate ) const {
		return eastl::find_if( searchHint, _underlyingContainer.end(), predicate );
	}

// ---------------------------------------------------

	template <typename Value>
	template <typename Predicate>
	ETInlineHint typename IntrusiveList<Value>::Iterator IntrusiveList<Value>::Find( Iterator searchHint, Predicate predicate ) {
		return eastl::find_if( searchHint, _underlyingContainer.end(), predicate );
	}

// ---------------------------------------------------

	template <typename Value>
	template <typename Predicate>
	ETInlineHint typename IntrusiveList<Value>::ConstIterator IntrusiveList<Value>::Find( Predicate predicate ) const {
		return Find( _underlyingContainer.begin(), predicate );
	}

// ---------------------------------------------------

	template <typename Value>
	template <typename Predicate>
	ETInlineHint typename IntrusiveList<Value>::Iterator IntrusiveList<Value>::Find( Predicate predicate ) {
		return Find( _underlyingContainer.begin(), predicate );
	}

// ---------------------------------------------------

	template <typename Value>
	template <typename Predicate>
	ETInlineHint void IntrusiveList<Value>::RemoveIf( Predicate predicate ) {
		RemoveAndDisposeIf( predicate, [] ( Reference /*unused*/ ) {} );
	}

// ---------------------------------------------------

	template <typename Value>
	template <typename Predicate, typename Disposer>
	ETInlineHint void IntrusiveList<Value>::RemoveAndDisposeIf( Predicate predicate, Disposer disposer ) {
		for( auto element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element ) ) {
				auto&	object( *element );

				element = _underlyingContainer.erase( element );
				disposer( object );

				continue;
			}

			++element;
		}
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveList<Value>::ConstIterator IntrusiveList<Value>::ConstBegin() const {
		return _underlyingContainer.cbegin();
	}

// ---------------------------------------------------

	template <class Value>
	ETInlineHint typename IntrusiveList<Value>::ConstIterator IntrusiveList<Value>::ConstEnd() const {
		return _underlyingContainer.cend();
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::ConstIterator IntrusiveList<Value>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::Iterator IntrusiveList<Value>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::ConstIterator IntrusiveList<Value>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::Iterator IntrusiveList<Value>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::ConstIterator IntrusiveList<Value>::IteratorTo( ConstReference element ) const {
		return _underlyingContainer.locate( element );
	}

// ---------------------------------------------------
	
	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::Iterator IntrusiveList<Value>::IteratorTo( Reference element ) {
		return _underlyingContainer.locate( element );
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::ConstReference IntrusiveList<Value>::Front() const {
		return _underlyingContainer.front();
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::Reference IntrusiveList<Value>::Front() {
		return _underlyingContainer.front();
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::ConstReference IntrusiveList<Value>::Back() const {
		return _underlyingContainer.back();
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::Reference IntrusiveList<Value>::Back() {
		return _underlyingContainer.back();
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint void IntrusiveList<Value>::PushFront( Reference item ) {
		_underlyingContainer.push_front( item );
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint void IntrusiveList<Value>::PopFront() {
		_underlyingContainer.pop_front();
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint void IntrusiveList<Value>::PushBack( Reference item ) {
		_underlyingContainer.push_back( item );
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint void IntrusiveList<Value>::PopBack() {
		_underlyingContainer.pop_back();
	}

// ---------------------------------------------------

	template <typename Value>
	template <typename Disposer>
	ETInlineHint void IntrusiveList<Value>::PopFrontAndDispose( Disposer disposer ) {
		auto&	element( _underlyingContainer.front() );

		_underlyingContainer.pop_front();

		disposer( element );
	}

// ---------------------------------------------------

	template <typename Value>
	template <typename Disposer>
	ETInlineHint void IntrusiveList<Value>::PopBackAndDispose( Disposer disposer ) {
		auto&	element( _underlyingContainer.back() );

		_underlyingContainer.pop_back();

		disposer( element );
	}

// ---------------------------------------------------

	template <typename Value>
	template <typename Disposer, typename ElementCloner>
	ETInlineHint void IntrusiveList<Value>::CloneFrom( const IntrusiveList<Value>& containerTemplate, Disposer disposer, ElementCloner cloner ) {
		ClearAndDispose( disposer );

		for( const auto& element : containerTemplate._underlyingContainer ) {
			_underlyingContainer.push_back( cloner( element ) );
		}
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint void IntrusiveList<Value>::Swap( IntrusiveList<Value>& other ) {
		_underlyingContainer.swap( other );
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::Iterator IntrusiveList<Value>::Insert( Iterator location, Reference item ) {
		return _underlyingContainer.insert( location, item );
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::Iterator IntrusiveList<Value>::Erase( Iterator position ) {
		return _underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::Iterator IntrusiveList<Value>::Erase( Iterator begin, Iterator end ) {
		return _underlyingContainer.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename Value>
	template <typename Disposer>
	ETInlineHint typename IntrusiveList<Value>::Iterator IntrusiveList<Value>::EraseAndDispose( Iterator position, Disposer disposer ) {
		auto&	element( *position );

		position = _underlyingContainer.erase( position );

		disposer( element );

		return position;
	}

// ---------------------------------------------------

	template <typename Value>
	template <typename Disposer>
	ETInlineHint typename IntrusiveList<Value>::Iterator IntrusiveList<Value>::EraseAndDispose( Iterator begin, Iterator end, Disposer disposer ) {
		while( begin != end ) {
			begin = EraseAndDispose( begin, disposer );
		}

		return begin;
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint void IntrusiveList<Value>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Value>
	template <typename Disposer>
	ETInlineHint void IntrusiveList<Value>::ClearAndDispose( Disposer disposer ) {
		while( !_underlyingContainer.empty() ) {
			PopFrontAndDispose( disposer );
		}
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint typename IntrusiveList<Value>::SizeType IntrusiveList<Value>::GetSize() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint bool IntrusiveList<Value>::IsEmpty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Value>
	ETInlineHint IntrusiveList<Value>::operator bool() const {
		return !_underlyingContainer.empty();
	}

}	// namespace Eldritch2