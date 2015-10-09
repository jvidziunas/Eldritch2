/*==================================================================*\
  OrderedMap.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename StoredObject, class Allocator>
	template <typename... AllocatorConstructorArguments>
	ETInlineHint OrderedMap<Key, StoredObject, Allocator>::OrderedMap( AllocatorConstructorArguments&&... allocatorConstructorArguments ) : _underlyingContainer( ::std::forward<AllocatorConstructorArguments>( allocatorConstructorArguments )... ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	template <typename InputIterator, typename... AllocatorConstructorArguments>
	ETInlineHint OrderedMap<Key, StoredObject, Allocator>::OrderedMap( InputIterator begin, InputIterator end, AllocatorConstructorArguments&&... allocatorConstructorArguments ) : _underlyingContainer( begin, end, ::std::forward<AllocatorConstructorArguments>( allocatorConstructorArguments )... ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	template <class AlternateAllocator, typename... AllocatorConstructorArguments>
	ETInlineHint OrderedMap<Key, StoredObject, Allocator>::OrderedMap( const ::Eldritch2::OrderedMap<Key, StoredObject, AlternateAllocator>& containerTemplate, AllocatorConstructorArguments&&... allocatorConstructorArguments ) : _underlyingContainer( containerTemplate.Begin(),
																																																														   containerTemplate.End(),
																																																														   ::std::forward<AllocatorConstructorArguments>( allocatorConstructorArguments )... ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint OrderedMap<Key, StoredObject, Allocator>::~OrderedMap() {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::Iterator OrderedMap<Key, StoredObject, Allocator>::Find( const KeyType& key ) {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::ConstIterator OrderedMap<Key, StoredObject, Allocator>::Find( const KeyType& key ) const {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	template <typename Predicate>
	ETInlineHint void OrderedMap<Key, StoredObject, Allocator>::RemoveIf( Predicate predicate ) {
		for( Iterator element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element ) ) {
				_underlyingContainer.erase( element++ );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	template <typename ExtraArgumentType, typename Predicate>
	ETInlineHint void OrderedMap<Key, StoredObject, Allocator>::RemoveIf( ExtraArgumentType extraArgument, Predicate predicate ) {
		for( Iterator element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element, extraArgument ) ) {
				_underlyingContainer.erase( element++ );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::Iterator OrderedMap<Key, StoredObject, Allocator>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::ConstIterator OrderedMap<Key, StoredObject, Allocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::ConstIterator OrderedMap<Key, StoredObject, Allocator>::ConstBegin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::Iterator OrderedMap<Key, StoredObject, Allocator>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::ConstIterator OrderedMap<Key, StoredObject, Allocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::ConstIterator OrderedMap<Key, StoredObject, Allocator>::ConstEnd() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::MappedType& OrderedMap<Key, StoredObject, Allocator>::operator[]( const KeyType& key ) {
		return static_cast<UnderlyingContainer&>( *this )[key];
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint void OrderedMap<Key, StoredObject, Allocator>::CloneFrom( const ::Eldritch2::OrderedMap<Key, StoredObject, Allocator>& containerTemplate ) {
		static_cast<UnderlyingContainer&>( *this ) = containerTemplate;
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint void OrderedMap<Key, StoredObject, Allocator>::Swap( ::Eldritch2::OrderedMap<Key, StoredObject, Allocator>& other ) {
		_underlyingContainer.swap( other );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::InsertResult OrderedMap<Key, StoredObject, Allocator>::Insert( const ValueType& value ) {
		return _underlyingContainer.insert( value );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::InsertResult OrderedMap<Key, StoredObject, Allocator>::Insert( const KeyType& key, const MappedType& value ) {
		return _underlyingContainer.insert( ValueType( key, value ) );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::SizeType OrderedMap<Key, StoredObject, Allocator>::Erase( const KeyType& key ) {
		return _underlyingContainer.erase( key );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint void OrderedMap<Key, StoredObject, Allocator>::Erase( Iterator position ) {
		_underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint void OrderedMap<Key, StoredObject, Allocator>::Erase( Iterator begin, Iterator end ) {
		while( begin != end ) {
			_underlyingContainer.erase( begin++ );
		}
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint void OrderedMap<Key, StoredObject, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename OrderedMap<Key, StoredObject, Allocator>::SizeType OrderedMap<Key, StoredObject, Allocator>::Size() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint bool OrderedMap<Key, StoredObject, Allocator>::Empty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint OrderedMap<Key, StoredObject, Allocator>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename const OrderedMap<Key, StoredObject, Allocator>::AllocatorType& OrderedMap<Key, StoredObject, Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2

namespace std {

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint ETNoAliasHint auto begin( ::Eldritch2::OrderedMap<Key, StoredObject, Allocator>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint ETNoAliasHint auto begin( const ::Eldritch2::OrderedMap<Key, StoredObject, Allocator>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint ETNoAliasHint auto end( ::Eldritch2::OrderedMap<Key, StoredObject, Allocator>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint ETNoAliasHint auto end( const ::Eldritch2::OrderedMap<Key, StoredObject, Allocator>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std