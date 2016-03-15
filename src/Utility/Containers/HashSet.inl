/*==================================================================*\
  HashSet.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
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

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::HashSet( AllocatorType&& allocator ) : HashSet( Hasher(), ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::HashSet( Hasher&& hasher, AllocatorType&& allocator ) : _underlyingContainer( 0u, ::std::move( hasher ), ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	template <class AlternateAllocator, int alternateLoadFactor>
	ETInlineHint HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::HashSet( const ::Eldritch2::HashSet<StoredObject, Hasher, KeyEqualityComparator, AlternateAllocator, alternateLoadFactor>& containerTemplate, AllocatorType&& allocator ) : _underlyingContainer( containerTemplate, ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::HashSet( ::Eldritch2::HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>&& moveSource ) : _underlyingContainer( ::std::move( moveSource ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Iterator HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Find( const KeyType& key ) {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	template <typename AlternateKey>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Iterator HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Find( const AlternateKey& key ) {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstIterator HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Find( const KeyType& key ) const {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	template <typename AlternateKey>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstIterator HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Find( const AlternateKey& key ) const {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	template <typename Predicate>
	ETInlineHint void HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::RemoveIf( Predicate predicate ) {
		for( Iterator element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element ) ) {
				Erase( element++ );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Iterator HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstIterator HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstIterator HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstBegin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Iterator HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstIterator HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstIterator HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ConstEnd() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::operator=( const HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& containerTemplate ) {
		_underlyingContainer = containerTemplate;
		return *this;
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint void HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::CloneFrom( const HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& containerTemplate ) {
		_underlyingContainer = containerTemplate._underlyingContainer;
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint void HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Swap( HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& rhs ) {
		_underlyingContainer.swap( rhs );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::InsertResult HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Insert( ValueType&& value ) {
		return _underlyingContainer.insert( ::std::move( value ) );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::InsertResult HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Insert( const ValueType& value ) {
		return _underlyingContainer.insert( value );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::SizeType HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Erase( const KeyType& key ) {
		return _underlyingContainer.erase( key );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint void HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Erase( Iterator position ) {
		_underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint void HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Erase( Iterator begin, Iterator end ) {
		_underlyingContainer.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint void HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	template <typename Disposer>
	ETInlineHint void HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::ClearAndDispose( Disposer disposer ) {
		for( auto&& value : _underlyingContainer ) {
			disposer( value );
		}
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::SizeType HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Size() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::SizeType HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Empty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint void HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::Reserve( const SizeType minimumSizeHint ) {
		_underlyingContainer.reserve( minimumSizeHint );
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::SizeType HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::BucketCount() const {
		return _underlyingContainer.bucket_count();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::SizeType HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::NonemptyBucketCount() const {
		return _underlyingContainer.nonempty_bucket_count();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::SizeType HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::UsedMemory() const {
		return _underlyingContainer.used_memory();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint typename const HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::AllocatorType& HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2

namespace std {

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint ETNoAliasHint auto begin( ::Eldritch2::HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint ETNoAliasHint auto begin( const ::Eldritch2::HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint ETNoAliasHint auto end( ::Eldritch2::HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <typename StoredObject, class Hasher, class KeyEqualityComparator, class Allocator, int loadFactor>
	ETInlineHint ETNoAliasHint auto end( const ::Eldritch2::HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std

