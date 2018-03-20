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
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMultiMap(
		const HashPredicateType& hashPredicate,
		const KeyEqualityPredicateType& keyEqualityPredicate,
		const AllocatorType& allocator
	) : _container( hashPredicate, keyEqualityPredicate, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMultiMap(
		const HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& map,
		const AllocatorType& allocator
	) : _container( map._container, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMultiMap(
		SizeType capacityHint,
		const AllocatorType& allocator
	) : _container( capacityHint, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMultiMap(
		const AllocatorType& allocator
	) : _container( allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMultiMap(
		HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>&& map
	) : _container( eastl::move( map._container ) ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
	ETInlineHint Range<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::EqualRange(
		const AlternateKey& key,
		const AlternateHashPredicate& hashPredicate,
		const AlternateKeyEqualityPredicate& keyEqualityPredicate
	) const {
		const auto	resultPair( _container.equal_range( key, hashPredicate, keyEqualityPredicate ) );

		return { resultPair.first, resultPair.second };
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
	ETInlineHint Range<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::EqualRange(
		const AlternateKey& key,
		const AlternateHashPredicate& hashPredicate,
		const AlternateKeyEqualityPredicate& keyEqualityPredicate
	) {
		const auto	resultPair( _container.equal_range( key, hashPredicate, keyEqualityPredicate ) );

		return { resultPair.first, resultPair.second };
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint Range<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::EqualRange( const KeyType& key ) const {
		const auto	resultPair( _container.equal_range( key ) );

		return { resultPair.first, resultPair.second };
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint Range<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::EqualRange( const KeyType& key ) {
		const auto	resultPair( _container.equal_range( key ) );

		return { resultPair.first, resultPair.second };
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename Predicate>
	ETInlineHint void HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::EraseIf( Predicate predicate ) {
		for (Iterator element( _container.begin() ), end( _container.end() ); element != end;) {
			if (predicate( *element )) {
				element = _container.erase( element );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstBegin( SizeType bucketIndex ) const {
		return _container.begin( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstBegin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstEnd( SizeType bucketIndex ) const {
		return _container.end( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstEnd() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin( SizeType bucketIndex ) const {
		return _container.begin( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::LocalIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin( SizeType bucketIndex ) {
		return _container.begin( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin() {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End( SizeType bucketIndex ) const {
		return _container.end( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::LocalIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End( SizeType bucketIndex ) {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End() {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator=( const HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& map ) {
		_container = map._container;

		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator=( HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>&& map ) {
		_container = eastl::move( map._container );

		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint Pair<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Insert( const ValueType& value ) {
		return _container.insert( value );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint Pair<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Insert( ValueType&& value ) {
		return _container.insert( eastl::move( value ) );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename... Arguments>
	ETInlineHint Pair<typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Emplace( Arguments&&... arguments ) {
		return _container.emplace( eastl::forward<Arguments>( arguments )... );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::SizeType HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Erase( const KeyType& key ) {
		return _container.erase( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Erase( Iterator begin, Iterator end ) {
		return _container.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Erase( Iterator position ) {
		return _container.erase( position );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Clear() {
		_container.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename Disposer>
	ETInlineHint void HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ClearAndDispose( Disposer disposer ) {
		for (ValueType& value : _container) {
			disposer( value );
		}

		_container.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::SizeType HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetSize() const {
		return _container.size();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint bool HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::IsEmpty() const {
		return _container.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator bool() const {
		return !_container.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashPredicateType& HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetHashPredicate() const {
		return _container.hash_predicate();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::AllocatorType& HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetAllocator() const {
		return _container.get_allocator();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void Swap( HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& map0, HashMultiMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& map1 ) {
		eastl::swap( map0._container, map1._container );
	}

}	// namespace Eldritch2