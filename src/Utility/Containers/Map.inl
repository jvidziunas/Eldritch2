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
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename Value, class Allocator>
	ETInlineHint Map<Key, Value, Allocator>::Map( const AllocatorType& allocator ) : _underlyingContainer( allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint Map<Key, Value, Allocator>::Map( InputIterator begin, InputIterator end, const AllocatorType& allocator ) : _underlyingContainer( begin, end, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint Map<Key, Value, Allocator>::Map( const Map<Key, Value, AlternateAllocator>& containerTemplate, const AllocatorType& allocator ) : _underlyingContainer( containerTemplate._underlyingContainer, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint Map<Key, Value, Allocator>::Map( Map<Key, Value, Allocator>&& moveSource ) : _underlyingContainer( eastl::move( moveSource._underlyingContainer ) ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint typename Map<Key, Value, Allocator>::Iterator Map<Key, Value, Allocator>::Find( const KeyType& key ) {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint typename Map<Key, Value, Allocator>::ConstIterator Map<Key, Value, Allocator>::Find( const KeyType& key ) const {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	template <typename Predicate>
	ETInlineHint void Map<Key, Value, Allocator>::RemoveIf( Predicate predicate ) {
		for( Iterator element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element ) ) {
				_underlyingContainer.erase( element++ );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	template <typename ExtraArgumentType, typename Predicate>
	ETInlineHint void Map<Key, Value, Allocator>::RemoveIf( ExtraArgumentType extraArgument, Predicate predicate ) {
		for( Iterator element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element, extraArgument ) ) {
				_underlyingContainer.erase( element++ );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint typename Map<Key, Value, Allocator>::Iterator Map<Key, Value, Allocator>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint typename Map<Key, Value, Allocator>::ConstIterator Map<Key, Value, Allocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint typename Map<Key, Value, Allocator>::ConstIterator Map<Key, Value, Allocator>::ConstBegin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint typename Map<Key, Value, Allocator>::Iterator Map<Key, Value, Allocator>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint typename Map<Key, Value, Allocator>::ConstIterator Map<Key, Value, Allocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint typename Map<Key, Value, Allocator>::ConstIterator Map<Key, Value, Allocator>::ConstEnd() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint typename Map<Key, Value, Allocator>::MappedType& Map<Key, Value, Allocator>::operator[]( const KeyType& key ) {
		return _underlyingContainer[key];
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint Map<Key, Value, Allocator>& Map<Key, Value, Allocator>::operator=( const Map<Key, Value, Allocator>& other ) {
		_underlyingContainer = other._underlyingContainer;

		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint Map<Key, Value, Allocator>& Map<Key, Value, Allocator>::operator=( Map<Key, Value, Allocator>&& ) {
		_underlyingContainer = eastl::move( other._underlyingContainer );

		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint void Map<Key, Value, Allocator>::Swap( Eldritch2::Map<Key, Value, Allocator>& other ) {
		_underlyingContainer.swap( other );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint typename Map<Key, Value, Allocator>::InsertResult Map<Key, Value, Allocator>::Insert( const ValueType& value ) {
		return _underlyingContainer.insert( value );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint typename Map<Key, Value, Allocator>::InsertResult Map<Key, Value, Allocator>::Insert( const KeyType& key, const MappedType& value ) {
		return _underlyingContainer.insert( ValueType( key, value ) );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint typename Map<Key, Value, Allocator>::SizeType Map<Key, Value, Allocator>::Erase( const KeyType& key ) {
		return _underlyingContainer.erase( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint void Map<Key, Value, Allocator>::Erase( Iterator position ) {
		_underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint void Map<Key, Value, Allocator>::Erase( Iterator begin, Iterator end ) {
		while( begin != end ) {
			_underlyingContainer.erase( begin++ );
		}
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint void Map<Key, Value, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint typename Map<Key, Value, Allocator>::SizeType Map<Key, Value, Allocator>::GetSize() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint bool Map<Key, Value, Allocator>::IsEmpty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint Map<Key, Value, Allocator>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class Allocator>
	ETInlineHint typename const Map<Key, Value, Allocator>::AllocatorType& Map<Key, Value, Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2