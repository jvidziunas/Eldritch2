/*==================================================================*\
  HashMap.inl
  ------------------------------------------------------------------
  Purpose:
  
  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMap(
		const HashPredicateType& hashPredicate,
		const EqualityPredicateType& keyEqualityPredicate,
		const AllocatorType& allocator
	) : _container( hashPredicate, keyEqualityPredicate, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMap(
		const HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& map
	) : _container( map._container ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMap(
		SizeType bucketCount,
		const HashPredicateType& hash,
		const EqualityPredicateType& equalityPredicate,
		const AllocatorType& allocator
	) : _container( bucketCount, hash, equalityPredicate, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMap(
		const AllocatorType& allocator
	) : _container( allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMap(
		HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>&& map,
		const AllocatorType& allocator
	) : _container( eastl::move( map._container ), allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMap(
		HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>&& map
	) : _container( eastl::move( map._container ) ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Find(
		const AlternateKey& key,
		const AlternateHashPredicate& hashPredicate,
		const AlternateKeyEqualityPredicate& keyEqualityPredicate
	) const {
		return _container.find_as( key, hashPredicate, keyEqualityPredicate );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Find(
		const AlternateKey& key,
		const AlternateHashPredicate& hashPredicate,
		const AlternateKeyEqualityPredicate& keyEqualityPredicate
	) {
		return _container.find_as( key, hashPredicate, keyEqualityPredicate );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Find( const KeyType& key ) {
		return _container.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Find( const KeyType& key ) const {
		return _container.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename Predicate>
	ETInlineHint void HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::EraseIf( Predicate predicate ) {
		for (Iterator element( _container.begin() ), end( _container.end() ); element != end;) {
			if (predicate( *element )) {
				element = _container.erase( element );
				continue;
			}

			++element;
		}
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
	ETInlineHint bool HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ContainsKey( const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate ) const {
		return this->Find( key, hashPredicate, keyEqualityPredicate ) != this->End();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint bool HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ContainsKey( const KeyType& key ) const {
		return this->Find( key ) != this->End();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstBegin( SizeType bucketIndex ) const {
		return _container.begin( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstBegin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstEnd( SizeType bucketIndex ) const {
		return _container.end( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstEnd() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin( SizeType bucketIndex ) const {
		return _container.begin( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::LocalIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin( SizeType bucketIndex ) {
		return _container.begin( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin() {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End( SizeType bucketIndex ) const {
		return _container.end( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::LocalIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End( SizeType bucketIndex ) {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End() {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::MappedType& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator[]( const KeyType& key ) {
		return _container[key];
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::MappedType& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator[]( KeyType&& key ) {
		return _container[eastl::forward<KeyType>( key )];
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator=(
		const HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& map
	) {
		_container = map._container;
		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator=(
		HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>&& map
	) {
		_container = eastl::move( map._container );
		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint Pair<typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Insert( const ValueType& value ) {
		return _container.insert( value );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint Pair<typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Insert( ValueType&& value ) {
		return _container.insert( eastl::forward<ValueType>( value ) );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename... Arguments>
	Pair<typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::TryEmplace( const KeyType& key, Arguments&&... arguments ) {
		return _container.try_emplace( key, eastl::forward<Arguments>( arguments )... );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename... Arguments>
	Pair<typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::TryEmplace( KeyType&& key, Arguments&&... arguments ) {
		return _container.try_emplace( eastl::forward<KeyType>( key ), eastl::forward<Arguments>( arguments )... );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename... Arguments>
	ETInlineHint Pair<typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Emplace( Arguments&&... arguments ) {
		return _container.emplace( eastl::forward<Arguments>( arguments )... );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::SizeType HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Erase( const KeyType& key ) {
		return _container.erase( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Erase( Iterator begin, Iterator end ) {
		return _container.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Erase( Iterator position ) {
		return _container.erase( position );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Clear() {
		_container.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename Disposer>
	ETInlineHint void HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ClearAndDispose( Disposer disposer ) {
		for (ValueType& value : _container) {
			disposer( value );
		}

		_container.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::SizeType HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetSize() const {
		return _container.size();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint bool HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::IsEmpty() const {
		return _container.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator bool() const {
		return !_container.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::EqualityPredicateType& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetKeyEqualityPredicate() const {
		return _container.key_eq();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashPredicateType& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetHashPredicate() const {
		return _container.hash_predicate();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::AllocatorType& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetAllocator() const {
		return _container.get_allocator();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void Swap( HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& map0, HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& map1 ) {
		eastl::swap( map0._container, map1._container );
	}

}	// namespace Eldritch2