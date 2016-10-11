/*==================================================================*\
  ArrayMap.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint ArrayMap<Key, Value, OrderingPredicate, Allocator>::ArrayMap( const SizeType reservedSizeInElements, const OrderingPredicateType& orderingPredicate, const AllocatorType& allocator ) : _underlyingContainer( reservedSizeInElements, orderingPredicate, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	template <typename InputIterator>
	ETInlineHint ArrayMap<Key, Value, OrderingPredicate, Allocator>::ArrayMap( InputIterator begin, InputIterator end, const OrderingPredicateType& orderingPredicate, const AllocatorType& allocator ) : _underlyingContainer( begin, end, orderingPredicate, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint ArrayMap<Key, Value, OrderingPredicate, Allocator>::ArrayMap( const Eldritch2::ArrayMap<Key, Value, OrderingPredicate, Allocator>& containerTemplate, const AllocatorType& allocator ) : _underlyingContainer( containerTemplate, allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint ArrayMap<Key, Value, OrderingPredicate, Allocator>::ArrayMap( Eldritch2::ArrayMap<Key, Value, OrderingPredicate, Allocator>&& moveSource ) : _underlyingContainer( eastl::move( moveSource._underlyingContainer ) ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint ArrayMap<Key, Value, OrderingPredicate, Allocator>::ArrayMap( const AllocatorType& allocator ) : _underlyingContainer( allocator ) {}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, OrderingPredicate, Allocator>::LowerBound( const KeyType& key ) const {
		return _underlyingContainer.lower_bound( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::Iterator ArrayMap<Key, Value, OrderingPredicate, Allocator>::LowerBound( const KeyType& key ) {
		return _underlyingContainer.lower_bound( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::Iterator ArrayMap<Key, Value, OrderingPredicate, Allocator>::UpperBound( const KeyType& key ) const {
		return _underlyingContainer.upper_bound( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::Iterator ArrayMap<Key, Value, OrderingPredicate, Allocator>::UpperBound( const KeyType& key ) {
		return _underlyingContainer.upper_bound( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	template <typename AlternateKeyType, typename AlternateOrderingPredicate>
	ETInlineHint typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, OrderingPredicate, Allocator>::Find( const AlternateKeyType& key, AlternateOrderingPredicate orderingPredicate ) const {
		return _underlyingContainer.find_as( key, orderingPredicate );
	}
	
// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::ConstIterator ArrayMap<Key, Value, OrderingPredicate, Allocator>::Find( const KeyType& key ) const {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	template <typename AlternateKeyType, typename AlternateOrderingPredicate>
	ETInlineHint typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::Iterator ArrayMap<Key, Value, OrderingPredicate, Allocator>::Find( const AlternateKeyType& key, AlternateOrderingPredicate orderingPredicate ) {
		return _underlyingContainer.find_as( key, orderingPredicate );
	}
	
// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::Iterator ArrayMap<Key, Value, OrderingPredicate, Allocator>::Find( const KeyType& key ) {
		return _underlyingContainer.find( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint Eldritch2::Pair<typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, OrderingPredicate, Allocator>::Insert( const ValueType& value ) {
		return _underlyingContainer.insert( value );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint Eldritch2::Pair<typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, OrderingPredicate, Allocator>::Insert( ValueType&& value ) {
		return _underlyingContainer.insert( eastl::forward<ValueType>( value ) );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	template <typename... Args>
	ETInlineHint Eldritch2::Pair<typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, OrderingPredicate, Allocator>::Emplace( ConstIterator position, Args&&... args ) {
		return _underlyingContainer.emplace( position, eastl::forward<Args>( args )... );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	template <typename... Args>
	ETInlineHint Eldritch2::Pair<typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::Iterator, bool> ArrayMap<Key, Value, OrderingPredicate, Allocator>::Emplace( Args&&... args ) {
		return _underlyingContainer.emplace( eastl::forward<Args>( args )... );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::SizeType ArrayMap<Key, Value, OrderingPredicate, Allocator>::Erase( const KeyType& key ) {
		return _underlyingContainer.erase( key );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::Iterator ArrayMap<Key, Value, OrderingPredicate, Allocator>::Erase( Iterator position ) {
		return _underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::Iterator ArrayMap<Key, Value, OrderingPredicate, Allocator>::Erase( Iterator begin, Iterator end ) {
		return _underlyingContainer.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint void ArrayMap<Key, Value, OrderingPredicate, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename const ArrayMap<Key, Value, OrderingPredicate, Allocator>::OrderingPredicateType& ArrayMap<Key, Value, OrderingPredicate, Allocator>::GetOrderingPredicate() const {
		return _underlyingContainer.key_comp();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArrayMap<Key, Value, OrderingPredicate, Allocator>::OrderingPredicateType& ArrayMap<Key, Value, OrderingPredicate, Allocator>::GetOrderingPredicate() {
		return _underlyingContainer.key_comp();
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint ArrayMap<Key, Value, OrderingPredicate, Allocator>& ArrayMap<Key, Value, OrderingPredicate, Allocator>::operator=( const ArrayMap<Key, Value, OrderingPredicate, Allocator>& other ) {
		_underlyingContainer = other._underlyingContainer;
		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint ArrayMap<Key, Value, OrderingPredicate, Allocator>& ArrayMap<Key, Value, OrderingPredicate, Allocator>::operator=( ArrayMap<Key, Value, OrderingPredicate, Allocator>&& other ) {
		_underlyingContainer = eastl::move( other._underlyingContainer );
		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint ArrayMap&	operator=( ArrayMap&& );

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint void ArrayMap<Key, Value, OrderingPredicate, Allocator>::SetCapacity( SizeType sizeInElements ) {
		_underlyingContainer.set_capacity( sizeInElements );
	}

// ---------------------------------------------------

	template <typename Key, typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint void ArrayMap<Key, Value, OrderingPredicate, Allocator>::Reserve( SizeType sizeInElements ) {
		_underlyingContainer.reserve( sizeInElements );
	}

}	// namespace Eldritch2