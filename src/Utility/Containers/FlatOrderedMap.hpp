/*==================================================================*\
  FlatOrderedMap.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#include <rdestl/functional.h>
#include <rdestl/pair.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename StoredObject, class OrderingPredicate = ::rde::less<Key>, class Allocator = ::Eldritch2::ChildAllocator>
	class FlatOrderedMap : private ::Eldritch2::ResizableArray<::rde::pair<Key, StoredObject>, Allocator> {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		typedef ::Eldritch2::ResizableArray<::rde::pair<Key, StoredObject>, Allocator>	UnderlyingContainer;

	public:
		using UnderlyingContainer::ValueType;
		typedef typename ValueType::first_type											KeyType;
		typedef typename ValueType::second_type											MappedType;
		typedef typename OrderingPredicate												OrderingPredicateType;
		using UnderlyingContainer::AllocatorType;
		using UnderlyingContainer::Iterator;
		using UnderlyingContainer::ConstIterator;
		using UnderlyingContainer::SizeType;
		typedef ::rde::pair<Iterator, bool>												InsertResult;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref FlatOrderedMap instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint FlatOrderedMap( const SizeType reservedSizeInElements, OrderingPredicateType orderingPredicate, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		//! Constructs this @ref FlatOrderedMap instance.
		template <typename InputIterator, typename... AllocatorConstructorArguments>
		ETInlineHint FlatOrderedMap( InputIterator begin, InputIterator end, OrderingPredicateType orderingPredicate, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		//! Constructs this @ref FlatOrderedMap instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint FlatOrderedMap( const ::Eldritch2::FlatOrderedMap<Key, StoredObject, OrderingPredicate, Allocator>& containerTemplate, AllocatorConstructorArguments&&... allocatorConstructorArguments );

		//! Destroys this @ref FlatOrderedMap instance.
		ETInlineHint ~FlatOrderedMap() = default;

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

		ETInlineHint void	CloneFrom( const ::Eldritch2::FlatOrderedMap<Key, StoredObject, OrderingPredicate, Allocator>& container );
		
		ETInlineHint void	Swap( ::Eldritch2::FlatOrderedMap<Key, StoredObject, OrderingPredicate, Allocator>&	container );

	// - CONTAINER MANIPULATION --------------------------

		ETInlineHint InsertResult	Insert( ValueType&& v );

		ETInlineHint SizeType		Erase( const KeyType& key );
		ETInlineHint Iterator		Erase( const Iterator position );
		ETInlineHint void			Erase( const Iterator begin, const Iterator end );

		using UnderlyingContainer::Clear;

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
#include <Utility/Containers/FlatOrderedMap.inl>
//------------------------------------------------------------------//