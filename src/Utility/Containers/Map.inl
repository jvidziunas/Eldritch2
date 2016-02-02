/*==================================================================*\
  Map.inl
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
	ETInlineHint Map<Key, StoredObject, Allocator>::Map( AllocatorType&& allocator ) : _underlyingContainer( ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	template <typename InputIterator, typename... AllocatorConstructorArguments>
	ETInlineHint Map<Key, StoredObject, Allocator>::Map( InputIterator begin, InputIterator end, AllocatorType&& allocator ) : _underlyingContainer( begin, end, ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	template <class AlternateAllocator, typename... AllocatorConstructorArguments>
	ETInlineHint Map<Key, StoredObject, Allocator>::Map( const ::Eldritch2::Map<Key, StoredObject, AlternateAllocator>& containerTemplate, AllocatorType&& allocator ) : _underlyingContainer( containerTemplate.Begin(), containerTemplate.End(), ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint Map<Key, StoredObject, Allocator>::Map( ::Eldritch2::Map<Key, StoredObject, Allocator>&& moveSource ) : _underlyingContainer( ::std::move( moveSource ) ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename Map<Key, StoredObject, Allocator>::Iterator Map<Key, StoredObject, Allocator>::Find( const KeyType& key ) {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename Map<Key, StoredObject, Allocator>::ConstIterator Map<Key, StoredObject, Allocator>::Find( const KeyType& key ) const {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	template <typename Predicate>
	ETInlineHint void Map<Key, StoredObject, Allocator>::RemoveIf( Predicate predicate ) {
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
	ETInlineHint void Map<Key, StoredObject, Allocator>::RemoveIf( ExtraArgumentType extraArgument, Predicate predicate ) {
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
	ETInlineHint typename Map<Key, StoredObject, Allocator>::Iterator Map<Key, StoredObject, Allocator>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename Map<Key, StoredObject, Allocator>::ConstIterator Map<Key, StoredObject, Allocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename Map<Key, StoredObject, Allocator>::ConstIterator Map<Key, StoredObject, Allocator>::ConstBegin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename Map<Key, StoredObject, Allocator>::Iterator Map<Key, StoredObject, Allocator>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename Map<Key, StoredObject, Allocator>::ConstIterator Map<Key, StoredObject, Allocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename Map<Key, StoredObject, Allocator>::ConstIterator Map<Key, StoredObject, Allocator>::ConstEnd() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename Map<Key, StoredObject, Allocator>::MappedType& Map<Key, StoredObject, Allocator>::operator[]( const KeyType& key ) {
		return static_cast<UnderlyingContainer&>( *this )[key];
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint void Map<Key, StoredObject, Allocator>::CloneFrom( const ::Eldritch2::Map<Key, StoredObject, Allocator>& containerTemplate ) {
		static_cast<UnderlyingContainer&>( *this ) = containerTemplate;
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint void Map<Key, StoredObject, Allocator>::Swap( ::Eldritch2::Map<Key, StoredObject, Allocator>& other ) {
		_underlyingContainer.swap( other );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename Map<Key, StoredObject, Allocator>::InsertResult Map<Key, StoredObject, Allocator>::Insert( const ValueType& value ) {
		return _underlyingContainer.insert( value );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename Map<Key, StoredObject, Allocator>::InsertResult Map<Key, StoredObject, Allocator>::Insert( const KeyType& key, const MappedType& value ) {
		return _underlyingContainer.insert( ValueType( key, value ) );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename Map<Key, StoredObject, Allocator>::SizeType Map<Key, StoredObject, Allocator>::Erase( const KeyType& key ) {
		return _underlyingContainer.erase( key );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint void Map<Key, StoredObject, Allocator>::Erase( Iterator position ) {
		_underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint void Map<Key, StoredObject, Allocator>::Erase( Iterator begin, Iterator end ) {
		while( begin != end ) {
			_underlyingContainer.erase( begin++ );
		}
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint void Map<Key, StoredObject, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename Map<Key, StoredObject, Allocator>::SizeType Map<Key, StoredObject, Allocator>::Size() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint bool Map<Key, StoredObject, Allocator>::Empty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint Map<Key, StoredObject, Allocator>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint typename const Map<Key, StoredObject, Allocator>::AllocatorType& Map<Key, StoredObject, Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2

namespace std {

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint ETNoAliasHint auto begin( ::Eldritch2::Map<Key, StoredObject, Allocator>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint ETNoAliasHint auto begin( const ::Eldritch2::Map<Key, StoredObject, Allocator>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint ETNoAliasHint auto end( ::Eldritch2::Map<Key, StoredObject, Allocator>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Allocator>
	ETInlineHint ETNoAliasHint auto end( const ::Eldritch2::Map<Key, StoredObject, Allocator>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std