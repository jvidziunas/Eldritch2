/*==================================================================*\
  FlatSet.inl
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
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint FlatSet<StoredObject, OrderingPredicate, Allocator>::FlatSet( AllocatorType&& allocator ) : UnderlyingContainer( ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint FlatSet<StoredObject, OrderingPredicate, Allocator>::FlatSet( const SizeType reservedSizeInElements, OrderingPredicateType&& orderingPredicate, AllocatorType&& allocator ) : UnderlyingContainer( reservedSizeInElements, ::std::move( allocator ) ), _orderingPredicate( ::std::move( orderingPredicate ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	template <typename InputIterator>
	ETInlineHint FlatSet<StoredObject, OrderingPredicate, Allocator>::FlatSet( InputIterator begin, InputIterator end, OrderingPredicateType&& orderingPredicate, AllocatorType&& allocator ) : UnderlyingContainer( begin, end, ::std::move( allocator ) ), _orderingPredicate( ::std::move( orderingPredicate ) ) {
		UnderlyingContainer::Sort( _orderingPredicate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	template <class AlternateAllocator>
	ETInlineHint FlatSet<StoredObject, OrderingPredicate, Allocator>::FlatSet( const ::Eldritch2::FlatSet<StoredObject, OrderingPredicate, AlternateAllocator>& containerTemplate, AllocatorType&& allocator ) : UnderlyingContainer( containerTemplate.Begin(), containerTemplate.End(), ::std::move( allocator ) ), _orderingPredicate( containerTemplate.GetOrderingPredicate() ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint FlatSet<StoredObject, OrderingPredicate, Allocator>::FlatSet( ::Eldritch2::FlatSet<StoredObject, OrderingPredicate, Allocator>&& moveSource ) : UnderlyingContainer( ::std::move( moveSource ) ), _orderingPredicate( ::std::move( moveSource._orderingPredicate ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatSet<StoredObject, OrderingPredicate, Allocator>::Iterator FlatSet<StoredObject, OrderingPredicate, Allocator>::LowerBound( const ValueType& value ) {
		auto&	orderingPredicate( GetOrderingPredicate() );

		return Utility::LowerBound( this->Begin(), this->End(), value, [&orderingPredicate] ( const ValueType& element, const ValueType& value ) mutable {
			return orderingPredicate( element, value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatSet<StoredObject, OrderingPredicate, Allocator>::ConstIterator FlatSet<StoredObject, OrderingPredicate, Allocator>::LowerBound( const ValueType& value ) const {
		auto&	orderingPredicate( GetOrderingPredicate() );

		return Utility::LowerBound( this->Begin(), this->End(), value, [&orderingPredicate] ( const ValueType& element, const ValueType& value ) mutable {
			return orderingPredicate( element, value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatSet<StoredObject, OrderingPredicate, Allocator>::Iterator FlatSet<StoredObject, OrderingPredicate, Allocator>::UpperBound( const ValueType& value ) {
		auto&	orderingPredicate( GetOrderingPredicate() );

		return Utility::UpperBound( this->Begin(), this->End(), value, [&orderingPredicate] ( const ValueType& element, const ValueType& value ) mutable {
			return orderingPredicate( element, value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatSet<StoredObject, OrderingPredicate, Allocator>::Iterator FlatSet<StoredObject, OrderingPredicate, Allocator>::UpperBound( const ValueType& value ) const {
		auto&	orderingPredicate( GetOrderingPredicate() );

		return Utility::UpperBound( this->Begin(), this->End(), value, [&orderingPredicate] ( const ValueType& element, const ValueType& value ) mutable {
			return orderingPredicate( element, value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatSet<StoredObject, OrderingPredicate, Allocator>::Iterator FlatSet<StoredObject, OrderingPredicate, Allocator>::Find( const ValueType& value ) {
		auto	location( LowerBound( value ) );

		if( location != this->End() && _orderingPredicate( value, *location ) ) {
			location = this->End();
		}

		return location;
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatSet<StoredObject, OrderingPredicate, Allocator>::ConstIterator FlatSet<StoredObject, OrderingPredicate, Allocator>::Find( const ValueType& value ) const {
		auto	location( LowerBound( value ) );

		if( location != this->End() && _orderingPredicate( value, *location ) ) {
			location = this->End();
		}

		return location;
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint bool FlatSet<StoredObject, OrderingPredicate, Allocator>::Contains( const ValueType& value ) const {
		return this->End() != this->Find( value );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint void FlatSet<StoredObject, OrderingPredicate, Allocator>::CloneFrom( const ::Eldritch2::FlatSet<StoredObject, OrderingPredicate, Allocator>& container ) {
		UnderlyingContainer::CloneFrom( static_cast<UnderlyingContainer&>(container) );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint void FlatSet<StoredObject, OrderingPredicate, Allocator>::Swap( ::Eldritch2::FlatSet<StoredObject, OrderingPredicate, Allocator>& container ) {
		UnderlyingContainer::Swap( static_cast<UnderlyingContainer&>(container) );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatSet<StoredObject, OrderingPredicate, Allocator>::InsertResult FlatSet<StoredObject, OrderingPredicate, Allocator>::Insert( const ValueType& value ) {
		auto	result( LowerBound( value ) );
		bool	found( true );

		if( (this->End() == result) || _orderingPredicate( *result, value ) ) {
			result = UnderlyingContainer::Insert( result, value );
			found = false;
		}

		return InsertResult( result, !found );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatSet<StoredObject, OrderingPredicate, Allocator>::SizeType FlatSet<StoredObject, OrderingPredicate, Allocator>::Erase( const ValueType& value ) {
		const auto	_( this->Find( value ) );

		if( this->End() != _ ) {
			return (this->Erase( _ ), 1u);
		}

		return 0u;
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatSet<StoredObject, OrderingPredicate, Allocator>::Iterator FlatSet<StoredObject, OrderingPredicate, Allocator>::Erase( const Iterator position ) {
		return UnderlyingContainer::Erase( position );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint void FlatSet<StoredObject, OrderingPredicate, Allocator>::Erase( Iterator beginPosition, Iterator endPosition ) {
		UnderlyingContainer::Erase( beginPosition, endPosition );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename FlatSet<StoredObject, OrderingPredicate, Allocator>::OrderingPredicateType& FlatSet<StoredObject, OrderingPredicate, Allocator>::GetOrderingPredicate() {
		return _orderingPredicate;
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint typename const FlatSet<StoredObject, OrderingPredicate, Allocator>::OrderingPredicateType& FlatSet<StoredObject, OrderingPredicate, Allocator>::GetOrderingPredicate() const {
		return _orderingPredicate;
	}

}	// namespace Eldritch2

namespace std {

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint ETNoAliasHint auto begin( ::Eldritch2::FlatSet<StoredObject, OrderingPredicate, Allocator>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint ETNoAliasHint auto begin( const ::Eldritch2::FlatSet<StoredObject, OrderingPredicate, Allocator>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint ETNoAliasHint auto end( ::Eldritch2::FlatSet<StoredObject, OrderingPredicate, Allocator>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename OrderingPredicate, class Allocator>
	ETInlineHint ETNoAliasHint auto end( const ::Eldritch2::FlatSet<StoredObject, OrderingPredicate, Allocator>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std