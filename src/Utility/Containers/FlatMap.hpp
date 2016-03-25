/*==================================================================*\
  FlatMap.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/Less.hpp>
#include <Utility/Pair.hpp>
//------------------------------------------------------------------//
#include <rdestl/functional.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename StoredObject, class OrderingPredicate = ::Eldritch2::Less<Key>, class Allocator = ::Eldritch2::ChildAllocator>
	class FlatMap : private ::Eldritch2::ResizableArray<::Eldritch2::Pair<Key, StoredObject>, Allocator> {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using UnderlyingContainer					= ::Eldritch2::ResizableArray<::Eldritch2::Pair<Key, StoredObject>, Allocator>;

	public:
		using UnderlyingContainer::ValueType;
		using KeyType								= typename ValueType::first_type;
		using MappedType							= typename ValueType::second_type;
		using OrderingPredicateType					= OrderingPredicate;
		using UnderlyingContainer::AllocatorType;
		using UnderlyingContainer::Iterator;
		using UnderlyingContainer::ConstIterator;
		using UnderlyingContainer::SizeType;
		using InsertResult							= ::Eldritch2::Pair<Iterator, bool>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref FlatMap instance.
		ETInlineHint explicit FlatMap( AllocatorType&& allocator = AllocatorType() );
		//! Constructs this @ref FlatMap instance.
		ETInlineHint FlatMap( const SizeType reservedSizeInElements, OrderingPredicateType&& orderingPredicate = OrderingPredicate(), AllocatorType&& allocator = AllocatorType() );
		//! Constructs this @ref FlatMap instance.
		template <typename InputIterator>
		ETInlineHint FlatMap( InputIterator begin, InputIterator end, OrderingPredicateType&& orderingPredicate = OrderingPredicate(), AllocatorType&& allocator = AllocatorType() );
		//! Constructs this @ref FlatMap instance.
		ETInlineHint explicit FlatMap( const ::Eldritch2::FlatMap<Key, StoredObject, OrderingPredicate, Allocator>& containerTemplate, AllocatorType&& allocator = AllocatorType() );
		//! Constructs this @ref FlatMap instance.
		ETInlineHint FlatMap( ::Eldritch2::FlatMap<Key, StoredObject, OrderingPredicate, Allocator>&& moveSource );

		ETInlineHint ~FlatMap() = default;

	// - ALGORITHMS --------------------------------------

		ETInlineHint Iterator			LowerBound( const KeyType& key );
		ETInlineHint ConstIterator		LowerBound( const KeyType& key ) const;

		ETInlineHint Iterator			UpperBound( const KeyType& key );
		ETInlineHint Iterator			UpperBound( const KeyType& key ) const;

		ETInlineHint Iterator			Find( const KeyType& key );
		ETInlineHint ConstIterator		Find( const KeyType& key ) const;

		ETInlineHint MappedType&		Find( const KeyType& key, MappedType& defaultValue );
		ETInlineHint const MappedType&	Find( const KeyType& key, const MappedType& defaultValue );

		using UnderlyingContainer::RemoveIf;

	// - ELEMENT ITERATION -------------------------------

		using UnderlyingContainer::Begin;
		using UnderlyingContainer::ConstBegin;
		using UnderlyingContainer::End;
		using UnderlyingContainer::ConstEnd;

	// - CONTAINER DUPLICATION ---------------------------

		ETInlineHint void	CloneFrom( const ::Eldritch2::FlatMap<Key, StoredObject, OrderingPredicate, Allocator>& container );
		
		ETInlineHint void	Swap( ::Eldritch2::FlatMap<Key, StoredObject, OrderingPredicate, Allocator>&	container );

	// - CONTAINER MANIPULATION --------------------------

		ETInlineHint InsertResult	Insert( ValueType&& v );

		ETInlineHint SizeType		Erase( const KeyType& key );
		ETInlineHint Iterator		Erase( const Iterator position );
		ETInlineHint void			Erase( const Iterator begin, const Iterator end );

		using UnderlyingContainer::Clear;
		using UnderlyingContainer::ClearAndDispose;

	// - CONTENT QUERY -----------------------------------

		using UnderlyingContainer::Size;
		using UnderlyingContainer::IsEmpty;
		using UnderlyingContainer::operator bool;

	// - CAPACITY QUERY ----------------------------------

		using UnderlyingContainer::GetCapacity;
		using UnderlyingContainer::SetCapacity;
		using UnderlyingContainer::Reserve;
		using UnderlyingContainer::GetHighWatermark;

	// - ORDERING PREDICATE ACCESS -----------------------

		ETInlineHint OrderingPredicateType&			GetOrderingPredicate();
		ETInlineHint const OrderingPredicateType&	GetOrderingPredicate() const;

	// - ALLOCATOR ACCESS --------------------------------

		using UnderlyingContainer::GetAllocator;

	// - DATA MEMBERS ------------------------------------

	private:
		OrderingPredicateType	_orderingPredicate;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/FlatMap.inl>
//------------------------------------------------------------------//