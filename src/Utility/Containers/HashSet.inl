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
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashSet( const HashPredicate& hashPredicate, const EqualityPredicate& equalityPredicate, const AllocatorType& allocator ) : _underlyingContainer( hashPredicate, equalityPredicate, allocator ) {}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashSet( const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& containerTemplate, const AllocatorType& allocator ) : _underlyingContainer( containerTemplate._underlyingContainer, allocator ) {}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashSet( const AllocatorType& allocator ) : _underlyingContainer( allocator ) {}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashSet( HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>&& moveSource ) : _underlyingContainer( eastl::move( moveSource._underlyingContainer ) ) {}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateValue, typename AlternateHashPredicate, typename AlternateEqualityPredicate>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find( const AlternateValue& value, const AlternateHashPredicate& alternateHashPredicate, const AlternateEqualityPredicate& alternateEqualityPredicate ) const {
		return _underlyingContainer.find_as( value, alternateHashPredicate, alternateEqualityPredicate );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateValue, typename AlternateHashPredicate, typename AlternateEqualityPredicate>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find( const AlternateValue& value, const AlternateHashPredicate& alternateHashPredicate, const AlternateEqualityPredicate& alternateEqualityPredicate ) {
		return _underlyingContainer.find_as( value, alternateHashPredicate, alternateEqualityPredicate );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find( const ValueType& value ) {
		return _underlyingContainer.find( value );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find( const ValueType& value ) const {
		return _underlyingContainer.find( value );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint bool HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Contains( const ValueType& value ) const {
		return _underlyingContainer.find( value ) != _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename Predicate>
	ETInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::RemoveIf( Predicate predicate ) {
		for( auto element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element ) ) {
				element = _underlyingContainer.erase( element );
				continue;
			}

			++element;	
		}
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstBegin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstEnd() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::operator=( const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& other ) {
		_underlyingContainer = other._underlyingContainer;
		return *this;
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::operator=( HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>&& other ) {
		_underlyingContainer = eastl::move( other._underlyingContainer );
		return *this;
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Swap( HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& rhs ) {
		_underlyingContainer.swap( rhs );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint Eldritch2::Pair<typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Insert( ValueType&& value ) {
		return _underlyingContainer.insert( eastl::forward<ValueType>( value ) );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename... Args, class /*SFINAE*/>
	ETInlineHint Eldritch2::Pair<typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Emplace( Args&&... args ) {
		return _underlyingContainer.emplace( eastl::forward<Args>( args )... );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint Eldritch2::Pair<typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Insert( const ValueType& value ) {
		return _underlyingContainer.insert( value );
	}

// ---------------------------------------------------
	
	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase( Iterator begin, Iterator end ) {
		return _underlyingContainer.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase( Iterator position ) {
		return _underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::SizeType HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase( const ValueType& value ) {
		return _underlyingContainer.erase( value );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::SizeType HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetSize() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::SizeType HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::IsEmpty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Reserve( SizeType minimumSizeHint ) {
		_underlyingContainer.reserve( minimumSizeHint );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EqualityPredicateType& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetEqualityPredicate() const {
		return _underlyingContainer.key_eq();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashPredicateType& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetHashPredicate() const {
		return _underlyingContainer.hash_function();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::AllocatorType& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2

