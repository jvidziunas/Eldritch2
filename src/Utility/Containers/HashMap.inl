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
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMap( const HashPredicateType& hashPredicate, const KeyEqualityPredicateType& keyEqualityPredicate, const AllocatorType& allocator ) : _underlyingContainer( hashPredicate, keyEqualityPredicate, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMap( const Eldritch2::HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& containerTemplate ) : _underlyingContainer( containerTemplate._underlyingContainer ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMap( SizeType sizeHint, const AllocatorType& allocator ) : _underlyingContainer( sizeHint, HashPredicateType(), KeyEqualityPredicateType(), allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMap( const AllocatorType& allocator ) : _underlyingContainer( allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashMap( Eldritch2::HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>&& moveSource ) : _underlyingContainer( eastl::move( moveSource._underlyingContainer ) ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Find( const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate ) const {
		return _underlyingContainer.find_as( key, hashPredicate, keyEqualityPredicate );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Find( const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate ) {
		return _underlyingContainer.find_as( key, hashPredicate, keyEqualityPredicate );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Find( const KeyType& key ) {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Find( const KeyType& key ) const {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename Predicate>
	ETInlineHint void HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::RemoveIf( Predicate predicate ) {
		for( Iterator element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element ) ) {
				element = _underlyingContainer.erase( element );
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
		return _underlyingContainer.begin( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstBegin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstEnd( SizeType bucketIndex ) const {
		return _underlyingContainer.end( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstEnd() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin( SizeType bucketIndex ) const {
		return _underlyingContainer.begin( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::LocalIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin( SizeType bucketIndex ) {
		return _underlyingContainer.begin( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstLocalIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End( SizeType bucketIndex ) const {
		return _underlyingContainer.end( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::LocalIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End( SizeType bucketIndex ) {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::MappedType& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator[]( const KeyType& key ) {
		return _underlyingContainer[key];
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::MappedType& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator[]( KeyType&& key ) {
		return _underlyingContainer[eastl::forward<KeyType>( key )];
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator=( const HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& containerTemplate ) {
		_underlyingContainer = containerTemplate._underlyingContainer;
		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator=( HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>&& containerTemplate ) {
		_underlyingContainer = eastl::move( containerTemplate._underlyingContainer );
		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Swap( HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& rhs ) {
		_underlyingContainer.swap( rhs );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint Eldritch2::Pair<typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Insert( const ValueType& value ) {
		return _underlyingContainer.insert( value );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint Eldritch2::Pair<typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Insert( ValueType&& value ) {
		return _underlyingContainer.insert( eastl::forward<ValueType>( value ) );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename... Arguments>
	ETInlineHint Eldritch2::Pair<typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Emplace( Arguments&&... arguments ) {
		return _underlyingContainer.emplace( eastl::forward<Arguments>( arguments )... );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::SizeType HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Erase( const KeyType& key ) {
		return _underlyingContainer.erase( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Erase( Iterator begin, Iterator end ) {
		return _underlyingContainer.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Iterator HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Erase( Iterator position ) {
		return _underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename Disposer>
	ETInlineHint void HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::ClearAndDispose( Disposer disposer ) {
		for( auto& value : _underlyingContainer ) {
			disposer( value );
		}

		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::SizeType HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetSize() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::SizeType HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::IsEmpty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::KeyEqualityPredicateType& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetKeyEqualityPredicate() const {
		return _underlyingContainer.key_eq();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::HashPredicateType& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetHashPredicate() const {
		return _underlyingContainer.hash_predicate();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::AllocatorType& HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2