/*==================================================================*\
  FlatMap.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Algorithms.hpp>
//------------------------------------------------------------------//
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::FlatMap( AllocatorType&& allocator ) : UnderlyingContainer( ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::FlatMap( const SizeType reservedSizeInElements, OrderingPredicateType&& orderingPredicate, AllocatorType&& allocator ) : UnderlyingContainer( reservedSizeInElements, ::std::move( allocator ) ), _orderingPredicate( ::std::move( orderingPredicate ) ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	template <typename InputIterator>
	ETInlineHint FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::FlatMap( InputIterator begin, InputIterator end, OrderingPredicateType&& orderingPredicate, AllocatorType&& allocator ) : UnderlyingContainer( begin, end, ::std::move( allocator ) ), _orderingPredicate( ::std::move( orderingPredicate ) ) {
		UnderlyingContainer::Sort( _orderingPredicate );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::FlatMap( const ::Eldritch2::FlatMap<Key, StoredObject, OrderingPredicate, Allocator>& containerTemplate, AllocatorType&& allocator ) : UnderlyingContainer( containerTemplate, ::std::move( allocator ) ), _orderingPredicate( containerTemplate.GetOrderingPredicate() ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::FlatMap( ::Eldritch2::FlatMap<Key, StoredObject, OrderingPredicate, Allocator>&& moveSource ) : UnderlyingContainer( ::std::move( moveSource ) ), _orderingPredicate( ::std::move( moveSource._orderingPredicate ) ) {}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Iterator FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::LowerBound( const KeyType& key ) {
		auto&	orderingPredicate( GetOrderingPredicate() );

		return Utility::LowerBound( this->Begin(), this->End(), key, [&orderingPredicate] ( const ValueType& element, const KeyType& key ) mutable {
			return orderingPredicate( element.first, key );
		} );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::ConstIterator FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::LowerBound( const KeyType& key ) const {
		const auto&	orderingPredicate( GetOrderingPredicate() );

		return Utility::LowerBound( this->Begin(), this->End(), key, [&orderingPredicate] ( const ValueType& element, const KeyType& key ) mutable {
			return orderingPredicate( element.first, key );
		} );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Iterator FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::UpperBound( const KeyType& key ) {
		auto&	orderingPredicate( GetOrderingPredicate() );

		return Utility::UpperBound( this->Begin(), this->End(), key, [&orderingPredicate] ( const ValueType& element, const KeyType& key ) mutable {
			return orderingPredicate( element.first, key );
		} );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Iterator FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::UpperBound( const KeyType& key ) const {
		const auto&	orderingPredicate( GetOrderingPredicate() );

		return Utility::UpperBound( this->Begin(), this->End(), key, [&orderingPredicate] ( const ValueType& element, const KeyType& key ) mutable {
			return orderingPredicate( element.first, key );
		} );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Iterator FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Find( const KeyType& key ) {
		auto	location( LowerBound( key ) );

		if( location != this->End() && GetOrderingPredicate()(location->first, key) ) {
			location = this->End();
		}

		return location;
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::ConstIterator FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Find( const KeyType& key ) const {
		auto	location( LowerBound( key ) );

		if( location != this->End() && GetOrderingPredicate()(location->first, key) ) {
			location = this->End();
		}

		return location;
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::MappedType& FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Find( const KeyType& key, MappedType& defaultValue ) {
		auto	location( LowerBound( key ) );

		if( location != this->End() ) {
			return GetOrderingPredicate()(location->first, key) ? defaultValue : location->second;
		}

		return defaultValue;
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename const FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::MappedType& FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Find( const KeyType& key, const MappedType& defaultValue ) {
		auto	location( LowerBound( key ) );

		if( location != this->End() ) {
			return GetOrderingPredicate()(location->first, key) ? defaultValue : location->second;
		}

		return defaultValue;
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint void FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::CloneFrom( const ::Eldritch2::FlatMap<Key, StoredObject, OrderingPredicate, Allocator>& container ) {
		UnderlyingContainer::CloneFrom( static_cast<UnderlyingContainer&>(container) );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint void FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Swap( ::Eldritch2::FlatMap<Key, StoredObject, OrderingPredicate, Allocator>& container ) {
		UnderlyingContainer::Swap( static_cast<UnderlyingContainer&>(container) );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::InsertResult FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Insert( ValueType&& v ) {
		auto	result( LowerBound( v.first ) );
		bool	found( true );

		if( (this->End() == result) || _orderingPredicate( result->first, v.first ) ) {
			result = UnderlyingContainer::Insert( result, ::std::move(v) );
			found = false;
		}

		return InsertResult( result, !found );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::SizeType FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Erase( const KeyType& key ) {
		const auto	_( this->Find( key ) );

		if( this->End() != _ ) {
			return ( this->Erase( _ ), 1u );
		}

		return 0u;
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Iterator FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Erase( const Iterator position ) {
		return UnderlyingContainer::Erase( position );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint void FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::Erase( const Iterator begin, const Iterator end ) {
		UnderlyingContainer::Erase( begin, end );
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::OrderingPredicateType& FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::GetOrderingPredicate() {
		return _orderingPredicate;
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename const FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::OrderingPredicateType& FlatMap<Key, StoredObject, OrderingPredicate, Allocator>::GetOrderingPredicate() const {
		return _orderingPredicate;
	}

}	// namespace Eldritch2

namespace std {

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint ETNoAliasHint auto begin( ::Eldritch2::FlatMap<Key, StoredObject, OrderingPredicate, Allocator>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint ETNoAliasHint auto begin( const ::Eldritch2::FlatMap<Key, StoredObject, OrderingPredicate, Allocator>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint ETNoAliasHint auto end( ::Eldritch2::FlatMap<Key, StoredObject, OrderingPredicate, Allocator>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <typename Key, typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint ETNoAliasHint auto end( const ::Eldritch2::FlatMap<Key, StoredObject, OrderingPredicate, Allocator>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std