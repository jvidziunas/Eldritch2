/*==================================================================*\
  ArraySet.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Algorithms.hpp>
//------------------------------------------------------------------//
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint ArraySet<Value, OrderingPredicate, Allocator>::ArraySet( SizeType reservedSizeInElements, const OrderingPredicateType& orderingPredicate, const AllocatorType& allocator ) : _underlyingContainer( reservedSizeInElements, orderingPredicate, allocator ) {}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	template <typename InputIterator>
	ETInlineHint ArraySet<Value, OrderingPredicate, Allocator>::ArraySet( InputIterator begin, InputIterator end, const OrderingPredicateType& orderingPredicate, const AllocatorType& allocator ) : _underlyingContainer( begin, end, orderingPredicate, allocator ) {}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint ArraySet<Value, OrderingPredicate, Allocator>::ArraySet( const ArraySet<Value, OrderingPredicate, Allocator>& containerTemplate, const AllocatorType& allocator ) : _underlyingContainer( containerTemplate.GetOrderingPredicate(), allocator ) {
		_underlyingContainer.insert( containerTemplate.Begin(), containerTemplate.End() );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint ArraySet<Value, OrderingPredicate, Allocator>::ArraySet( const AllocatorType& allocator ) : _underlyingContainer( allocator ) {}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint ArraySet<Value, OrderingPredicate, Allocator>::ArraySet( ArraySet<Value, OrderingPredicate, Allocator>&& moveSource ) : _underlyingContainer( eastl::move( moveSource._containerTemplate ) ) {}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::ConstIterator ArraySet<Value, OrderingPredicate, Allocator>::LowerBound( const ValueType& value ) const {
		return _underlyingContainer.lower_bound( value );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::Iterator ArraySet<Value, OrderingPredicate, Allocator>::LowerBound( const ValueType& value ) {
		return _underlyingContainer.lower_bound( value );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::ConstIterator ArraySet<Value, OrderingPredicate, Allocator>::UpperBound( const ValueType& value ) const {
		return _underlyingContainer.upper_bound( value );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::Iterator ArraySet<Value, OrderingPredicate, Allocator>::UpperBound( const ValueType& value ) {
		return _underlyingContainer.upper_bound( value );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	template <typename AlternateValue, typename BinaryPredicate>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::ConstIterator ArraySet<Value, OrderingPredicate, Allocator>::Find( const AlternateValue& value, BinaryPredicate comparisonPredicate ) const {
		return _underlyingContainer.find_as( value, comparisonPredicate );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	template <typename AlternateValue, typename BinaryPredicate>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::Iterator ArraySet<Value, OrderingPredicate, Allocator>::Find( const AlternateValue& value, BinaryPredicate comparisonPredicate ) {
		return _underlyingContainer.find_as( value, comparisonPredicate );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::ConstIterator ArraySet<Value, OrderingPredicate, Allocator>::Find( const ValueType& value ) const {
		return _underlyingContainer.find( value );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::Iterator ArraySet<Value, OrderingPredicate, Allocator>::Find( const ValueType& value ) {
		return _underlyingContainer.find( value );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	template <typename AlternateValue, typename BinaryPredicate>
	ETInlineHint bool ArraySet<Value, OrderingPredicate, Allocator>::Contains( const AlternateValue& value, BinaryPredicate comparisonPredicate ) const {
		return _underlyingContainer.find( value, comparisonPredicate ) != _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint bool ArraySet<Value, OrderingPredicate, Allocator>::Contains( const ValueType& value ) const {
		return _underlyingContainer.find( value ) != _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	template <typename Predicate>
	ETInlineHint void ArraySet<Value, OrderingPredicate, Allocator>::RemoveIf( Predicate predicate ) {
		for( auto iterator( _underlyingContainer.begin() ); iterator != _underlyingContainer.end(); ) {
			if( predicate( *iterator ) ) {
				iterator = _underlyingContainer.erase( iterator );
				continue;
			}

			++iterator;
		}
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::ConstIterator ArraySet<Value, OrderingPredicate, Allocator>::ConstBegin() const {
		return _underlyingContainer.cbegin();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::ConstIterator ArraySet<Value, OrderingPredicate, Allocator>::ConstEnd() const {
		return _underlyingContainer.cend();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::ConstIterator ArraySet<Value, OrderingPredicate, Allocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::Iterator ArraySet<Value, OrderingPredicate, Allocator>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::ConstIterator ArraySet<Value, OrderingPredicate, Allocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::Iterator ArraySet<Value, OrderingPredicate, Allocator>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint ArraySet<Value, OrderingPredicate, Allocator>& ArraySet<Value, OrderingPredicate, Allocator>::operator=( const ArraySet<Value, OrderingPredicate, Allocator>& other ) {
		_underlyingContainer = other._underlyingContainer;

		return *this;
	}

	// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint ArraySet<Value, OrderingPredicate, Allocator>& ArraySet<Value, OrderingPredicate, Allocator>::operator=( ArraySet<Value, OrderingPredicate, Allocator>&& other ) {
		_underlyingContainer = eastl::move( other._underlyingContainer );

		return *this;
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint void ArraySet<Value, OrderingPredicate, Allocator>::Swap( ArraySet<Value, OrderingPredicate, Allocator>& container ) {
		_underlyingContainer.swap( container );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	template <typename... Arguments>
	ETInlineHint Eldritch2::Pair<typename ArraySet<Value, OrderingPredicate, Allocator>::Iterator, bool> ArraySet<Value, OrderingPredicate, Allocator>::Emplace( Arguments&&... arguments ) {
		return _underlyingContainer.emplace( eastl::forward<Arguments>( arguments )... );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint Eldritch2::Pair<typename ArraySet<Value, OrderingPredicate, Allocator>::Iterator, bool> ArraySet<Value, OrderingPredicate, Allocator>::Insert( const ValueType& value ) {
		return _underlyingContainer.insert( value );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint Eldritch2::Pair<typename ArraySet<Value, OrderingPredicate, Allocator>::Iterator, bool> ArraySet<Value, OrderingPredicate, Allocator>::Insert( ValueType&& value ) {
		return _underlyingContainer.insert( eastl::forward<ValueType>( value ) );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::SizeType ArraySet<Value, OrderingPredicate, Allocator>::Erase( const ValueType& value ) {
		return _underlyingContainer.erase( value );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::Iterator ArraySet<Value, OrderingPredicate, Allocator>::Erase( Iterator beginPosition, Iterator endPosition ) {
		return _underlyingContainer.erase( beginPosition, endPosition );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::Iterator ArraySet<Value, OrderingPredicate, Allocator>::Erase( Iterator position ) {
		return _underlyingContainer.erase( value );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint void ArraySet<Value, OrderingPredicate, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::SizeType ArraySet<Value, OrderingPredicate, Allocator>::GetSize() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint bool ArraySet<Value, OrderingPredicate, Allocator>::IsEmpty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint ArraySet<Value, OrderingPredicate, Allocator>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename ArraySet<Value, OrderingPredicate, Allocator>::SizeType ArraySet<Value, OrderingPredicate, Allocator>::GetCapacity() const {
		return _underlyingContainer.capacity();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint void ArraySet<Value, OrderingPredicate, Allocator>::SetCapacity( SizeType capacity ) {
		_underlyingContainer.set_capacity( capacity );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint void ArraySet<Value, OrderingPredicate, Allocator>::Reserve( SizeType sizeHint ) {
		_underlyingContainer.reserve( sizeHint );
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename const ArraySet<Value, OrderingPredicate, Allocator>::OrderingPredicateType& ArraySet<Value, OrderingPredicate, Allocator>::GetOrderingPredicate() const {
		return _underlyingContainer.key_comp();
	}

// ---------------------------------------------------

	template <typename Value, typename OrderingPredicate, class Allocator>
	ETInlineHint typename const ArraySet<Value, OrderingPredicate, Allocator>::AllocatorType& ArraySet<Value, OrderingPredicate, Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2