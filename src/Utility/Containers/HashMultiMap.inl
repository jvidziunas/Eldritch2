/*==================================================================*\
  HashMultiMap.inl
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

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMultiMap( const HashPredicateType& hashPredicate, const KeyEqualityPredicateType& keyEqualityPredicate, const AllocatorType& allocator ) : _underlyingContainer( hashPredicate, keyEqualityPredicate, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMultiMap( const HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& containerTemplate, const AllocatorType& allocator ) : _underlyingContainer( containerTemplate._underlyingContainer, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMultiMap( SizeType sizeHint, const AllocatorType& allocator ) : _underlyingContainer( sizeHint, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMultiMap( const AllocatorType& allocator ) : _underlyingContainer( allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMultiMap( HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>&& moveSource ) : _underlyingContainer( eastl::move( moveSource._underlyingContainer ) ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
	ETInlineHint Eldritch2::Range<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::EqualRange( const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate ) const {
		const auto	resultPair( _underlyingContainer.equal_range( key, hashPredicate, keyEqualityPredicate ) );

		return { resultPair.first, resultPair.second };
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
	ETInlineHint Eldritch2::Range<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::EqualRange( const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate ) {
		const auto	resultPair( _underlyingContainer.equal_range( key, hashPredicate, keyEqualityPredicate ) );

		return { resultPair.first, resultPair.second };
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint Eldritch2::Range<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::EqualRange( const KeyType& key ) const {
		const auto	resultPair( _underlyingContainer.equal_range( key ) );

		return { resultPair.first, resultPair.second };
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint Eldritch2::Range<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::EqualRange( const KeyType& key ) {
		const auto	resultPair( _underlyingContainer.equal_range( key ) );

		return { resultPair.first, resultPair.second };
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename Predicate>
	ETInlineHint void HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::RemoveIf( Predicate predicate ) {
		for( Iterator element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element ) ) {
				element = _underlyingContainer.erase( element );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstBegin( SizeType bucketIndex ) const {
		return _underlyingContainer.begin( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstBegin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstEnd( SizeType bucketIndex ) const {
		return _underlyingContainer.end( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstEnd() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin( SizeType bucketIndex ) const {
		return _underlyingContainer.begin( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::LocalIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin( SizeType bucketIndex ) {
		return _underlyingContainer.begin( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End( SizeType bucketIndex ) const {
		return _underlyingContainer.end( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::LocalIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End( SizeType bucketIndex ) {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator=( const HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& containerTemplate ) {
		_underlyingContainer = containerTemplate;

		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator=( HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>&& containerTemplate ) {
		_underlyingContainer = eastl::move( containerTemplate._underlyingContainer );

		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Swap( HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& rhs ) {
		_underlyingContainer.swap( rhs );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint Eldritch2::Pair<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Insert( const ValueType& value ) {
		return _underlyingContainer.insert( value );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint Eldritch2::Pair<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Insert( ValueType&& value ) {
		return _underlyingContainer.insert( eastl::move( value ) );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename... Arguments>
	ETInlineHint Eldritch2::Pair<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Emplace( Arguments&&... arguments ) {
		return _underlyingContainer.emplace( eastl::forward<Arguments>( arguments )... );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::SizeType HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Erase( const KeyType& key ) {
		return _underlyingContainer.erase( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Erase( Iterator begin, Iterator end ) {
		return _underlyingContainer.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Erase( Iterator position ) {
		return _underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename Disposer>
	ETInlineHint void HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ClearAndDispose( Disposer disposer ) {
		for( auto& value : _underlyingContainer ) {
			disposer( value );
		}

		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::SizeType HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetSize() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::SizeType HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::IsEmpty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashPredicateType& HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetHashPredicate() const {
		return _underlyingContainer.hash_predicate();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::AllocatorType& HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2

