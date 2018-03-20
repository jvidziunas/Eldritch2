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
#include <Common/Containers/Range.hpp>
#include <Common/Algorithms.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashSet(
		const HashPredicate& hashPredicate,
		const EqualityPredicate& equalityPredicate,
		const AllocatorType& allocator
	) : _container( hashPredicate, equalityPredicate, allocator ) {}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashSet(
		const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& set,
		const AllocatorType& allocator
	) : _container( set._container, allocator ) {}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashSet(
		const AllocatorType& allocator
	) : _container( allocator ) {}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashSet(
		HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>&& set
	) : _container( eastl::move( set._container ) ) {}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateValue, typename AlternateHashPredicate, typename AlternateEqualityPredicate>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find(
		const AlternateValue& value,
		AlternateHashPredicate alternateHashPredicate,
		AlternateEqualityPredicate alternateEqualityPredicate
	) const {
		return _container.find_as( value, eastl::forward<AlternateHashPredicate>( alternateHashPredicate ), eastl::forward<AlternateEqualityPredicate>( alternateEqualityPredicate ) );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename AlternateValue, typename AlternateHashPredicate, typename AlternateEqualityPredicate>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find(
		const AlternateValue& value,
		AlternateHashPredicate alternateHashPredicate,
		AlternateEqualityPredicate alternateEqualityPredicate
	) {
		return _container.find_as( value, eastl::forward<AlternateHashPredicate>( alternateHashPredicate ), eastl::forward<AlternateEqualityPredicate>( alternateEqualityPredicate ) );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find( const ValueType& value ) {
		return _container.find( value );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find( const ValueType& value ) const {
		return _container.find( value );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint bool HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Contains( const ValueType& value ) const {
		return _container.find( value ) != _container.end();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename Predicate>
	ETInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EraseIf( Predicate predicate ) {
		for (auto element( _container.begin() ), end( _container.end() ); element != end;) {
			if (predicate( *element )) {
				element = _container.erase( element );
				continue;
			}

			++element;	
		}
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin() {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstBegin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End() {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstEnd() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::operator=(
		const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& set
	) {
		_container = set._container;
		return *this;
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::operator=(
		HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>&& set
	) {
		_container = eastl::move( set._container );
		return *this;
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint Pair<typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Insert( ValueType&& value ) {
		return _container.insert( eastl::forward<ValueType>( value ) );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <typename... Args>
	ETInlineHint Pair<typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Emplace( Args&&... args ) {
		return _container.emplace( eastl::forward<Args>( args )... );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	template <class /*SFINAE*/>
	ETInlineHint Pair<typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Insert( const ValueType& value ) {
		return _container.insert( value );
	}

// ---------------------------------------------------
	
	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase( Iterator begin, Iterator end ) {
		return _container.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase( Iterator position ) {
		return _container.erase( position );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::SizeType HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase( const ValueType& value ) {
		return _container.erase( value );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Clear() {
		_container.clear();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::SizeType HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetSize() const {
		return _container.size();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint bool HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::IsEmpty() const {
		return _container.empty();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::operator bool() const {
		return !_container.empty();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Reserve( SizeType minimumSizeHint ) {
		_container.reserve( minimumSizeHint );
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EqualityPredicateType& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetEqualityPredicate() const {
		return _container.key_eq();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashPredicateType& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetHashPredicate() const {
		return _container.hash_function();
	}

// ---------------------------------------------------

	template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint typename const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::AllocatorType& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetAllocator() const {
		return _container.get_allocator();
	}

// ---------------------------------------------------

	template<typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
	ETInlineHint void Swap( HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& set0, HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& set1 ) {
		eastl::swap( set0._container, set1._container );
	}

}	// namespace Eldritch2