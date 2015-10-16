/*==================================================================*\
  OrderedSet.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, class Allocator>
	ETInlineHint OrderedSet<StoredObject, Allocator>::OrderedSet( AllocatorType&& allocator ) : _underlyingContainer( ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	template <typename InputIterator>
	ETInlineHint OrderedSet<StoredObject, Allocator>::OrderedSet( InputIterator begin, InputIterator end, AllocatorType&& allocator ) : _underlyingContainer( ::std::move( allocator ) ) {
		while( begin != end ) {
			this->Insert( *begin++ );
		}
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	template <class AlternateAllocator>
	ETInlineHint OrderedSet<StoredObject, Allocator>::OrderedSet( const ::Eldritch2::OrderedSet<StoredObject, AlternateAllocator>& containerTemplate, AllocatorType&& allocator ) : _underlyingContainer( ::std::move( allocator ) ) {
		for( auto&& value : containerTemplate ) {
			this->Insert( value );
		}
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint OrderedSet<StoredObject, Allocator>::OrderedSet( ::Eldritch2::OrderedSet<StoredObject, Allocator>&& moveSource ) : _underlyingContainer( ::std::move( moveSource ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint typename OrderedSet<StoredObject, Allocator>::Iterator OrderedSet<StoredObject, Allocator>::Find( const ValueType& itemTemplate ) {
		return _underlyingContainer.find( itemTemplate );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint typename OrderedSet<StoredObject, Allocator>::ConstIterator OrderedSet<StoredObject, Allocator>::Find( const ValueType& itemTemplate ) const {
		return _underlyingContainer.find( itemTemplate );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	template <typename Predicate>
	ETInlineHint void OrderedSet<StoredObject, Allocator>::RemoveIf( Predicate predicate ) {
		for( Iterator element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element ) ) {
				_underlyingContainer.erase( element++ );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	template <typename ExtraArgumentType, typename Predicate>
	ETInlineHint void OrderedSet<StoredObject, Allocator>::RemoveIf( ExtraArgumentType extraArgument, Predicate predicate ) {
		for( Iterator element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element, extraArgument ) ) {
				_underlyingContainer.erase( element++ );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint typename OrderedSet<StoredObject, Allocator>::Iterator OrderedSet<StoredObject, Allocator>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint typename OrderedSet<StoredObject, Allocator>::ConstIterator OrderedSet<StoredObject, Allocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint typename OrderedSet<StoredObject, Allocator>::ConstIterator OrderedSet<StoredObject, Allocator>::ConstBegin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint typename OrderedSet<StoredObject, Allocator>::Iterator OrderedSet<StoredObject, Allocator>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint typename OrderedSet<StoredObject, Allocator>::ConstIterator OrderedSet<StoredObject, Allocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint typename OrderedSet<StoredObject, Allocator>::ConstIterator OrderedSet<StoredObject, Allocator>::ConstEnd() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint void OrderedSet<StoredObject, Allocator>::CloneFrom( const ::Eldritch2::OrderedSet<StoredObject, Allocator>& containerTemplate ) {
		this->Clear();
		for( auto&& value : containerTemplate ) {
			this->Insert( value );
		}
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint void OrderedSet<StoredObject, Allocator>::Swap( ::Eldritch2::OrderedSet<StoredObject, Allocator>& other ) {
		_underlyingContainer.swap( other );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint typename OrderedSet<StoredObject, Allocator>::InsertResult OrderedSet<StoredObject, Allocator>::Insert( const ValueType& valueTemplate ) {
		return _underlyingContainer.insert( valueTemplate );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint void OrderedSet<StoredObject, Allocator>::Erase( Iterator position ) {
		_underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint void OrderedSet<StoredObject, Allocator>::Erase( Iterator begin, Iterator end ) {
		while( begin != end ) {
			_underlyingContainer.erase( begin++ );
		}
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint void OrderedSet<StoredObject, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint typename OrderedSet<StoredObject, Allocator>::SizeType OrderedSet<StoredObject, Allocator>::Size() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint bool OrderedSet<StoredObject, Allocator>::Empty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint OrderedSet<StoredObject, Allocator>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint typename const OrderedSet<StoredObject, Allocator>::AllocatorType& OrderedSet<StoredObject, Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2

namespace std {

	template <typename StoredObject, class Allocator>
	ETInlineHint ETNoAliasHint auto begin( ::Eldritch2::OrderedSet<StoredObject, Allocator>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint ETNoAliasHint auto cbegin( const ::Eldritch2::OrderedSet<StoredObject, Allocator>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint ETNoAliasHint auto end( ::Eldritch2::OrderedSet<StoredObject, Allocator>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Allocator>
	ETInlineHint ETNoAliasHint auto cend( const ::Eldritch2::OrderedSet<StoredObject, Allocator>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std