/*==================================================================*\
  UnorderedMap.inl
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
#include <Utility/Algorithms.hpp>
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::UnorderedMap( AllocatorType&& allocator ) : _underlyingContainer( ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::UnorderedMap( const SizeType initialCapacity, Hasher&& hasher, AllocatorType&& allocator ) : _underlyingContainer( initialCapacity, ::std::move( hasher ), ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	template <class AlternateAllocator, int alternateLoadFactor>
	ETInlineHint UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::UnorderedMap( const ::Eldritch2::UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, AlternateAllocator, alternateLoadFactor>& containerTemplate, AllocatorType&& allocator ) : _underlyingContainer( containerTemplate, ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::UnorderedMap( ::Eldritch2::UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>&& moveSource ) : _underlyingContainer( ::std::move( moveSource ) ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Iterator UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Find( const KeyType& key ) {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstIterator UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Find( const KeyType& key ) const {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	template <typename Predicate>
	ETInlineHint void UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::RemoveIf( Predicate predicate ) {
		for( Iterator element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element ) ) {
				Erase( element++ );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Iterator UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstIterator UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstIterator UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstBegin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Iterator UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstIterator UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstIterator UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstEnd() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::MappedType& UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::operator[]( KeyType&& key ) {
		return _underlyingContainer[::std::move( key )];
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::MappedType& UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::operator[]( const KeyType& key ) {
		return _underlyingContainer[key];
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::operator=( const UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& containerTemplate ) {
		_underlyingContainer = containerTemplate;
		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint void UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::CloneFrom( const UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& containerTemplate ) {
		_underlyingContainer = containerTemplate._underlyingContainer;
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint void UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Swap( UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& rhs ) {
		_underlyingContainer.swap( rhs );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::InsertResult UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Insert( ValueType&& value ) {
		return _underlyingContainer.insert( ::std::move( value ) );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::InsertResult UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Insert( const ValueType& value ) {
		return _underlyingContainer.insert( value );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::SizeType UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Erase( const KeyType& key ) {
		return _underlyingContainer.erase( key );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint void UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Erase( Iterator position ) {
		_underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint void UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Erase( Iterator begin, Iterator end ) {
		_underlyingContainer.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint void UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	template <typename Disposer>
	ETInlineHint void UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ClearAndDispose( Disposer disposer ) {
		for( auto&& value : _underlyingContainer ) {
			disposer( value );
		}
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::SizeType UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Size() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::SizeType UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Empty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint void UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Reserve( const SizeType minimumSizeHint ) {
		_underlyingContainer.reserve( minimumSizeHint );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::SizeType UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::BucketCount() const {
		return _underlyingContainer.bucket_count();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::SizeType UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::NonemptyBucketCount() const {
		return _underlyingContainer.nonempty_bucket_count();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::SizeType UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::UsedMemory() const {
		return _underlyingContainer.used_memory();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename const UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::AllocatorType& UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2

namespace std {

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint ETNoAliasHint auto begin( ::Eldritch2::UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint ETNoAliasHint auto begin( const ::Eldritch2::UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint ETNoAliasHint auto end( ::Eldritch2::UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint ETNoAliasHint auto end( const ::Eldritch2::UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std